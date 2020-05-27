#include "crtl/bits/rwlock.h"
#include "crtl/log.h"
#include "crtl/assert.h"
#include "crtl/bits/types_basic.h"



int crtl_rwlock_init(crtl_lock_rw_t *rwlock, const crtl_lock_rwattr_t *attr)
{
    if(0 != pthread_rwlock_init(rwlock, attr)) {
        crtl_print_err("pthread_rwlock_init error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_rwlock_destroy(crtl_lock_rw_t *rwlock)
{
    if(0 != pthread_rwlock_destroy(rwlock)) {
        crtl_print_err("pthread_rwlock_destroy error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}



int crtl_rwlock_rdlock(crtl_lock_rw_t *rwlock, int trywait, int timedwait, int seconds, long nanoseconds)
{
    if(trywait) {
        if(0 != pthread_rwlock_tryrdlock(rwlock)) {
            crtl_print_err("pthread_rwlock_tryrdlock error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
#ifdef __USE_XOPEN2K
    } else if (timedwait) {
        //struct timespec {
        //    time_t tv_sec;      /* Seconds */
        //    long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
        //};
        struct timespec timespec = {seconds, nanoseconds};
        if(0 != pthread_rwlock_timedrdlock(rwlock, &timespec)) {
            crtl_print_err("pthread_rwlock_timedrdlock error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
#endif //  __USE_XOPEN2K      
    } else {
        if(0 != pthread_rwlock_rdlock(rwlock)) {
            crtl_print_err("pthread_rwlock_rdlock error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
    }
    return CRTL_SUCCESS;
}


int crtl_rwlock_wrlock(crtl_lock_rw_t *rwlock, int trywait, int timedwait, int seconds, long nanoseconds)
{
    if(trywait) {
        if(0 != pthread_rwlock_trywrlock(rwlock)) {
            crtl_print_err("pthread_rwlock_trywrlock error. %s\n", CRTL_SYS_ERROR);
            crtl_assert_fp(stderr, 0);
            return CRTL_ERROR;
        }
#ifdef __USE_XOPEN2K
    } else if (timedwait) {
        //struct timespec {
        //    time_t tv_sec;      /* Seconds */
        //    long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
        //};
        struct timespec timespec = {seconds, nanoseconds};
        if(0 != pthread_rwlock_timedwrlock(rwlock, &timespec)) {
            crtl_print_err("pthread_rwlock_timedwrlock error. %s\n", CRTL_SYS_ERROR);
            crtl_assert_fp(stderr, 0);
            return CRTL_ERROR;
        }
#endif //  __USE_XOPEN2K      
    } else {
        if(0 != pthread_rwlock_wrlock(rwlock)) {
            crtl_print_err("pthread_rwlock_wrlock error. %s\n", CRTL_SYS_ERROR);
            crtl_assert_fp(stderr, 0);
            return CRTL_ERROR;
        }
    }
    return CRTL_SUCCESS;
}


int crtl_rwlock_unlock(crtl_lock_rw_t *rwlock)
{
    if(0 != pthread_rwlock_unlock(rwlock)) {
        crtl_print_err("pthread_rwlock_unlock error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}




int crtl_rwlockattr_init(crtl_lock_rwattr_t *__attr)
{
    if(0 != pthread_rwlockattr_init(__attr)) {
        crtl_print_err("pthread_rwlockattr_init error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_rwlockattr_destroy(crtl_lock_rwattr_t *__attr)
{
    if(0 != pthread_rwlockattr_destroy(__attr)) {
        crtl_print_err("pthread_rwlockattr_destroy error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}



int crtl_rwlockattr_getpshared(const crtl_lock_rwattr_t *__attr, int *__pshared)
{
    if(0 != pthread_rwlockattr_getpshared(__attr, __pshared)) {
        crtl_print_err("pthread_rwlockattr_getpshared error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}



int crtl_rwlockattr_setpshared(crtl_lock_rwattr_t *__attr, int __pshared)
{
    if(0 != pthread_rwlockattr_setpshared(__attr, __pshared)) {
        crtl_print_err("pthread_rwlockattr_setpshared error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}
int crtl_rwlockattr_setpshared_shared(crtl_lock_rwattr_t *__attr)
{
    return crtl_rwlockattr_setpshared(__attr, PTHREAD_PROCESS_SHARED);
}

int crtl_rwlockattr_setpshared_private(crtl_lock_rwattr_t *__attr)
{
    return crtl_rwlockattr_setpshared(__attr, PTHREAD_PROCESS_PRIVATE);
}

