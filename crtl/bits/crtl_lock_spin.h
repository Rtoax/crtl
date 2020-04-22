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



#ifndef _PTHREAD_H

#ifdef __USE_XOPEN2K
/* Functions to handle spinlocks.  */

/* Initialize the spinlock LOCK.  If PSHARED is nonzero the spinlock can
   be shared between different processes.  */
extern int pthread_spin_init(pthread_spinlock_t *__lock, int __pshared);

/* Destroy the spinlock LOCK.  */
extern int pthread_spin_destroy(pthread_spinlock_t *__lock);

/* Wait until spinlock LOCK is retrieved.  */
extern int pthread_spin_lock(pthread_spinlock_t *__lock);

/* Try to lock spinlock LOCK.  */
extern int pthread_spin_trylock(pthread_spinlock_t *__lock);

/* Release spinlock LOCK.  */
extern int pthread_spin_unlock(pthread_spinlock_t *__lock);

#endif//__USE_XOPEN2K

#endif//_PTHREAD_H


#endif //__CRTL_BITS_LOCK_SPIN_H

