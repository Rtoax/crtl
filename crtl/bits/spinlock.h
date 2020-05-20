#ifndef __CRTL_BITS_LOCK_SPIN_H
#define __CRTL_BITS_LOCK_SPIN_H 1


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <pthread.h>


typedef pthread_spinlock_t  crtl_lock_spin_t;


#define __CRTL_SPIN_LOCK_UNLOCKED(lock) \
    crtl_spin_init_shared(&lock)


int crtl_spin_init(crtl_lock_spin_t *__lock, int __pshared);
int crtl_spin_init_shared(crtl_lock_spin_t *__lock);
int crtl_spin_init_private(crtl_lock_spin_t *__lock);

int crtl_spin_destroy(crtl_lock_spin_t *__lock);

int crtl_spin_lock(crtl_lock_spin_t *__lock, int trylock);

int crtl_spin_unlock(crtl_lock_spin_t *__lock);





#endif //__CRTL_BITS_LOCK_SPIN_H

