#include "crtl/bits/crtl_lock_barrier.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"
#include "crtl/bits/crtl_types_basic.h"



int crtl_barrier_init(crtl_lock_barrier_t *barrier, const crtl_lock_barrierattr_t *attr, unsigned int __count)
{
    if(0 != pthread_barrier_init(barrier, attr, __count)) {
        crtl_print_err("pthread_barrier_init error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_barrier_destroy(crtl_lock_barrier_t *barrier)
{
    if(0 != pthread_barrier_destroy(barrier)) {
        crtl_print_err("pthread_barrier_destroy error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}
int crtl_barrier_wait(crtl_lock_barrier_t *barrier)
{
    if(0 != pthread_barrier_wait(barrier)) {
        crtl_print_err("pthread_barrier_wait error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}




int crtl_barrierattr_init(crtl_lock_barrierattr_t *__attr)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_barrierattr_init(__attr)) {
        crtl_print_err("pthread_barrierattr_init error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#else
    crtl_print_err("pthread_barrierattr_init not support. \n");
#endif 
    return CRTL_SUCCESS;
}


int crtl_barrierattr_destroy(crtl_lock_barrierattr_t *__attr)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_barrierattr_destroy(__attr)) {
        crtl_print_err("pthread_barrierattr_destroy error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#else
    crtl_print_err("pthread_barrierattr_destroy not support. \n");
#endif 
    return CRTL_SUCCESS;
}


int crtl_barrierattr_getpshared(const crtl_lock_barrierattr_t *__attr, int *__pshared)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_barrierattr_getpshared(__attr, __pshared)) {
        crtl_print_err("pthread_barrierattr_getpshared error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#else
    crtl_print_err("pthread_barrierattr_getpshared not support. \n");
#endif 
    return CRTL_SUCCESS;
}




int crtl_barrierattr_setpshared(crtl_lock_barrierattr_t *__attr, int __pshared)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_barrierattr_setpshared(__attr, __pshared)) {
        crtl_print_err("pthread_barrierattr_setpshared error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#else
    crtl_print_err("pthread_barrierattr_setpshared not support. \n");
#endif 
    return CRTL_SUCCESS;
}



int crtl_barrierattr_setpshared_shared(crtl_lock_barrierattr_t *__attr)
{
    return crtl_barrierattr_setpshared(__attr, PTHREAD_PROCESS_SHARED);
}

int crtl_barrierattr_setpshared_private(crtl_lock_barrierattr_t *__attr)
{
    return crtl_barrierattr_setpshared(__attr, PTHREAD_PROCESS_PRIVATE);
}

