#include "crtl/bits/crtl_lock_semaphore.h"
#include "crtl/log.h"
#include "crtl/assert.h"
#include "crtl/bits/types_basic.h"


int crtl_semaphore_init (crtl_lock_sem_t *__sem, unsigned int value)
{
    if(0 != sem_init(__sem, 0, value)) {
        crtl_print_err("sem_init error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_semaphore_destroy (crtl_lock_sem_t *__sem)
{
    if(0 != sem_destroy(__sem)) {
        crtl_print_err("sem_destroy error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_semaphore_wait (crtl_lock_sem_t *__sem, int trywait, int timedwait, int seconds, long nanoseconds)
{
    if(trywait) {
        if(0 != sem_trywait(__sem)) {
            crtl_print_err("sem_trywait error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
#ifdef __USE_XOPEN2K
    } else if (timedwait) {
        //struct timespec {
        //    time_t tv_sec;      /* Seconds */
        //    long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
        //};
        struct timespec timespec = {seconds, nanoseconds};
        if(0 != sem_timedwait(__sem, &timespec)) {
            crtl_print_err("sem_timedwait error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
#endif //  __USE_XOPEN2K      
    } else {
        if(0 != sem_wait(__sem)) {
            crtl_print_err("sem_wait error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
    }
    return CRTL_SUCCESS;
}


int crtl_semaphore_post (crtl_lock_sem_t *__sem)
{
    if(0 != sem_post(__sem)) {
        crtl_print_err("sem_post error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_semaphore_getvalue (crtl_lock_sem_t * __sem)
{
    int __sval = 0;
    if(0 != sem_getvalue(__sem, &__sval)) {
        crtl_print_err("sem_post error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return __sval;
}

crtl_lock_sem_t *crtl_semaphore_fopen(const char *__name, unsigned int value)
{
    crtl_lock_sem_t *__sem = NULL;
    __sem = sem_open(__name, O_CREAT);
    if(SEM_FAILED == __sem) {
        crtl_print_err("sem_open error. %s\n", CRTL_SYS_ERROR);
        return NULL;
    }
    return __sem;
}

int crtl_semaphore_fclose(crtl_lock_sem_t *__sem)
{
    if(0 != sem_close(__sem)) {
        crtl_print_err("sem_close error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_semaphore_funlink(const char *__name)
{
    if(0 != sem_unlink(__name)) {
        crtl_print_err("sem_unlink error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}





