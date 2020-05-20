
#include "crtl/bits/cond.h"
#include "crtl/log.h"
#include "crtl/assert.h"
#include "crtl/bits/types_basic.h"



int crtl_cond_init(crtl_lock_cond_t *cond, const crtl_lock_condattr_t *attr)
{
    if(0 != pthread_cond_init(cond, attr)) {
        crtl_print_err("pthread_cond_init error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_cond_destroy(crtl_lock_cond_t *cond)
{
    if(0 != pthread_cond_destroy(cond)) {
        crtl_print_err("pthread_cond_destroy error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_cond_signal(crtl_lock_cond_t *cond)
{
    if(0 != pthread_cond_signal(cond)) {
        crtl_print_err("pthread_cond_signal error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_cond_broadcast(crtl_lock_cond_t *cond)
{
    if(0 != pthread_cond_broadcast(cond)) {
        crtl_print_err("pthread_cond_broadcast error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_cond_wait(crtl_lock_cond_t *__cond, crtl_lock_mutex_t *__mutex, int timedwait, int seconds, long nanoseconds)
{
    int ret = -1;
    if (timedwait) {
        //struct timespec {
        //    time_t tv_sec;      /* Seconds */
        //    long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
        //};
        struct timespec timespec = {seconds, nanoseconds};
        if(0 != (ret = pthread_cond_timedwait(__cond, __mutex, &timespec))) {
            crtl_print_err("pthread_cond_timedwait error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
    } else {
        if(0 != pthread_cond_wait(__cond, __mutex)) {
            crtl_print_err("pthread_cond_wait error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
    }
    return CRTL_SUCCESS;
}



int crtl_condattr_init(crtl_lock_condattr_t *__attr)
{
    if(0 != pthread_condattr_init(__attr)) {
        crtl_print_err("pthread_condattr_init error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_condattr_destroy(crtl_lock_condattr_t *__attr)
{
    if(0 != pthread_condattr_destroy(__attr)) {
        crtl_print_err("pthread_condattr_destroy error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_condattr_getpshared(const crtl_lock_condattr_t *__attr, int *__pshared)
{
    if(0 != pthread_condattr_getpshared(__attr, __pshared)) {
        crtl_print_err("pthread_condattr_getpshared error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}



int crtl_condattr_setpshared(crtl_lock_condattr_t *__attr, int __pshared)
{
    if(0 != pthread_condattr_setpshared(__attr, __pshared)) {
        crtl_print_err("pthread_condattr_setpshared error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}



int crtl_condattr_setpshared_shared(crtl_lock_condattr_t *__attr)
{
    return crtl_condattr_setpshared(__attr, PTHREAD_PROCESS_SHARED);
}

int crtl_condattr_setpshared_private(crtl_lock_condattr_t *__attr)
{
    return crtl_condattr_setpshared(__attr, PTHREAD_PROCESS_PRIVATE);
}

int crtl_condattr_getclock(const crtl_lock_condattr_t *__attr, int *__clock_id)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_condattr_getclock(__attr, __clock_id)) {
        crtl_print_err("pthread_condattr_getclock error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#endif    
    return CRTL_SUCCESS;
}



int crtl_condattr_setclock(crtl_lock_condattr_t *__attr, int __clock_id)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_condattr_setclock(__attr, __clock_id)) {
        crtl_print_err("pthread_condattr_setclock error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#endif    
    return CRTL_SUCCESS;
}

int crtl_condattr_setclock_realtime(crtl_lock_condattr_t *__attr, int __clock_id)
{
    return crtl_condattr_setclock(__attr, CLOCK_REALTIME);
}
int crtl_condattr_setclock_monotonic(crtl_lock_condattr_t *__attr, int __clock_id)
{
    return crtl_condattr_setclock(__attr, CLOCK_MONOTONIC);
}
int crtl_condattr_setclock_process_cputime_id(crtl_lock_condattr_t *__attr, int __clock_id)
{
    return crtl_condattr_setclock(__attr, CLOCK_PROCESS_CPUTIME_ID);
}
int crtl_condattr_setclock_thread_cputime_id(crtl_lock_condattr_t *__attr, int __clock_id)
{
    return crtl_condattr_setclock(__attr, CLOCK_THREAD_CPUTIME_ID);
}


