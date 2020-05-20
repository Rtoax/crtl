#include <malloc.h>

#include "crtl/bits/rwlock.h"
#include "crtl/bits/semaphore.h"


#include "crtl/tree.h"

#include "crtl/log.h"
#include "crtl/easy/macro.h"
#include "crtl/easy/attribute.h"
#include "crtl/sm.h"
#include "crtl/assert.h"

#include "crypto/tree/rbtree.h"

#include "crypto/list/list.h"
#include "crypto/atomic/atomic.h"


/* 状态机事件处理结构体 */
struct __crtl_sm_evthandler {
    
    crtl_sm_event_t wait_event; /* 等待发生的事件 */

    crtl_sm_handler_fn_t event_handler; /* 事件处理函数 */

    crtl_sm_state_t next_state;   /* 下一个状态 */

    crtl_lock_rw_t event_rwlock; /* 事件处理节点 读写锁 */

    /* 用户参数 */
    int user_argc;
    char **user_argv;

    struct crtl_sm_state_node *belongs_to_node;
    struct rb_node node_of_handler;
};


/* 状态机节点结构体 */
struct crtl_sm_state_node {
    
    crtl_sm_state_t curr_state;   /* 当前状态 */

    crtl_lock_rw_t state_rwlock; /* 状态读写锁 */

    crtl_atomic_t ref_tree_of_handlers; /* 标识“事件-处理函数”树是否被初始化 */
    
    struct rb_node node_of_state;   /* 状态 树， 属于一个 rb_root 节点 -> 属于 crtl_sm_t */
    struct rb_root tree_of_handlers;  /* 消息处理函数 树 */
    
};

/* 状态机 */
struct crtl_sm_struct {

    crtl_sm_state_t current_state;

    crtl_lock_rw_t sm_rwlock; /* 状态机读写锁 */
    crtl_atomic_t ref_init; /* 标记状态机中的树是否被初始化 */
    
    struct rb_root state_tree; /* 状态机中的状态树 */
    
    struct list_head all_sm_link; /* 用于底层存贮这个状态机，一面用户忘记删除 */
};



#define __SM_RDLOCK(p_sm) crtl_rwlock_rdlock(&((struct crtl_sm_struct*)p_sm)->sm_rwlock, 0, 0, 0, 0)
#define __SM_WRLOCK(p_sm) crtl_rwlock_wrlock(&((struct crtl_sm_struct*)p_sm)->sm_rwlock, 0, 0, 0, 0)
#define __SM_UNLOCK(p_sm) crtl_rwlock_unlock(&((struct crtl_sm_struct*)p_sm)->sm_rwlock)


#define __SM_STATE_RDLOCK(p_state) crtl_rwlock_rdlock(&((struct crtl_sm_state_node*)p_state)->state_rwlock, 0, 0, 0, 0)
#define __SM_STATE_WRLOCK(p_state) crtl_rwlock_wrlock(&((struct crtl_sm_state_node*)p_state)->state_rwlock, 0, 0, 0, 0)
#define __SM_STATE_UNLOCK(p_state) crtl_rwlock_unlock(&((struct crtl_sm_state_node*)p_state)->state_rwlock)

#define __SM_EVENT_RDLOCK(p_event) crtl_rwlock_rdlock(&((struct __crtl_sm_evthandler*)p_event)->event_rwlock, 0, 0, 0, 0)
#define __SM_EVENT_WRLOCK(p_event) crtl_rwlock_wrlock(&((struct __crtl_sm_evthandler*)p_event)->event_rwlock, 0, 0, 0, 0)
#define __SM_EVENT_UNLOCK(p_event) crtl_rwlock_unlock(&((struct __crtl_sm_evthandler*)p_event)->event_rwlock)


static LIST_HEAD_UNUSED(__crtl_all_sm_list);

/* 状态机列表的读写锁 */
static crtl_lock_rw_t _unused __crtl_all_sm_list_rwlock = CRTL_LOCK_RWLOCK_INITIALIZER;
#define __crtl_all_sm_list_rdlock crtl_rwlock_rdlock(&__crtl_all_sm_list_rwlock, 0,0,0,0)
#define __crtl_all_sm_list_wrlock crtl_rwlock_wrlock(&__crtl_all_sm_list_rwlock, 0,0,0,0)
#define __crtl_all_sm_list_unlock crtl_rwlock_unlock(&__crtl_all_sm_list_rwlock)


#define __ADD_SM_TO_LIST_WITH_RWLOCK(psm) \
        __crtl_all_sm_list_wrlock; \
        list_add_tail(&psm->all_sm_link, &__crtl_all_sm_list); \
        __crtl_all_sm_list_unlock;



static int _unused __insert_crtl_sm_state_node(struct crtl_sm_struct *root, struct crtl_sm_state_node *data)
{
	struct rb_node **tmp = &(root->state_tree.rb_node), *parent = NULL;
	while(*tmp)
	{
		struct crtl_sm_state_node *this = container_of(*tmp, struct crtl_sm_state_node, node_of_state);
		parent = *tmp;
		if(data->curr_state < this->curr_state)
		{
			tmp = &((*tmp)->rb_left);
		}
		else if(data->curr_state > this->curr_state)
		{
			tmp = &((*tmp)->rb_right);
		}
		else 
		{
			return CRTL_ERROR;
		}
	}
	rb_link_node(&data->node_of_state, parent, tmp);
	rb_insert_color(&data->node_of_state, &root->state_tree);
    
	return CRTL_SUCCESS;
}



static struct crtl_sm_state_node _unused * __search_crtl_sm_state_node(struct crtl_sm_struct *root, crtl_sm_state_t curr_state)
{
    crtl_assert(root);
    struct rb_node *node = root->state_tree.rb_node;
	while(node)
	{
		struct crtl_sm_state_node *data = container_of(node, struct crtl_sm_state_node, node_of_state);
		if(curr_state < data->curr_state)
		{
			node = node->rb_left;
		}
		else if(curr_state > data->curr_state)
		{
			node = node->rb_right;
		}
		else
			return data;
	}
	return NULL;
}


static void _unused __delete_crtl_sm_state_node(struct crtl_sm_struct *root, crtl_sm_state_t curr_state)
{
    struct crtl_sm_state_node *data = __search_crtl_sm_state_node(root, curr_state);
    if(!data) {
        __crtl_dbg("Not found state %d.\n", curr_state);
        return;
    }
    rb_erase(&data->node_of_state, &root->state_tree);
    crtl_rwlock_destroy(&data->state_rwlock);
    free(data);
}

static int _unused __insert_crtl_sm_event_node(struct crtl_sm_state_node *root, struct __crtl_sm_evthandler *data)
{
	struct rb_node **tmp = &(root->tree_of_handlers.rb_node), *parent = NULL;
	while(*tmp)
	{
		struct __crtl_sm_evthandler *this = container_of(*tmp, struct __crtl_sm_evthandler, node_of_handler);
		parent = *tmp;
		if(data->wait_event < this->wait_event)
		{
			tmp = &((*tmp)->rb_left);
		}
		else if(data->wait_event > this->wait_event)
		{
			tmp = &((*tmp)->rb_right);
		}
		else 
		{
			return CRTL_ERROR;
		}
	}
	rb_link_node(&data->node_of_handler, parent, tmp);
	rb_insert_color(&data->node_of_handler, &root->tree_of_handlers);
    
	return CRTL_SUCCESS;
}


static struct __crtl_sm_evthandler _unused * __search_crtl_sm_event_node(struct crtl_sm_state_node *root, crtl_sm_event_t event)
{
    crtl_assert(root);
    struct rb_node *node = root->tree_of_handlers.rb_node;
	while(node)
	{
		struct __crtl_sm_evthandler *data = container_of(node, struct __crtl_sm_evthandler, node_of_handler);
		if(event < data->wait_event)
		{
			node = node->rb_left;
		}
		else if(event > data->wait_event)
		{
			node = node->rb_right;
		}
		else
        {      
            __crtl_dbg("exist:--- %ld->%ld.\n", root->curr_state, event);
			return data;
        }
	}
	return NULL;
}


static void _unused __delete_crtl_sm_event_node(struct crtl_sm_state_node *root, crtl_sm_event_t event)
{
    struct __crtl_sm_evthandler *data = __search_crtl_sm_event_node(root, event);
    if(!data) {
        __crtl_dbg("Not found event %d.\n", event);
        return;
    }
    rb_erase(&data->node_of_handler, &root->tree_of_handlers);
    crtl_rwlock_destroy(&data->event_rwlock);
    free(data);
}


/*  */
crtl_sm_t crtl_sm_init(crtl_sm_state_t inti_state)
{
    crtl_sm_t user_sm = 0;
    
    struct crtl_sm_struct *sm = malloc(sizeof(struct crtl_sm_struct));
    if(unlikely(!sm)) {
        crtl_assert(sm);
        return 0;
    }
    user_sm = (crtl_sm_t)sm;
    
    crtl_lock_rwattr_t __attr;
    crtl_rwlockattr_init(&__attr);
    crtl_rwlockattr_setpshared_shared(&__attr);
    crtl_rwlock_init(&sm->sm_rwlock, &__attr);

    __SM_WRLOCK(sm);

    sm->current_state = inti_state; //初始状态
    sm->state_tree.rb_node = NULL; //
    crtl_atomic_set(&sm->ref_init, 1);
    
    __SM_UNLOCK(sm);

    crtl_rwlockattr_destroy(&__attr);

    /* 将状态机句柄添加至链表，用于存储 */
    __ADD_SM_TO_LIST_WITH_RWLOCK(sm);

    /* 初始化状态需要被注册 */
    crtl_sm_state_register(user_sm, sm->current_state);
    
    return user_sm;
}


int crtl_sm_state_register(crtl_sm_t sm, crtl_sm_state_t curr_state)
{
    crtl_assert(sm);

    struct crtl_sm_struct *sm_struct = (struct crtl_sm_struct *)sm;
    __crtl_dbg("%d.\n", sm_struct->ref_init.val);
    
    __SM_RDLOCK(sm_struct);
    struct crtl_sm_state_node *exist = __search_crtl_sm_state_node(sm_struct, curr_state);
    if(unlikely(exist)) {
        crtl_print_err("Already exist state node: %ld.\n", curr_state);
        __SM_UNLOCK(sm_struct);
        return CRTL_ERROR;
    }
    __SM_UNLOCK(sm_struct);
    
    struct crtl_sm_state_node *state_item = malloc(sizeof(struct crtl_sm_state_node));
    if(unlikely(!state_item)) {
        crtl_assert(state_item);
        return CRTL_ERROR;
    }

    state_item->curr_state = curr_state;
    __SM_WRLOCK(sm_struct);
    if(CRTL_ERROR == __insert_crtl_sm_state_node(sm_struct, state_item)) {
        crtl_print_err("Already exist state node: %ld.\n", curr_state);
        __SM_UNLOCK(sm_struct);
        free(state_item);
        return CRTL_ERROR;
    }
#if 0
    struct rb_node *node;
    for(node=rb_first(&sm_struct->state_tree); node; node=rb_next(node))
    {
        __crtl_dbg(">> curr_state: %ld \n", rb_entry(node, struct crtl_sm_state_node, node_of_state)->curr_state);
    }
#endif    
    crtl_lock_rwattr_t __attr;
    crtl_rwlockattr_init(&__attr);
    crtl_rwlockattr_setpshared_shared(&__attr);
    crtl_rwlock_init(&state_item->state_rwlock, &__attr);

    /* 锁定 状态 的树， 该树用于存放事件-处理函数（状态机驱动程序） */
    __SM_STATE_WRLOCK(state_item);

    state_item->tree_of_handlers.rb_node = NULL; //
    crtl_atomic_set(&state_item->ref_tree_of_handlers, 1);
    
    __SM_STATE_UNLOCK(state_item);
    
    crtl_rwlockattr_destroy(&__attr);
    __SM_UNLOCK(sm_struct);
    
    return CRTL_SUCCESS;
}


int crtl_sm_handler_register(crtl_sm_t sm, 
                                crtl_sm_state_t curr_state, 
                                crtl_sm_event_t event, 
                                crtl_sm_state_t next_state,
                                crtl_sm_handler_fn_t handler,
                                int argc, char *argv[])
{
    crtl_assert(sm);
    crtl_assert(handler);

    struct crtl_sm_struct *sm_struct = (struct crtl_sm_struct *)sm;
    __crtl_dbg("%d.\n", sm_struct->ref_init.val);

    /* 状态是否存在 */
    __SM_WRLOCK(sm_struct);
    struct crtl_sm_state_node *curr_state_exist = __search_crtl_sm_state_node(sm_struct, curr_state);
    if(unlikely(!curr_state_exist)) {
        crtl_print_err("Not exist state node: %ld.\n", curr_state);
        __SM_UNLOCK(sm_struct);
        return CRTL_ERROR;
    }
    /* 状态是否存在 */
    struct crtl_sm_state_node *next_state_exist = __search_crtl_sm_state_node(sm_struct, next_state);
    if(unlikely(!next_state_exist)) {
        crtl_print_err("Not exist state node: %ld.\n", next_state);
        __SM_UNLOCK(sm_struct);
        return CRTL_ERROR;
    }

    struct __crtl_sm_evthandler *event_handler = malloc(sizeof(struct __crtl_sm_evthandler));
    crtl_assert(event_handler);
    
    crtl_lock_rwattr_t __attr;
    crtl_rwlockattr_init(&__attr);
    crtl_rwlockattr_setpshared_shared(&__attr);
    crtl_rwlock_init(&event_handler->event_rwlock, &__attr);
    crtl_rwlockattr_destroy(&__attr);

    __SM_EVENT_WRLOCK(event_handler);
    
    event_handler->next_state = next_state;
    event_handler->belongs_to_node = curr_state_exist;
    event_handler->event_handler = handler;
    event_handler->user_argc = argc;
    event_handler->user_argv = argv;
    event_handler->wait_event = event;
    
    __SM_EVENT_UNLOCK(event_handler);

    __SM_STATE_WRLOCK(curr_state_exist);
    if(CRTL_ERROR == __insert_crtl_sm_event_node(curr_state_exist, event_handler)) {
        crtl_print_err("Already exist event(%ld) node in state(%ld).\n",event, curr_state);
        __SM_UNLOCK(sm_struct);
        __SM_STATE_UNLOCK(curr_state_exist);
        free(event_handler);
        return CRTL_ERROR;
    }
    
#if 0
    struct rb_node *node;
    for(node=rb_first(&curr_state_exist->tree_of_handlers); node; node=rb_next(node))
    {
        __crtl_dbg(">> wait_event: %ld \n", rb_entry(node, struct __crtl_sm_evthandler, node_of_handler)->wait_event);
    }
#endif    
    __SM_STATE_UNLOCK(curr_state_exist);

    __SM_UNLOCK(sm_struct);

    
    return CRTL_SUCCESS;
}


int crtl_sm_exec(crtl_sm_t sm, crtl_sm_event_t event)
{
    crtl_assert(sm);
    struct crtl_sm_struct *sm_struct = (struct crtl_sm_struct *)sm;
    __crtl_dbg("%d.\n", sm_struct->ref_init.val);
    
    /* 状态是否存在 */
    __SM_RDLOCK(sm_struct);
    struct crtl_sm_state_node *curr_state_exist = __search_crtl_sm_state_node(sm_struct, sm_struct->current_state);
    if(unlikely(!curr_state_exist)) {
        crtl_print_err("Not exist state node: %ld.\n", sm_struct->current_state);
        __SM_UNLOCK(sm_struct);
        return CRTL_ERROR;
    }
    
    __SM_STATE_RDLOCK(curr_state_exist);
    struct __crtl_sm_evthandler *exist_event_handler = __search_crtl_sm_event_node(curr_state_exist, event);
    if(unlikely(!exist_event_handler)) {
        crtl_print_err("Not exist event(%ld) in state(%ld) node: %ld.\n", event, curr_state_exist->curr_state);
        __SM_UNLOCK(sm_struct);
        __SM_STATE_UNLOCK(curr_state_exist);
        return CRTL_ERROR;
    }
    __SM_EVENT_RDLOCK(exist_event_handler);
    
    int ret = exist_event_handler->event_handler(sm_struct->current_state,
                                                 event,
                                                 exist_event_handler->next_state,
                                                 exist_event_handler->user_argc,
                                                 exist_event_handler->user_argv);

    sm_struct->current_state = exist_event_handler->next_state;
    
    __SM_EVENT_UNLOCK(exist_event_handler);
    __SM_STATE_UNLOCK(curr_state_exist);
    __SM_UNLOCK(sm_struct);
    
    return ret;
}


int crtl_sm_destroy(crtl_sm_t sm)
{
    crtl_assert(sm);
    struct crtl_sm_struct *sm_struct = (struct crtl_sm_struct *)sm;
    __crtl_dbg("%d.\n", sm_struct->ref_init.val);

    __SM_WRLOCK(sm_struct);

    struct rb_node *state_node;
    for(state_node=rb_first(&sm_struct->state_tree); state_node; state_node=rb_next(state_node))
    {
        struct crtl_sm_state_node *state = rb_entry(state_node, struct crtl_sm_state_node, node_of_state);
        __SM_STATE_WRLOCK(state);
        
        struct rb_node *event_node;
        for(event_node=rb_first(&state->tree_of_handlers); event_node; event_node=rb_next(event_node))
        {
            struct __crtl_sm_evthandler *event = rb_entry(event_node, struct __crtl_sm_evthandler, node_of_handler);
            __SM_EVENT_WRLOCK(event);
            crtl_rwlock_destroy(&event->event_rwlock);
            __SM_EVENT_UNLOCK(event);
            __crtl_dbg("(%ld, %ld)\n", state->curr_state, event->wait_event);
            free(event);
        }
        crtl_rwlock_destroy(&state->state_rwlock);
        __SM_STATE_UNLOCK(state);
        free(state);
    }
    crtl_rwlock_destroy(&sm_struct->sm_rwlock);
    __SM_UNLOCK(sm_struct);
    
    free(sm_struct);

    return CRTL_SUCCESS;
}

