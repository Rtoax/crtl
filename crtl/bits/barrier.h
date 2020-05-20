#ifndef __CRTL_BITS_LOCK_BARRIER_H
#define __CRTL_BITS_LOCK_BARRIER_H 1


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <pthread.h>

typedef pthread_barrier_t       crtl_lock_barrier_t;
typedef pthread_barrierattr_t   crtl_lock_barrierattr_t;


int crtl_barrier_init(crtl_lock_barrier_t *barrier, const crtl_lock_barrierattr_t *attr, unsigned int __count);
int crtl_barrier_destroy(crtl_lock_barrier_t *barrier);
int crtl_barrier_wait(crtl_lock_barrier_t *barrier);


int crtl_barrierattr_init(crtl_lock_barrierattr_t *__attr);
int crtl_barrierattr_destroy(crtl_lock_barrierattr_t *__attr);
int crtl_barrierattr_getpshared(const crtl_lock_barrierattr_t *__attr, int *__pshared);
int crtl_barrierattr_setpshared(crtl_lock_barrierattr_t *__attr, int __pshared);
int crtl_barrierattr_setpshared_shared(crtl_lock_barrierattr_t *__attr);
int crtl_barrierattr_setpshared_private(crtl_lock_barrierattr_t *__attr);



#ifndef _PTHREAD_H


#ifdef __USE_XOPEN2K

/* Functions to handle barriers.  */

/* Initialize BARRIER with the attributes in ATTR.  The barrier is
   opened when COUNT waiters arrived.  */
extern int pthread_barrier_init(pthread_barrier_t *__restrict __barrier,
				 const pthread_barrierattr_t *__restrict
				 __attr, unsigned int __count);

/* Destroy a previously dynamically initialized barrier BARRIER.  */
extern int pthread_barrier_destroy(pthread_barrier_t *__barrier);

/* Wait on barrier BARRIER.  */
extern int pthread_barrier_wait(pthread_barrier_t *__barrier);


/* Initialize barrier attribute ATTR.  */
extern int pthread_barrierattr_init(pthread_barrierattr_t *__attr);

/* Destroy previously dynamically initialized barrier attribute ATTR.  */
extern int pthread_barrierattr_destroy(pthread_barrierattr_t *__attr);

/* Get the process-shared flag of the barrier attribute ATTR.  */
extern int pthread_barrierattr_getpshared(const pthread_barrierattr_t *
					   __restrict __attr,
					   int *__restrict __pshared);

/* Set the process-shared flag of the barrier attribute ATTR.  */
extern int pthread_barrierattr_setpshared(pthread_barrierattr_t *__attr,
					   int __pshared);
#endif


#endif//_PTHREAD_H


#endif //__CRTL_BITS_LOCK_BARRIER_H




