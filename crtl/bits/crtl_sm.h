#ifndef __CRTL_BITS_SM_H
#define __CRTL_BITS_SM_H 1

#include "crtl/bits/crtl_atomic.h"

#include "crtl/bits/crtl_lock_rwlock.h"
#include "crtl/bits/crtl_lock_semaphore.h"
#include "crtl/bits/crtl_list.h"

#include "crtl/bits/crtl_types_fn.h"
#include "crtl/bits/crtl_types_size.h"

#include "crtl/bits/crtl_tree_rbtree.h"


typedef unsigned long crtl_sm_state_t;
typedef signed long crtl_sm_event_t;

typedef int (*crtl_sm_handler_fn_t)(const crtl_sm_state_t curr_state, \
                                    const crtl_sm_event_t event, \
                                    const crtl_sm_state_t next_state, \
                                    int argc, char *argv[]);



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

/* 状态机 类型 */
typedef unsigned long crtl_sm_t;



/* 初始化状态机 */
crtl_sm_t crtl_sm_init(crtl_sm_state_t init_state);

int crtl_sm_state_register(crtl_sm_t sm, crtl_sm_state_t curr_state);

int crtl_sm_handler_register(crtl_sm_t sm, crtl_sm_state_t curr_state, crtl_sm_event_t event, crtl_sm_state_t next_state,
                                crtl_sm_handler_fn_t handler, int argc, char *argv[]);

int crtl_sm_exec(crtl_sm_t sm, crtl_sm_event_t event);

int crtl_sm_destroy(crtl_sm_t sm);

#endif /*<__CRTL_BITS_SM_H>*/
