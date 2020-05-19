
#include "crtl/bits/crtl_lock_spin.h"
#include "crtl/log.h"
#include "crtl/assert.h"
#include "crtl/bits/types_basic.h"




int crtl_spin_init(crtl_lock_spin_t *__lock, int __pshared)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_spin_init(__lock, __pshared)) {
        crtl_print_err("pthread_spin_init error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#else
    crtl_print_err("pthread_spin_init not support. \n");
#endif   
    return CRTL_SUCCESS;
}
int crtl_spin_init_shared(crtl_lock_spin_t *__lock)
{
    return crtl_spin_init(__lock, PTHREAD_PROCESS_SHARED);
}
int crtl_spin_init_private(crtl_lock_spin_t *__lock)
{
    return crtl_spin_init(__lock, PTHREAD_PROCESS_PRIVATE);
}



int crtl_spin_destroy(crtl_lock_spin_t *__lock)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_spin_destroy(__lock)) {
        crtl_print_err("pthread_spin_destroy error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#else
    crtl_print_err("pthread_spin_destroy not support. \n");
#endif   
    return CRTL_SUCCESS;
}


int crtl_spin_lock(crtl_lock_spin_t *__lock, int trylock)
{
#ifdef __USE_XOPEN2K
    if(trylock) {
        if(0 != pthread_spin_trylock(__lock)) {
            crtl_print_err("pthread_spin_trylock error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }    
    } else {
        if(0 != pthread_spin_lock(__lock)) {
            crtl_print_err("pthread_spin_lock error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
    }
#else
    crtl_print_err("pthread_spin_lock not support. \n");
#endif   
    return CRTL_SUCCESS;
}


int crtl_spin_unlock(crtl_lock_spin_t *__lock)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_spin_unlock(__lock)) {
        crtl_print_err("pthread_spin_unlock error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#else
    crtl_print_err("pthread_spin_unlock not support. \n");
#endif   
    return CRTL_SUCCESS;
}

