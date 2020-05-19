#ifndef __CRTL_THREAD_H
#define __CRTL_THREAD_H 1

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <limits.h>

#include "crtl/lock.h"



#define CRTL_THREAD_SCHED_PRIO_TIME_CRITICAL  90    
#define CRTL_THREAD_SCHED_PRIO_HIGHEST    80    
#define CRTL_THREAD_SCHED_PRIO_HIGHER     70    
#define CRTL_THREAD_SCHED_PRIO_HIGH   60   
#define CRTL_THREAD_SCHED_PRIO_ABOVE_NORMAL   50  
#define CRTL_THREAD_SCHED_PRIO_NORMAL     40   
#define CRTL_THREAD_SCHED_PRIO_BELOW_NORMAL    30  
#define CRTL_THREAD_SCHED_PRIO_LOWEST     10
#define CRTL_THREAD_SCHED_PRIO_IDLE   1





#define crtl_thread_normal(p_threadID, fn, arg) \
    crtl_thread_create(p_threadID, PTHREAD_CREATE_DETACHED, 0, \
                        CRTL_THREAD_SCHED_PRIO_NORMAL, SCHED_FIFO, PTHREAD_SCOPE_SYSTEM,\
                        NULL, 0, 0, fn, arg, NULL, NULL)


typedef pthread_t       crtl_thread_t;
typedef pthread_attr_t  crtl_threadattr_t;
typedef pthread_once_t  crtl_thread_once_t;
typedef pthread_key_t   crtl_thread_key_t;



#define CRTL_THREAD_ONCE_INIT   PTHREAD_ONCE_INIT


typedef void *(*crtl_thread_start_routine_fn)(void *);
typedef void (*crtl_thread_cleanup_routine_fn)(void *);







int crtl_threadattr_init(crtl_threadattr_t *attr);
int crtl_threadattr_destroy(crtl_threadattr_t *attr);

int crtl_threadattr_getdetachstate(const crtl_threadattr_t *__attr, int *__detachstate);
int crtl_threadattr_setdetachstate_detached(crtl_threadattr_t *__attr);
int crtl_threadattr_setdetachstate_joinable(crtl_threadattr_t *__attr);
int crtl_threadattr_setdetachstate(crtl_threadattr_t *__attr, int __detachstate);


int crtl_threadattr_getguardsize(const crtl_threadattr_t *__attr, size_t *__guardsize);
int crtl_threadattr_setguardsize(crtl_threadattr_t *__attr, size_t __guardsize);


int crtl_threadattr_getsched(crtl_threadattr_t *__attr, int *__sched_priority, int *__sched_policy);
int crtl_threadattr_setsched(crtl_threadattr_t *__attr, int __sched_priority/*1-99*/, int __sched_policy);
int crtl_threadattr_setsched_fifo(crtl_threadattr_t *__attr, int __sched_priority/*1-99*/);
int crtl_threadattr_setsched_rr(crtl_threadattr_t *__attr, int __sched_priority/*1-99*/);
int crtl_threadattr_setsched_other(crtl_threadattr_t *__attr);


int crtl_threadattr_getinheritsched(const crtl_threadattr_t *__attr, int *__inherit);
int crtl_threadattr_setinheritsched_inherit(crtl_threadattr_t *__attr);
int crtl_threadattr_setinheritsched_explicit(crtl_threadattr_t *__attr);
int crtl_threadattr_setinheritsched(crtl_threadattr_t *__attr, int __inherit);


int crtl_threadattr_getscope(const crtl_threadattr_t *__attr, int *__scope);
int crtl_threadattr_setscope_system(crtl_threadattr_t *__attr);
int crtl_threadattr_setscope_process(crtl_threadattr_t *__attr);
int crtl_threadattr_setscope(crtl_threadattr_t *__attr, int __scope);

size_t crtl_thread_getstack_size(void);
int crtl_threadattr_getstack(const crtl_threadattr_t *__attr, void ** __stackaddr, size_t * __stacksize);
int crtl_threadattr_setstack(crtl_threadattr_t *__attr, void *__stackaddr, size_t __stacksize);

int crtl_threadattr_getaffinity(const crtl_threadattr_t *__attr, cpu_set_t *__cpuset);
int crtl_threadattr_setaffinity(crtl_threadattr_t *__attr, const cpu_set_t *__cpuset);


int crtl_thread_create(crtl_thread_t * __newthread, int __detachstate, size_t __guardsize, 
                         int __sched_priority, int __sched_policy, int __scope, 
                         void *__stackaddr, size_t __stacksize, int cpu_core_id,
                         crtl_thread_start_routine_fn __start_routine, void *__start_arg,
                         crtl_thread_cleanup_routine_fn __cleanup_routine, void *__clean_arg);

crtl_thread_t crtl_thread_self();
void crtl_thread_exit(int *retval);
int crtl_thread_kill(crtl_thread_t thread, int sig);
int crtl_thread_kill_self(int sig);

int crtl_thread_join(crtl_thread_t __th, void **__thread_return, int tryjoin, int timedjoin, int seconds, long nanoseconds);
int crtl_thread_getattr(crtl_thread_t thread, crtl_threadattr_t *attr);

int crtl_thread_equal(crtl_thread_t thread1, crtl_thread_t thread2);

int crtl_thread_getschedparam(crtl_thread_t thread, int *__sched_priority, int *__sched_policy);
int crtl_thread_setschedparam(crtl_thread_t thread, int __sched_priority/*1-99*/, int __sched_policy);
int crtl_thread_setschedparam_fifo(crtl_thread_t thread, int __sched_priority/*1-99*/);
int crtl_thread_setschedparam_rr(crtl_thread_t thread, int __sched_priority/*1-99*/);
int crtl_thread_setschedparam_other(crtl_thread_t thread);
int crtl_thread_setschedprio(crtl_thread_t thread, int __sched_priority/*1-99*/);

int crtl_thread_getname(crtl_thread_t thread, char *__buf, int buf_len);
int crtl_thread_setname(crtl_thread_t thread, const char *__name);

int crtl_thread_getconcurrency(void);
int crtl_thread_setconcurrency(int __level);

int crtl_thread_yield(void);


int crtl_thread_getaffinity(const crtl_thread_t thread, cpu_set_t *__cpuset);
int crtl_thread_setaffinity(crtl_thread_t thread, const cpu_set_t *__cpuset);

int crtl_thread_once(crtl_thread_once_t *__once_control, void (*__init_routine) (void));


int crtl_thread_setcancelstate(int __state, int *__oldstate);
int crtl_thread_setcancelstate_enable(int *__oldstate);
int crtl_thread_setcancelstate_disable(int *__oldstate);

int crtl_thread_setcanceltype(int __type, int *__oldtype);
int crtl_thread_setcanceltype_deferred(int *__oldtype);
int crtl_thread_setcanceltype_asynchronous(int *__oldtype);

int crtl_thread_cancel(crtl_thread_t thread);
void crtl_thread_testcancel(void);

int crtl_thread_key_create(crtl_thread_key_t *key, void (*destructor_fn)(void *));
int crtl_thread_key_delete(crtl_thread_key_t key);
void* crtl_thread_key_getspecific(crtl_thread_key_t key);
int crtl_thread_key_setspecific(crtl_thread_key_t key, const void *ptr);





#endif /*<__CRTL_THREAD_H>*/



