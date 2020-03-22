#include "crtl/crtl_alloc.h"
#include "crtl/crtl_time.h"
#include "crtl/crtl_timer.h"
#include "crtl/crtl_types.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"

#include "crtl/bits/crtl_lock_rwlock.h"

/* 定时器列表的读写锁 */
static crtl_lock_rw_t _unused __crtl_timers_list_rwlock = CRTL_LOCK_RWLOCK_INITIALIZER;
#define __crtl_timers_list_rdlock crtl_rwlock_rdlock(&__crtl_timers_list_rwlock, 0,0,0,0)
#define __crtl_timers_list_wrlock crtl_rwlock_wrlock(&__crtl_timers_list_rwlock, 0,0,0,0)
#define __crtl_timers_list_unlock crtl_rwlock_unlock(&__crtl_timers_list_rwlock)

static LIST_HEAD_UNUSED(__crtl_timers_list_head);


/* 定时器列表是否初始化标识位 */
static volatile long _unused __crtl_timers_list_init_flag = 1; /* 已初始化=1 */


long crtl_timer_create(crtl_boolean is_loop, void (*callback)(void *arg), void *arg, long sec, long nanosec)
{
    /* 入参有误 */
    if(unlikely(!callback) || (unlikely(!sec) && unlikely(!nanosec))) {
        crtl_print_err("wrong params error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }

    long timer_id = CRTL_ERROR;

    struct crtl_timer_struct *__timer = crtl_malloc1(1, sizeof(struct crtl_timer_struct));
    if(unlikely(!__timer)) {
        /* 申请失败，退出 */
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    struct timeval tv;
    gettimeofday(&tv,NULL);
    struct timespec timespec = {0,0};
	timespec.tv_sec = tv.tv_sec + sec;
	timespec.tv_nsec = 1000 * (tv.tv_usec) + nanosec;

    
    
    timer_id = (long) __timer;
    return timer_id;
}


