#ifndef __CRTL_BITS_LOCK_COND_H
#define __CRTL_BITS_LOCK_COND_H 1


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <pthread.h>
#include "crtl/bits/crtl_lock_mutex.h"

typedef pthread_cond_t      crtl_lock_cond_t;
typedef pthread_condattr_t  crtl_lock_condattr_t;

#define CRTL_LOCK_COND_INITIALIZER PTHREAD_COND_INITIALIZER;


int crtl_cond_init(crtl_lock_cond_t *cond, const crtl_lock_condattr_t *attr);
int crtl_cond_destroy(crtl_lock_cond_t *cond);
int crtl_cond_signal(crtl_lock_cond_t *cond);
int crtl_cond_broadcast(crtl_lock_cond_t *cond);
int crtl_cond_wait(crtl_lock_cond_t *__cond, crtl_lock_mutex_t *__mutex, int timedwait, int seconds, long nanoseconds);



int crtl_condattr_init(crtl_lock_condattr_t *__attr);
int crtl_condattr_destroy(crtl_lock_condattr_t *__attr);
int crtl_condattr_getpshared(const crtl_lock_condattr_t *__attr, int *__pshared);
int crtl_condattr_setpshared(crtl_lock_condattr_t *__attr, int __pshared);

int crtl_condattr_setpshared_shared(crtl_lock_condattr_t *__attr);
int crtl_condattr_setpshared_private(crtl_lock_condattr_t *__attr);
int crtl_condattr_getclock(const crtl_lock_condattr_t *__attr, int *__clock_id);

int crtl_condattr_setclock(crtl_lock_condattr_t *__attr, int __clock_id);
int crtl_condattr_setclock_realtime(crtl_lock_condattr_t *__attr, int __clock_id);
int crtl_condattr_setclock_monotonic(crtl_lock_condattr_t *__attr, int __clock_id);
int crtl_condattr_setclock_process_cputime_id(crtl_lock_condattr_t *__attr, int __clock_id);
int crtl_condattr_setclock_thread_cputime_id(crtl_lock_condattr_t *__attr, int __clock_id);





#ifndef _PTHREAD_H


/* Functions for handling conditional variables.  */

/* Initialize condition variable COND using attributes ATTR, or use
   the default values if later is NULL.  */
extern int pthread_cond_init(pthread_cond_t *__restrict __cond,
			      const pthread_condattr_t *__restrict __cond_attr);

/* Destroy condition variable COND.  */
extern int pthread_cond_destroy(pthread_cond_t *__cond);

/* Wake up one thread waiting for condition variable COND.  */
extern int pthread_cond_signal(pthread_cond_t *__cond);

/* Wake up all threads waiting for condition variables COND.  */
extern int pthread_cond_broadcast(pthread_cond_t *__cond);

/* Wait for condition variable COND to be signaled or broadcast.
   MUTEX is assumed to be locked before.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int pthread_cond_wait(pthread_cond_t *__restrict __cond,
			      pthread_mutex_t *__restrict __mutex);

/* Wait for condition variable COND to be signaled or broadcast until
   ABSTIME.  MUTEX is assumed to be locked before.  ABSTIME is an
   absolute time specification; zero is the beginning of the epoch
   (00:00:00 GMT, January 1, 1970).

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int pthread_cond_timedwait(pthread_cond_t *__restrict __cond,
				   pthread_mutex_t *__restrict __mutex,
				   const struct timespec *__restrict __abstime);

/* Functions for handling condition variable attributes.  */

/* Initialize condition variable attribute ATTR.  */
extern int pthread_condattr_init(pthread_condattr_t *__attr);

/* Destroy condition variable attribute ATTR.  */
extern int pthread_condattr_destroy(pthread_condattr_t *__attr);

/* Get the process-shared flag of the condition variable attribute ATTR.  */
extern int pthread_condattr_getpshared(const pthread_condattr_t *
					__restrict __attr,
					int *__restrict __pshared);

/* Set the process-shared flag of the condition variable attribute ATTR.  */
extern int pthread_condattr_setpshared(pthread_condattr_t *__attr,
					int __pshared);

#ifdef __USE_XOPEN2K
/* Get the clock selected for the conditon variable attribute ATTR.  */
extern int pthread_condattr_getclock(const pthread_condattr_t *
				      __restrict __attr,
				      __clockid_t *__restrict __clock_id);

/* Set the clock selected for the conditon variable attribute ATTR.  */
extern int pthread_condattr_setclock(pthread_condattr_t *__attr,
				      __clockid_t __clock_id);
#endif


#endif//_PTHREAD_H


#endif //__CRTL_BITS_LOCK_COND_H



