#ifndef __CRTL_BITS_LOCK_RWLOCK_H
#define __CRTL_BITS_LOCK_RWLOCK_H 1


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <pthread.h>


typedef pthread_rwlock_t        crtl_lock_rw_t;
typedef pthread_rwlockattr_t    crtl_lock_rwattr_t;

#define CRTL_LOCK_RWLOCK_INITIALIZER PTHREAD_RWLOCK_INITIALIZER

int crtl_rwlock_init(crtl_lock_rw_t *rwlock, const crtl_lock_rwattr_t *attr);
int crtl_rwlock_destroy(crtl_lock_rw_t *rwlock);
int crtl_rwlock_rdlock(crtl_lock_rw_t *rwlock, int trywait, int timedwait, int seconds, long nanoseconds);
int crtl_rwlock_wrlock(crtl_lock_rw_t *rwlock, int trywait, int timedwait, int seconds, long nanoseconds);
int crtl_rwlock_unlock(crtl_lock_rw_t *rwlock);



int crtl_rwlockattr_init(crtl_lock_rwattr_t *__attr);
int crtl_rwlockattr_destroy(crtl_lock_rwattr_t *__attr);
int crtl_rwlockattr_getpshared(const crtl_lock_rwattr_t *__attr, int *__pshared);
int crtl_rwlockattr_setpshared(crtl_lock_rwattr_t *__attr, int __pshared);
int crtl_rwlockattr_setpshared_shared(crtl_lock_rwattr_t *__attr);
int crtl_rwlockattr_setpshared_private(crtl_lock_rwattr_t *__attr);





#ifndef _PTHREAD_H


#if defined __USE_UNIX98 || defined __USE_XOPEN2K
/* Functions for handling read-write locks.  */

/* Initialize read-write lock RWLOCK using attributes ATTR, or use
   the default values if later is NULL.  */
extern int pthread_rwlock_init (pthread_rwlock_t *__restrict __rwlock,
				const pthread_rwlockattr_t *__restrict
				__attr);

/* Destroy read-write lock RWLOCK.  */
extern int pthread_rwlock_destroy (pthread_rwlock_t *__rwlock);

/* Acquire read lock for RWLOCK.  */
extern int pthread_rwlock_rdlock (pthread_rwlock_t *__rwlock);

/* Try to acquire read lock for RWLOCK.  */
extern int pthread_rwlock_tryrdlock (pthread_rwlock_t *__rwlock);

# ifdef __USE_XOPEN2K
/* Try to acquire read lock for RWLOCK or return after specfied time.  */
extern int pthread_rwlock_timedrdlock (pthread_rwlock_t *__restrict __rwlock,
				       const struct timespec *__restrict
				       __abstime);
# endif

/* Acquire write lock for RWLOCK.  */
extern int pthread_rwlock_wrlock (pthread_rwlock_t *__rwlock);

/* Try to acquire write lock for RWLOCK.  */
extern int pthread_rwlock_trywrlock (pthread_rwlock_t *__rwlock);

# ifdef __USE_XOPEN2K
/* Try to acquire write lock for RWLOCK or return after specfied time.  */
extern int pthread_rwlock_timedwrlock (pthread_rwlock_t *__restrict __rwlock,
				       const struct timespec *__restrict
				       __abstime);
# endif

/* Unlock RWLOCK.  */
extern int pthread_rwlock_unlock (pthread_rwlock_t *__rwlock);


/* Functions for handling read-write lock attributes.  */

/* Initialize attribute object ATTR with default values.  */
extern int pthread_rwlockattr_init(pthread_rwlockattr_t *__attr);

/* Destroy attribute object ATTR.  */
extern int pthread_rwlockattr_destroy(pthread_rwlockattr_t *__attr);

/* Return current setting of process-shared attribute of ATTR in PSHARED.  */
extern int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *
					  __restrict __attr,
					  int *__restrict __pshared);

/* Set process-shared attribute of ATTR to PSHARED.  */
extern int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *__attr,
					  int __pshared);

/* Return current setting of reader/writer preference.  */
extern int pthread_rwlockattr_getkind_np(const pthread_rwlockattr_t *
					  __restrict __attr,
					  int *__restrict __pref);

/* Set reader/write preference.  */
extern int pthread_rwlockattr_setkind_np(pthread_rwlockattr_t *__attr,
					  int __pref);

#endif///defined __USE_UNIX98 || defined __USE_XOPEN2K


#endif//_PTHREAD_H


#endif //__CRTL_BITS_LOCK_SPIN_H


