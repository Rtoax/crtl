#include "crtl/crtl_alloc.h"
#include "crtl/crtl_time.h"
#include "crtl/crtl_timer.h"
#include "crtl/crtl_types.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"
#include "crtl/crtl_task.h"

#include "crtl/bits/crtl_lock_rwlock.h"

#include "crtl/easy/byteswap.h"

/* 定时器列表的读写锁 */
static crtl_lock_rw_t _unused __crtl_timers_list_rwlock = CRTL_LOCK_RWLOCK_INITIALIZER;
#define __crtl_timers_list_rdlock crtl_rwlock_rdlock(&__crtl_timers_list_rwlock, 0,0,0,0)
#define __crtl_timers_list_wrlock crtl_rwlock_wrlock(&__crtl_timers_list_rwlock, 0,0,0,0)
#define __crtl_timers_list_unlock crtl_rwlock_unlock(&__crtl_timers_list_rwlock)

/* 定时器链表的读写锁 */
static LIST_HEAD_UNUSED(__crtl_timers_list_head);


/* 定时器列表轮询线程初始化标识位 */
static volatile long _unused __crtl_timers_list_poll_task_init_flag = 0; /* 已初始化=1 */
static crtl_thread_t _unused __crtl_timer_schedule_threadID;


/* 查找一个定时器-线程不安全 */
inline static struct crtl_timer_struct * __crtl_timer_getbyid(__crtl_timer_id_t timeid)
{
    /* 入参有误 */
    if(unlikely((__crtl_timer_id_t)0==(__crtl_timer_id_t)timeid)) {
        crtl_print_err("wrong params error. timeid=%ld\n", timeid);
        crtl_assert_fp(stderr, 0);
        return NULL;
    }

    struct crtl_timer_struct *__list_ite_timer = NULL;
    struct crtl_timer_struct *_unused __list_ite_next_timer = NULL;
    
    /* 轮询定时器链表 */
    list_for_each_entry_safe(__list_ite_timer,__list_ite_next_timer, &__crtl_timers_list_head, list) {
//        __crtl_dbg(">>> %lx, %lx\n", __list_ite_timer->timer_id, timeid);
        if( __list_ite_timer->timer_id == timeid ) {
            return __list_ite_timer;
        }
        if(list_is_last(&__list_ite_timer->list, &__crtl_timers_list_head)) {
//            __crtl_dbg("Last NOde no OK.\n");
            return NULL;
        } 
        if( __list_ite_next_timer->timer_id == timeid ) {
            return __list_ite_next_timer;
        } else {
            __list_ite_timer = __list_ite_next_timer;
        }
    }
    __crtl_dbg("Not exist.\n");
    return NULL;
}


/* 添加定时器到定时器链表 */
inline static int _unused __crtl_timer_add_to_list(struct crtl_timer_struct *__insert_timer)
{
    struct crtl_timer_struct *__list_ite_timer = NULL;
    struct crtl_timer_struct *_unused __list_ite_next_timer = NULL;
    struct crtl_timer_struct *_unused __small_timer = NULL;
    struct crtl_timer_struct *_unused __big_timer = NULL;

    struct timespec time_interval = {0, 0};
    
    /**
     *  1.Find the location of insert timer
     *
     *      __small_timer   __big_timer
     *
     *  2.Insert new timer
     *      __small_timer  __insert_timer  __big_timer
     */
    
    /* 如果为空,直接添加 */
    if(list_empty(&__crtl_timers_list_head)) {
        list_add_tail(&__insert_timer->list, &__crtl_timers_list_head);

    /* 如果不为空,查找位置 */
    } else {
        /* 轮询定时器链表 */
        list_for_each_entry_safe(__list_ite_timer,__list_ite_next_timer, &__crtl_timers_list_head, list) {
            
            /* 比较两个定时器即将到时时间 */
            int cmp = crtl_timespec_subabs(&__list_ite_timer->itimerspec_value.it_value, 
                                           &__insert_timer->itimerspec_value.it_value, 
                                           &time_interval);
            /* 轮询的定时器大于即将插入的定时器，插入到前一个节点*/
            if(cmp == CRTL_GT) {
                struct crtl_timer_struct *_unused __list_prev_timer = list_prev_entry(__list_ite_timer, list);
                list_add(&__insert_timer->list, &__list_prev_timer->list);
                break;
            } 
            /* 当 cmp == CRTL_EQ || cmp == CRTL_LT */
            /* 如果是最后一个定时器（最后到时的定时器） */
            if(list_is_last(&__list_ite_timer->list, &__crtl_timers_list_head)) {
                list_add(&__insert_timer->list, &__list_ite_timer->list);
                break;
            
            /* 如果不是最后一个定时器， 同时使用 __list_ite_next_timer */
            } else {
            
                /* 轮询的定时器等于即将插入的定时器，插入*/
                if(cmp == CRTL_EQ) {
                    list_add(&__insert_timer->list, &__list_ite_timer->list);
                    break;
                
                /* 轮询的定时器小于即将插入的定时器，比较下一个节点 */
                } else if(cmp == CRTL_LT) {
                    int cmp_next = crtl_timespec_subabs(&__list_ite_next_timer->itimerspec_value.it_value, 
                                                        &__insert_timer->itimerspec_value.it_value, 
                                                        &time_interval);
                    /* 下一个节点大于或者等于，直接插入 */
                    if(cmp_next == CRTL_GT || cmp_next == CRTL_EQ) {
                        list_add(&__insert_timer->list, &__list_ite_timer->list);
                        break;
                    /* 下一个节点小于，继续比较 */
                    } else if(cmp_next == CRTL_LT) {
                        continue;
                    }
                }
            }
    	}
    }
    
    return CRTL_SUCCESS;
}

static void _unused *__crtl_timer_schedule_task_fn(void*arg)
{
//    __crtl_dbg("Start Schdule Timer list. thread id %ld\n", crtl_thread_self());

    struct timespec currentTime = {0,0};
    struct crtl_timer_struct *__list_ite_timer = NULL;
    struct crtl_timer_struct *_unused __list_ite_next_timer = NULL;
    struct timespec time_interval = {0, 0};
    struct crtl_timer_struct *__this_timer = NULL;
        
    while(1) {
        usleep(__CRTL_TIMER_REFRESH_FREQUENCY_NanoSEC/10000);
        
        /* 获取当前时间 */
        crtl_gettimeofday_timespec(&currentTime);
                
        __crtl_timers_list_wrlock;
        
        /* 轮询定时器链表 */
        list_for_each_entry_safe(__list_ite_timer,__list_ite_next_timer, &__crtl_timers_list_head, list) {

            __this_timer = __list_ite_timer;
            if(!list_is_last(&__list_ite_timer->list, &__crtl_timers_list_head)) {
                __list_ite_timer = __list_ite_next_timer;
            }
                        
            /* 计算当前时间与定时器超时时间差 */
            crtl_timespec_subabs(&__this_timer->itimerspec_value.it_value, &currentTime,  &time_interval);

            /* 如果时间差在刷新间隔内，调度定时器 */
            if(__CRTL_TIMER_REFRESH_FREQUENCY_NanoSEC >= time_interval.tv_sec*1000000000+time_interval.tv_nsec) {
                
                /* 调度 */
                __this_timer->timer_sche_callback(__this_timer->timer_sche_arg);

                /* 从链表中删除这个定时器 */
                list_del_init(&__this_timer->list);
            
                /* 是否为循环定时器 */
                if(__this_timer->timer_loop == CRTL_TIMER_LOOP) {
                    
                    /* 获取到期时间:轮询表中将被与GetTimeofDay比较 */
                    crtl_timespec_add(&currentTime, &__this_timer->itimerspec_value.it_interval, 
                                                    &__this_timer->itimerspec_value.it_value);
                    
                    __crtl_timer_add_to_list(__this_timer);
                    
                } else {
                    crtl_mfree1(__this_timer);
                }
                
            }
        }
        /* 如果删除后定时器列表为空 */
        if(list_empty_careful(&__crtl_timers_list_head)) {
            __crtl_dbg("Timer List is empty.>>>>>>>>>>\n");
            CAS(&__crtl_timers_list_poll_task_init_flag, 1, 0);
            __crtl_timers_list_unlock;
            break;
        }
        __crtl_timers_list_unlock;

    }

    /* 退出线程 */
    __crtl_dbg("Terminate Timer Schedule thread.\n");
    crtl_thread_exit(NULL);
    
    return NULL;
}


/* 创建定时器 */
__crtl_timer_id_t crtl_timer_create(int is_loop, void (*callback)(void *arg), void *arg, long sec, long nanosec)
{
    /* 入参有误 */
    if(unlikely(!callback) || (unlikely(!sec) && unlikely(!nanosec))) {
        crtl_print_err("wrong params error.\n");
        crtl_assert_fp(stderr, 0);
        return 0;
    }
    
    /* 类型检测 */
    typecheck_fn(__crtl_timer_cb_fn_t, callback);
    
    struct crtl_timer_struct *__timer = crtl_malloc1(1, sizeof(struct crtl_timer_struct));
    if(unlikely(!__timer)) {
        /* 申请失败，退出 */
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return 0;
    }

    if(is_loop) {
        __timer->timer_loop = CRTL_TIMER_LOOP;
    } else {
        __timer->timer_loop = CRTL_TIMER_NONLOOP;
    }
#define GEN_TIMERID(paddr) ((__crtl_timer_id_t)paddr)
    
    __timer->timer_id = GEN_TIMERID(__timer);
    
    
    struct timespec currentTime = {0,0};

    /* 计算并保存间隔时间 */
    crtl_timespec_generate(&__timer->itimerspec_value.it_interval, sec, nanosec);

    /* 获取当前时间 */
    crtl_gettimeofday_timespec(&currentTime);
    
    /* 获取到期时间:轮询表中将被与GetTimeofDay比较 */
    crtl_timespec_add(&currentTime, &__timer->itimerspec_value.it_interval, &__timer->itimerspec_value.it_value);

    /* 置NULL */
    __timer->list.next = __timer->list.prev = NULL;
    __timer->timer_sche_callback = callback;
    __timer->timer_sche_arg = arg;

    
    __crtl_timers_list_wrlock;
    /* 添加到定时器链表 */
    __crtl_timer_add_to_list(__timer);
    
    /* 创建链表轮询任务 */
    if(CAS(&__crtl_timers_list_poll_task_init_flag, 0, 1)) {
        __crtl_dbg("__crtl_timers_list_poll_task_init_flag = %ld.\n", __crtl_timers_list_poll_task_init_flag);
        
        /* 创建线程 */
        int ret = crtl_thread_create(&__crtl_timer_schedule_threadID, PTHREAD_CREATE_DETACHED, 0, 
                                      CRTL_THREAD_SCHED_PRIO_HIGHEST, SCHED_RR, PTHREAD_SCOPE_SYSTEM,
                                      NULL, 0, 0, __crtl_timer_schedule_task_fn, arg, NULL, NULL);
//        int ret = crtl_thread_normal(&__crtl_timer_schedule_threadID, __crtl_timer_schedule_task_fn, arg);
        if(ret != CRTL_SUCCESS) {/* 初始化失败 */
            crtl_print_err("crtl_thread_create error.\n");
            crtl_assert_fp(stderr, 0);
            __crtl_timers_list_unlock;
            return 0;
        }
        __crtl_dbg("create timer schedule thread.\n");
    }
    __crtl_timers_list_unlock;
    
    /* 获取返回值 */
//    __crtl_dbg("Create Timer ID: %lx(%lx)(%d)\n", __timer->timer_id, GEN_TIMERID(__timer), sizeof(__crtl_timer_id_t));
//    crtl_memprint(stdout, &__timer->timer_id, 8);
    return GEN_TIMERID(__timer);
}

/* 获取定时器时间 */
_api int crtl_timer_gettime_interval(__crtl_timer_id_t timerid, long *sec, long *nanosec)
{
    /* 首先找到这个定时器 */
    struct crtl_timer_struct *__this_timer = NULL;
    __crtl_timers_list_rdlock;
    __this_timer = __crtl_timer_getbyid(timerid);

    if(unlikely(!__this_timer)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        __crtl_timers_list_unlock;
        return CRTL_ERROR;
    }

    /* 计算并保存间隔时间 */
    *sec = __this_timer->itimerspec_value.it_interval.tv_sec;
    *nanosec = __this_timer->itimerspec_value.it_interval.tv_nsec;
    __crtl_timers_list_unlock;

    return CRTL_SUCCESS;
}


/* 更新定时器时间 */
_api int crtl_timer_settime_interval(__crtl_timer_id_t timerid, long sec, long nanosec)
{
    /* 首先找到这个定时器 */
    struct crtl_timer_struct *__this_timer = NULL;
    __crtl_timers_list_wrlock;
    __this_timer = __crtl_timer_getbyid(timerid);

    if(unlikely(!__this_timer)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        __crtl_timers_list_unlock;
        return CRTL_ERROR;
    }
    /* 计算并保存间隔时间 */
    crtl_timespec_generate(&__this_timer->itimerspec_value.it_interval, sec, nanosec);
    __crtl_timers_list_unlock;

    return CRTL_SUCCESS;
}

/* 更新定时器 */
_api int crtl_timer_nonloop(__crtl_timer_id_t timerid)
{
    /* 首先找到这个定时器 */
    struct crtl_timer_struct *__this_timer = NULL;
    __crtl_timers_list_wrlock;
    __this_timer = __crtl_timer_getbyid(timerid);

    if(unlikely(!__this_timer)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        __crtl_timers_list_unlock;
        return CRTL_ERROR;
    }

    __this_timer->timer_loop = CRTL_TIMER_NONLOOP;
    
    __crtl_timers_list_unlock;

    return CRTL_SUCCESS;
}


/* 删除定时器 */
_api int crtl_timer_delete(__crtl_timer_id_t timerid)
{
    struct crtl_timer_struct *__this_timer = NULL;

    __crtl_timers_list_wrlock;
    __this_timer = __crtl_timer_getbyid(timerid);

    if(unlikely(!__this_timer)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        __crtl_timers_list_unlock;
        return CRTL_ERROR;
    }

    /* 从链表中删除这个定时器 */
    list_del_init(&__this_timer->list);
    __crtl_timers_list_unlock;
    
    crtl_mfree1(__this_timer);

    return CRTL_SUCCESS;
}
//

