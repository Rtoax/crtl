
#include "crtl/bits/crtl_lock_mutex.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"
#include "crtl/crtl_types.h"




int crtl_mutex_init(crtl_lock_mutex_t *mutex, const crtl_lock_mutexattr_t *attr)
{
    if(0 != pthread_mutex_init(mutex, attr)) {
        crtl_print_err("pthread_mutex_init error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_mutex_destroy(crtl_lock_mutex_t *mutex)
{
    if(0 != crtl_mutex_destroy(mutex)) {
        crtl_print_err("crtl_mutex_destroy error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_mutex_lock(crtl_lock_mutex_t *__mutex, int trywait, int timedwait, int seconds, long nanoseconds)
{
    if(trywait) {
        if(0 != pthread_mutex_trylock(__mutex)) {
            crtl_print_err("pthread_mutex_trylock error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
#ifdef __USE_XOPEN2K
    } else if (timedwait) {
        //struct timespec {
        //    time_t tv_sec;      /* Seconds */
        //    long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
        //};
        struct timespec timespec = {seconds, nanoseconds};
        if(0 != pthread_mutex_timedlock(__mutex, &timespec)) {
            crtl_print_err("pthread_mutex_timedlock error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
#endif //  __USE_XOPEN2K      
    } else {
        if(0 != pthread_mutex_lock(__mutex)) {
            crtl_print_err("pthread_mutex_lock error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
    }
    return CRTL_SUCCESS;
}



int crtl_mutex_unlock(crtl_lock_mutex_t *__mutex)
{
    if(0 != pthread_mutex_unlock(__mutex)) {
        crtl_print_err("pthread_mutex_unlock error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_mutexattr_init(crtl_lock_mutexattr_t *__attr)
{
    if(0 != pthread_mutexattr_init(__attr)) {
        crtl_print_err("pthread_mutexattr_init error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_mutexattr_destroy(crtl_lock_mutexattr_t *__attr)
{
    if(0 != pthread_mutexattr_destroy(__attr)) {
        crtl_print_err("pthread_mutexattr_destroy error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}



int crtl_mutexattr_getpshared(const crtl_lock_mutexattr_t *__attr, int *__pshared)
{
    if(0 != pthread_mutexattr_getpshared(__attr, __pshared)) {
        crtl_print_err("pthread_mutexattr_getpshared error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_mutexattr_setpshared_shared(crtl_lock_mutexattr_t *__attr)
{
    return crtl_mutexattr_setpshared(__attr, PTHREAD_PROCESS_SHARED);
}

int crtl_mutexattr_setpshared_private(crtl_lock_mutexattr_t *__attr)
{
    return crtl_mutexattr_setpshared(__attr, PTHREAD_PROCESS_PRIVATE);
}

int crtl_mutexattr_setpshared(crtl_lock_mutexattr_t *__attr, int __pshared)
{
    if(0 != pthread_mutexattr_setpshared(__attr, __pshared)) {
        crtl_print_err("pthread_mutexattr_setpshared error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_mutexattr_gettype(const crtl_lock_mutexattr_t *__attr, int *__kind)
{
#if defined __USE_UNIX98 || defined __USE_XOPEN2K8
    if(0 != pthread_mutexattr_gettype(__attr, __kind)) {
        crtl_print_err("pthread_mutexattr_gettype error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#endif
    return CRTL_SUCCESS;
}




int crtl_mutexattr_settype_normal(crtl_lock_mutexattr_t *__attr)
{
    return crtl_mutexattr_settype(__attr, PTHREAD_MUTEX_NORMAL);
}
int crtl_mutexattr_settype_errorcheck(crtl_lock_mutexattr_t *__attr)
{
    return crtl_mutexattr_settype(__attr, PTHREAD_MUTEX_ERRORCHECK);
}
int crtl_mutexattr_settype_recursive(crtl_lock_mutexattr_t *__attr)
{
    return crtl_mutexattr_settype(__attr, PTHREAD_MUTEX_RECURSIVE);
}
int crtl_mutexattr_settype_default(crtl_lock_mutexattr_t *__attr)
{
    return crtl_mutexattr_settype(__attr, PTHREAD_MUTEX_DEFAULT);
}


int crtl_mutexattr_settype(crtl_lock_mutexattr_t *__attr, int __kind)
{
#if defined __USE_UNIX98 || defined __USE_XOPEN2K8
    if(0 != pthread_mutexattr_settype(__attr, __kind)) {
        crtl_print_err("pthread_mutexattr_settype error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#endif
    return CRTL_SUCCESS;
}


int crtl_mutexattr_getprotocol(const crtl_lock_mutexattr_t *__attr, int *__protocol)
{
    if(0 != pthread_mutexattr_getprotocol(__attr, __protocol)) {
        crtl_print_err("pthread_mutexattr_getprotocol error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}




int crtl_mutexattr_setprotocol_none(crtl_lock_mutexattr_t *__attr)
{
    return crtl_mutexattr_setprotocol(__attr, PTHREAD_PRIO_NONE);
}
int crtl_mutexattr_setprotocol_inherit(crtl_lock_mutexattr_t *__attr)
{
    return crtl_mutexattr_setprotocol(__attr, PTHREAD_PRIO_INHERIT);
}
int crtl_mutexattr_setprotocol_protect(crtl_lock_mutexattr_t *__attr)
{
    return crtl_mutexattr_setprotocol(__attr, PTHREAD_PRIO_PROTECT);
}

int crtl_mutexattr_setprotocol(crtl_lock_mutexattr_t *__attr, int __protocol)
{
    if(0 != pthread_mutexattr_setprotocol(__attr, __protocol)) {
        crtl_print_err("pthread_mutexattr_setprotocol error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_mutexattr_getrobust(const crtl_lock_mutexattr_t *__attr, int *__robustness)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_mutexattr_getrobust(__attr, __robustness)) {
        crtl_print_err("pthread_mutexattr_getrobust error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;  
    }
#endif
    return CRTL_SUCCESS;
}

int crtl_mutexattr_setrobust_stalled(crtl_lock_mutexattr_t *__attr)
{
    return crtl_mutexattr_setrobust(__attr, PTHREAD_MUTEX_STALLED);
}
int crtl_mutexattr_setrobust_robust(crtl_lock_mutexattr_t *__attr)
{
    return crtl_mutexattr_setrobust(__attr, PTHREAD_MUTEX_ROBUST);
}






int crtl_mutexattr_setrobust(crtl_lock_mutexattr_t *__attr, int __robustness)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_mutexattr_setrobust(__attr, __robustness)) {
        crtl_print_err("pthread_mutexattr_setrobust error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#endif  
    return CRTL_SUCCESS;
}


