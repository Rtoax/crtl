#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>

#include "crtl/crtl_alloc.h"
#include "crtl/crtl_time.h"
#include "crtl/crtl_timer.h"
#include "crtl/crtl_types.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"
#include "crtl/crtl_task.h"
#include "crtl/crtl_tree.h"
#include "crtl/bits/crtl_lock_rwlock.h"

#include "crtl/easy/byteswap.h"


#include "crtl_mute_dbg.h"


/* rb tree of all timers you create */
static crtl_rbtree_t _unused __rt_rbtree_static_timerfds = NULL;
static crtl_lock_mutex_t _unused __rt_rbtree_static_timerfds_lock = CRTL_LOCK_MUTEX_INITIALIZER;
static fd_set _unused __rt_timerfds_fdset;
static int _unused __rt_timerfd_maxfd = 0;
static int _unused __rt_timerfds[FD_SETSIZE] = {0};


/* lock of rb tree of all timers */
#define __RBTREE_TIMERFD_LOCK      do{crtl_mutex_lock(&__rt_rbtree_static_timerfds_lock, 0,0,0,0);}while(0);
#define __RBTREE_TIMERFD_UNLOCK    do{crtl_mutex_unlock(&__rt_rbtree_static_timerfds_lock);}while(0);

/* task loop */
static volatile long _unused __crtl_timerfds_list_poll_task_init_flag = 0; /* 已初始化=1 */
static crtl_thread_t _unused __rt_timerfd_update_and_execute;


/* cmp fn for rbtree timers */
static int _unused __rt_rbtree_cmp_fn_timerfd(const void *data1, const void *data2)
{
    if(!data1 || !data2)
    {
        crtl_print_err("null pointer error.\n");
        return CRTL_ERROR;
    }
    
    const struct crtl_timer_struct *t1 = data1, *t2 = data2;

    if(t1->timer_fd > t2->timer_fd)
        return CRTL_GT;
    else if(t1->timer_fd == t2->timer_fd)
        return CRTL_EQ;
    else if(t1->timer_fd < t2->timer_fd)
        return CRTL_LT;
    
    return CRTL_EQ;
}
static void __sche_sighandler(int signum)
{
    crtl_thread_exit(NULL);
    __crtl_dbg("Terminate Timer Schedule thread.\n");
}
_hidden int __crtl_timerfds_destroy();

/* update rbtree of timer */
static void* __rt_timerfd_polling_thread(void *arg)
{
    sleep(1); //waiting for first timer

    signal(SIGINT, __sche_sighandler);
    
    fd_set readset;

    int _unused nready;
    uint64_t exp;
    
    __crtl_dbg("polling timerfds __rt_timerfd_update_and_execute = %ld.\n", __rt_timerfd_update_and_execute);
    
    /* rbtree timer loop */
    while(1)
    {
        readset = __rt_timerfds_fdset;

        nready = select(__rt_timerfd_maxfd+1, &readset, NULL, NULL, NULL);
//        __crtl_dbg("polling timerfds nready = %d.\n", nready);
        
        __RBTREE_TIMERFD_LOCK;
        
        struct crtl_timer_struct *_t = NULL;
        crtl_rbtree_node_t *find_node = NULL;
        for(find_node=crtl_rbtree_getfirst(__rt_rbtree_static_timerfds); 
            find_node; find_node = crtl_rbtree_getnext(find_node))
        {
            _t = find_node->data;
            
            if(FD_ISSET(_t->timer_fd, &readset))
            {
//                __crtl_dbg("polling timerfd %d.\n", _t->timer_fd);
                
                int _unused n = read(_t->timer_fd, &exp, sizeof(exp));
                
//                __crtl_dbg("fd = %d, exp = %ld\n", _t->timer_fd, exp);
                _t->timer_sche_callback(_t->timer_sche_arg);
                
                switch(_t->timer_loop)
                {
                    /* loop timer */ 
                    case CRTL_TIMER_LOOP:
                        break;

                    /* no loop timer */    
                    case CRTL_TIMER_NONLOOP:
                        
                        crtl_rbtree_delete(__rt_rbtree_static_timerfds, _t);
                        
                        FD_CLR(_t->timer_fd, &__rt_timerfds_fdset);

                        close(_t->timer_fd);
                        
                        _t = NULL;
                        
                        break;

                    /* default */    
                    default:
                        break;
                }
            }
            
        }
        __crtl_dbg("Ntimer %d.\n", crtl_rbtree_nnode(__rt_rbtree_static_timerfds));
            
        if(crtl_rbtree_is_empty(__rt_rbtree_static_timerfds) == CRTL_SUCCESS) {
            __crtl_dbg("Ready to Terminate Timer Schedule thread.\n");
            /* 退出线程 */
            __crtl_timerfds_destroy();
        }
        __RBTREE_TIMERFD_UNLOCK;
    }

    /* 退出线程 */
    __crtl_dbg("Terminate Timer Schedule thread.\n");
    crtl_thread_exit(NULL);
    
    return NULL;
}

static struct crtl_timer_struct * _unused __find_timer_by_timerfd(int timerfd)
{
    struct crtl_timer_struct timer;
    timer.timer_fd = timerfd;
    crtl_rbtree_node_t *find_node = crtl_rbtree_search(__rt_rbtree_static_timerfds, &timer);
    if(!find_node) {
        return NULL;
    } else {
        return find_node->data;
    }
    return NULL;
}

/* create a timer */
_api int crtl_timerfd_create(int is_loop, __crtl_timer_cb_fn_t callback, void *arg, int sec, int nsec)
{
    //crtl_print_err("timer->%s\n", name);
    __RBTREE_TIMERFD_LOCK;
    if(!__rt_rbtree_static_timerfds)
    {
        __rt_rbtree_static_timerfds = crtl_rbtree_init(&__rt_rbtree_cmp_fn_timerfd, NULL);
        if(!__rt_rbtree_static_timerfds) 
        {
            __RBTREE_TIMERFD_UNLOCK;
            crtl_print_err("null pointer error.\n");
            crtl_assert(0);
            goto err;
        }
        FD_ZERO(&__rt_timerfds_fdset);
        
    }
    __RBTREE_TIMERFD_UNLOCK;

    __RBTREE_TIMERFD_LOCK;
    if(CAS(&__crtl_timerfds_list_poll_task_init_flag, 0, 1)) {
        __crtl_dbg("__crtl_timerfds_list_poll_task_init_flag = %ld.\n", __crtl_timerfds_list_poll_task_init_flag);
        
        /* 创建线程 */
        int ret = crtl_thread_create(&__rt_timerfd_update_and_execute, PTHREAD_CREATE_DETACHED, 0, 
                                      CRTL_THREAD_SCHED_PRIO_HIGHEST, SCHED_RR, PTHREAD_SCOPE_SYSTEM,
                                      NULL, 0, 0, __rt_timerfd_polling_thread, arg, NULL, NULL);
        if(ret != CRTL_SUCCESS) {/* 初始化失败 */
            crtl_print_err("crtl_thread_create error.\n");
            crtl_assert_fp(stderr, 0);
            __RBTREE_TIMERFD_UNLOCK;
            return -1;
        }
    }
    //crtl_print_err("timer->%s\n", name);
    __RBTREE_TIMERFD_UNLOCK;
    
    
    if(!callback)
    {
        crtl_print_err("null pointer error.\n");
        goto err;
    }

    struct crtl_timer_struct *new_timer = (struct crtl_timer_struct*)malloc(sizeof(struct crtl_timer_struct));

    new_timer->timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC); //TFD_CLOEXEC, TFD_NONBLOCK

//    __crtl_dbg("timerfd_create timerfd = %d.\n", new_timer->timer_fd);
    
    new_timer->timer_sche_callback = callback;
    new_timer->timer_sche_arg = arg;

    if(is_loop) {
        /* loop */
        new_timer->timer_loop    = CRTL_TIMER_LOOP;
        new_timer->itimerspec_value.it_interval.tv_sec = sec;
        new_timer->itimerspec_value.it_interval.tv_nsec = nsec;
    } else {
        new_timer->timer_loop    = CRTL_TIMER_NONLOOP;
        new_timer->itimerspec_value.it_interval.tv_sec = 0;
        new_timer->itimerspec_value.it_interval.tv_nsec = 0;
    }   
    

    /* interval time */   
    new_timer->itimerspec_value.it_value.tv_sec = sec;
    new_timer->itimerspec_value.it_value.tv_nsec = nsec;
    
    
    timerfd_settime(new_timer->timer_fd, 0, &new_timer->itimerspec_value, NULL);

    __RBTREE_TIMERFD_LOCK;
        
    if(__rt_rbtree_static_timerfds) {
//        __crtl_dbg("Insert fd %d to tree.\n", new_timer->timer_fd);
        crtl_rbtree_insert(__rt_rbtree_static_timerfds, new_timer, sizeof(struct crtl_timer_struct));
    }
    
    if(FD_ISSET(new_timer->timer_fd, &__rt_timerfds_fdset))
        crtl_print_warning("%d already set in timerfds.\n", new_timer->timer_fd);
    else
    {
        FD_SET(new_timer->timer_fd, &__rt_timerfds_fdset);
        
        int i;
        for(i=0; i<FD_SETSIZE; i++)
            if(__rt_timerfds[i] == 0)
                __rt_timerfds[i] = new_timer->timer_fd;
            
        //log_notice("timerFD = %d\n", new_timer->timerFD);
    }
    
    if(__rt_timerfd_maxfd < new_timer->timer_fd)
        __rt_timerfd_maxfd = new_timer->timer_fd;
    
    __RBTREE_TIMERFD_UNLOCK;
    
    return new_timer->timer_fd;
    
err:
    crtl_print_err("create timer error.\n");
    return -1;
}

/* control timer: alter, control */
_api int crtl_timerfd_ctrl(int timerfd, int request/*RT_TIMER_CTRL_XXX*/, ...)
{
    __RBTREE_TIMERFD_LOCK;
    struct crtl_timer_struct *timer = __find_timer_by_timerfd(timerfd);
    if(!timer) {
        crtl_print_err("null pointer error. timerfd = %d\n", timerfd);
        __RBTREE_TIMERFD_UNLOCK;
        return CRTL_ERROR;
    }
    
    va_list va;
    va_start(va, request);

    int _unused _int = 0;
    int _unused *_pint = NULL;
    long int _unused      _longint = 0;
    long int _unused    *_plongint = NULL;
    char     _unused         *_str = NULL;
    void     _unused          *_fn = NULL;

    struct itimerspec _unused *_pitimerspec = NULL;
    struct itimerspec _unused _itimerspec   = {{0,0}, {0,0}};
    
    switch(request)
    {
        case TIMER_GET_FD:
            _plongint = va_arg(va, long int *);
            *_plongint = timer->timer_fd;
            break;
        case TIMER_GET_NAME:
            
            break;
        case TIMER_GET_FN:
            //_fn = va_arg(va, void*);
            //_fn = (void*)timer->callback.fn;
            crtl_print_err("not support code: TIMER_GET_FN\n");
            crtl_assert(0);
            __RBTREE_TIMERFD_UNLOCK;
            return CRTL_NOTSUPPORT;
            break;
        case TIMER_GET_ARG:
            crtl_print_err("not support code: TIMER_GET_ARG\n");
            crtl_assert(0);
            __RBTREE_TIMERFD_UNLOCK;
            return CRTL_NOTSUPPORT;
            break;
        case TIMER_GET_LOOPTYPE:
            _pint = va_arg(va, int *);
            *_pint = timer->timer_loop;
            break;
        case TIMER_GET_NLOOP:
            crtl_print_err("not support code: TIMER_GET_NLOOP\n");
            crtl_assert(0);
            __RBTREE_TIMERFD_UNLOCK;
            return CRTL_NOTSUPPORT;
            break;
        case TIMER_GET_INTERVAL:
            _pitimerspec = va_arg(va, struct itimerspec*);
            _pitimerspec->it_value.tv_sec       = timer->itimerspec_value.it_value.tv_sec;
            _pitimerspec->it_value.tv_nsec      = timer->itimerspec_value.it_value.tv_nsec;
            _pitimerspec->it_interval.tv_sec    = timer->itimerspec_value.it_interval.tv_sec;
            _pitimerspec->it_interval.tv_nsec   = timer->itimerspec_value.it_interval.tv_nsec;
            break;
        case TIMER_SET_FD:
            crtl_print_err("not support code: TIMER_SET_FD\n");
            crtl_assert(0);
            __RBTREE_TIMERFD_UNLOCK;
            return CRTL_NOTSUPPORT;
            break;
        case TIMER_SET_NAME:
            crtl_print_err("not support code: TIMER_SET_NAME\n");
            crtl_assert(0);
            __RBTREE_TIMERFD_UNLOCK;
            return CRTL_NOTSUPPORT;
            break;
        case TIMER_SET_FN:
            crtl_print_err("not support code: TIMER_GET_FN\n");
            crtl_assert(0);
            __RBTREE_TIMERFD_UNLOCK;
            return CRTL_NOTSUPPORT;
            break;
        case TIMER_SET_ARG:
            crtl_print_err("not support code: TIMER_GET_FN\n");
            crtl_assert(0);
            __RBTREE_TIMERFD_UNLOCK;
            return CRTL_NOTSUPPORT;
            break;
        case TIMER_SET_LOOPTYPE:
            _longint = va_arg(va, long int);
            timer->timer_loop = _longint;
            break;
        case TIMER_SET_NLOOP:
            crtl_print_err("not support code: TIMER_GET_FN\n");
            crtl_assert(0);
            __RBTREE_TIMERFD_UNLOCK;
            return CRTL_NOTSUPPORT;
            break;
        case TIMER_SET_INTERVAL:
            _itimerspec = va_arg(va, struct itimerspec);
            timer->itimerspec_value.it_value.tv_sec       = _itimerspec.it_value.tv_sec;
            timer->itimerspec_value.it_value.tv_nsec      = _itimerspec.it_value.tv_nsec;
            timer->itimerspec_value.it_interval.tv_sec    = _itimerspec.it_interval.tv_sec;
            timer->itimerspec_value.it_interval.tv_nsec   = _itimerspec.it_interval.tv_nsec;
            
            timerfd_settime(timer->timer_fd, 0, &timer->itimerspec_value, &_itimerspec);
            
            break;
    }

    va_end(va);
    __RBTREE_TIMERFD_UNLOCK;
    return CRTL_SUCCESS;
    
}


/* delete timer from timer rbtree */
_api int crtl_timerfd_delete(int timerfd)
{
    __RBTREE_TIMERFD_LOCK;
    struct crtl_timer_struct *_user_timer = __find_timer_by_timerfd(timerfd);
    if(!_user_timer)
    {
        crtl_print_warning("null pointer error.\n");
        __RBTREE_TIMERFD_UNLOCK;
        return CRTL_ERROR;
    }

    crtl_rbtree_node_t *find_node = crtl_rbtree_search(__rt_rbtree_static_timerfds, _user_timer);
    if(!find_node)
    {
        crtl_print_warning("not find timer \n");
        __RBTREE_TIMERFD_UNLOCK;
        return CRTL_ERROR;
    }

    struct crtl_timer_struct *__timer = (struct crtl_timer_struct*)(find_node->data);

    FD_CLR(__timer->timer_fd, &__rt_timerfds_fdset);
    
    crtl_rbtree_delete(__rt_rbtree_static_timerfds, __timer);
    
    __crtl_dbg("Ntimer %d.\n", crtl_rbtree_nnode(__rt_rbtree_static_timerfds));
    if(crtl_rbtree_is_empty(__rt_rbtree_static_timerfds) == CRTL_SUCCESS) {
        __crtl_dbg("Ready to Terminate Timer Schedule thread.\n");
        /* 退出线程 */
        __crtl_timerfds_destroy();
    }
    __RBTREE_TIMERFD_UNLOCK;

    close(__timer->timer_fd);

    free(_user_timer);
    _user_timer = NULL;
    
    return CRTL_SUCCESS;
    
}

_hidden int __crtl_timerfds_destroy() 
{
    /* 退出线程 */
    __crtl_dbg("Terminate Timer Schedule thread.\n");
    crtl_thread_kill(__rt_timerfd_update_and_execute, SIGINT);
    CAS(&__crtl_timerfds_list_poll_task_init_flag, 1, 0);
    
    int ret = crtl_rbtree_destroy(__rt_rbtree_static_timerfds);
    __rt_rbtree_static_timerfds = NULL;
    FD_ZERO(&__rt_timerfds_fdset);
    return ret;
}

/* destroy all timers */
_api int crtl_timerfds_destroy()
{
    __RBTREE_TIMERFD_LOCK;
    
    int ret = __crtl_timerfds_destroy();
    __RBTREE_TIMERFD_UNLOCK;
    
    return ret;
}





