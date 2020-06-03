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




#endif //__CRTL_BITS_LOCK_BARRIER_H




