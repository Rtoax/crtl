#ifndef __CRTL_BITS_LOCK_MUTEX_H
#define __CRTL_BITS_LOCK_MUTEX_H 1


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <pthread.h>


typedef pthread_mutex_t     crtl_lock_mutex_t;
typedef pthread_mutexattr_t crtl_lock_mutexattr_t;


#define CRTL_LOCK_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER;



int crtl_mutex_init(crtl_lock_mutex_t *mutex, const crtl_lock_mutexattr_t *attr);
int crtl_mutex_destroy(crtl_lock_mutex_t *mutex);
int crtl_mutex_lock(crtl_lock_mutex_t *__mutex, int trywait, int timedwait, int seconds, long nanoseconds);
int crtl_mutex_unlock(crtl_lock_mutex_t *__mutex);




int crtl_mutexattr_init(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_destroy(crtl_lock_mutexattr_t *__attr);

int crtl_mutexattr_getpshared(const crtl_lock_mutexattr_t *__attr, int *__pshared);
int crtl_mutexattr_setpshared_shared(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setpshared_private(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setpshared(crtl_lock_mutexattr_t *__attr, int __pshared);

int crtl_mutexattr_gettype(const crtl_lock_mutexattr_t *__attr, int *__kind);
int crtl_mutexattr_settype_normal(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_settype_errorcheck(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_settype_recursive(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_settype_default(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_settype(crtl_lock_mutexattr_t *__attr, int __kind);

int crtl_mutexattr_getprotocol(const crtl_lock_mutexattr_t *__attr, int *__protocol);
int crtl_mutexattr_setprotocol_none(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setprotocol_inherit(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setprotocol_protect(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setprotocol(crtl_lock_mutexattr_t *__attr, int __protocol);


int crtl_mutexattr_getrobust(const crtl_lock_mutexattr_t *__attr, int *__robustness);
int crtl_mutexattr_setrobust_stalled(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setrobust_robust(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setrobust(crtl_lock_mutexattr_t *__attr, int __robustness);







#endif //__CRTL_BITS_LOCK_MUTEX_H

