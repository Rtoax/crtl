#ifndef __CRTL_BITS_LOCK_RWLOCK_H
#define __CRTL_BITS_LOCK_RWLOCK_H 1


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <pthread.h>


typedef pthread_rwlock_t        crtl_lock_rw_t;
typedef pthread_rwlockattr_t    crtl_lock_rwattr_t;

#define CRTL_LOCK_RWLOCK_INITIALIZER PTHREAD_RWLOCK_INITIALIZER

#define CRTL_LOCK_RWLOCK_SHARE_INIT(p_rw_lock) ({\
        crtl_lock_rwattr_t rwlock_attr; crtl_rwlockattr_init(&rwlock_attr);\
        crtl_rwlockattr_setpshared_shared(&rwlock_attr);\
        crtl_rwlock_init((p_rw_lock), &rwlock_attr);\
    })
#define CRTL_LOCK_RWLOCK_PRIVATE_INIT(p_rw_lock) ({\
        crtl_lock_rwattr_t rwlock_attr; crtl_rwlockattr_init(&rwlock_attr);\
        crtl_rwlockattr_setpshared_private(&rwlock_attr);\
        crtl_rwlock_init((p_rw_lock), &rwlock_attr);\
    })

#define CRTL_LOCK_RWLOCK_RDLOCK(p_rw_lock) ({crtl_rwlock_rdlock((p_rw_lock), 0,0,0,0);})
#define CRTL_LOCK_RWLOCK_WRLOCK(p_rw_lock) ({crtl_rwlock_wrlock((p_rw_lock), 0,0,0,0);})
#define CRTL_LOCK_RWLOCK_UNLOCK(p_rw_lock) ({crtl_rwlock_unlock((p_rw_lock));})
#define CRTL_LOCK_RWLOCK_DESTROY(p_rw_lock) ({crtl_rwlock_destroy((p_rw_lock));})



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





#endif //__CRTL_BITS_LOCK_SPIN_H


