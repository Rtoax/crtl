#include "crtl/bits/crtl_thread.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"
#include "crtl/crtl_types.h"


#include "crtl/bits/crtl_types_bits_set.h"




int crtl_thread_create(crtl_thread_t * __newthread, int __detachstate, size_t __guardsize, 
                         int __sched_priority, int __sched_policy, int __scope, 
                         void *__stackaddr, size_t __stacksize, int cpu_core_id,
                         crtl_thread_start_routine_fn __start_routine, void *__start_arg,
                         crtl_thread_cleanup_routine_fn __cleanup_routine, void *__clean_arg)
{
    crtl_threadattr_t thread_attr;
    crtl_threadattr_init(&thread_attr);
    crtl_threadattr_setdetachstate(&thread_attr, __detachstate);
    crtl_threadattr_setguardsize(&thread_attr, __guardsize);
    crtl_threadattr_setsched(&thread_attr, __sched_priority, __sched_policy);
    crtl_threadattr_setscope(&thread_attr, __scope);
    crtl_threadattr_setstack(&thread_attr, __stackaddr, __stacksize);

    cpu_set_t __cpuset;
    
	CPU_ZERO(&__cpuset);
	CPU_SET(cpu_core_id, &__cpuset);

    crtl_threadattr_setaffinity(&thread_attr, &__cpuset);
    
    if(0 != pthread_create(__newthread, &thread_attr, __start_routine, __start_arg)) {
        crtl_print_err("pthread_create error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

crtl_thread_t crtl_thread_self()
{
    pthread_t threadid = 0;
    if(!(threadid = pthread_self())) {
        crtl_print_err("pthread_self error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return threadid;
}

void crtl_thread_exit(int *retval)
{
    pthread_exit(retval);
}


int crtl_thread_join(crtl_thread_t __th, void **__thread_return, int tryjoin, int timedjoin, int seconds, long nanoseconds)
{
#ifdef __USE_GNU
    if(tryjoin) {
        if(0 != pthread_tryjoin_np(__th, __thread_return)) {
            crtl_print_err("pthread_tryjoin_np error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
    } else if (timedjoin) {
        //struct timespec {
        //    time_t tv_sec;      /* Seconds */
        //    long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
        //};
        struct timespec timespec = {seconds, nanoseconds};
        if(0 != pthread_timedjoin_np(__th, __thread_return, &timespec)) {
            crtl_print_err("pthread_timedjoin_np error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }    
    } else 
#endif //  __USE_GNU  
    {
        if(0 != pthread_join(__th, __thread_return)) {
            crtl_print_err("pthread_join error. %s\n", CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
    }
    return CRTL_SUCCESS;
}

int crtl_thread_getattr(crtl_thread_t thread, crtl_threadattr_t *attr)
{
    if(0 != pthread_getattr_np(thread, attr)) {
        crtl_print_err("pthread_getattr_np error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}
int crtl_thread_equal(crtl_thread_t thread1, crtl_thread_t thread2)
{
    if(0 == pthread_equal(thread1, thread2)) {
        crtl_print_err("pthread_equal error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}



int crtl_thread_getschedparam(crtl_thread_t thread, int *__sched_priority, int *__sched_policy)
{
    struct sched_param sched;
    
    if(0 != pthread_getschedparam(thread, __sched_policy, &sched)) {
        crtl_print_err("pthread_getschedparam error. %s\n", CRTL_SYS_ERROR);
        *__sched_priority = -1;
        return CRTL_ERROR;
    }
    
    *__sched_priority = sched.sched_priority;
    return CRTL_SUCCESS;
}

int crtl_thread_setschedparam(crtl_thread_t thread, int __sched_priority/*1-99*/, int __sched_policy)
{
    struct sched_param sched;
    sched.sched_priority = __sched_priority;
    
    if(0 != pthread_setschedparam(thread, __sched_policy, &sched)) {
        crtl_print_err("pthread_setschedparam error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    
    return CRTL_SUCCESS;
}

int crtl_thread_setschedparam_fifo(crtl_thread_t thread, int __sched_priority/*1-99*/)
{//SCHED_FIFO, SCHED_RR, and SCHED_OTHER
    return crtl_thread_setschedparam(thread, __sched_priority, SCHED_FIFO);
}
int crtl_thread_setschedparam_rr(crtl_thread_t thread, int __sched_priority/*1-99*/)
{//SCHED_FIFO, SCHED_RR, and SCHED_OTHER
    return crtl_thread_setschedparam(thread, __sched_priority, SCHED_RR);//轮询
}

int crtl_thread_setschedparam_other(crtl_thread_t thread)
{//SCHED_FIFO, SCHED_RR, and SCHED_OTHER
    return crtl_thread_setschedparam(thread, 0, SCHED_OTHER);
}

int crtl_thread_setschedprio(crtl_thread_t thread, int __sched_priority/*1-99*/)
{
    if(0 != pthread_setschedprio(thread, __sched_priority)) {
        crtl_print_err("pthread_setschedprio error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_thread_getname(crtl_thread_t thread, char *__buf, int buf_len)
{
#ifdef __USE_GNU
    if(0 != pthread_getname_np(thread, __buf, buf_len)) {
        crtl_print_err("pthread_getname_np error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#else
    strcpy(__buf, "unknown");
#endif
    return CRTL_SUCCESS;
}
int crtl_thread_setname(crtl_thread_t thread, const char *__name)
{
#ifdef __USE_GNU
    if(0 != pthread_setname_np(thread, __name)) {
        crtl_print_err("pthread_setname_np error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#else
    crtl_print_err("pthread_setname_np undefine __USE_GNU error. \n");
#endif
    return CRTL_SUCCESS;
}

int crtl_thread_getconcurrency(void)
{
#ifdef __USE_UNIX98
    return pthread_getconcurrency();
#else
    crtl_print_err("pthread_getconcurrency not support error.\n");
    return 0;
#endif
}
int crtl_thread_setconcurrency(int __level)
{
#ifdef __USE_UNIX98
    if(0 != pthread_setconcurrency(__level)) {
        crtl_print_err("pthread_setconcurrency error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#else
    crtl_print_err("pthread_setconcurrency not support error.\n");
    return CRTL_ERROR;
#endif
    return CRTL_SUCCESS;
}


int crtl_thread_yield(void)
{
#ifdef __USE_GNU
    if(0 != pthread_yield()) {
        crtl_print_err("pthread_yield error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#endif    
    return CRTL_SUCCESS;
}



int crtl_thread_getaffinity(const crtl_thread_t thread, cpu_set_t *__cpuset)
{
#ifdef __USE_GNU
    if(0 != pthread_getaffinity_np(thread, sizeof(cpu_set_t), __cpuset)) {
        crtl_print_err("pthread_getaffinity_np error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#endif //__USE_GNU
    return CRTL_SUCCESS;
}

int crtl_thread_setaffinity(crtl_thread_t thread, const cpu_set_t *__cpuset)
{
#ifdef __USE_GNU
    if(0 != pthread_setaffinity_np(thread, sizeof(cpu_set_t), __cpuset)) {
        crtl_print_err("pthread_setaffinity_np error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#endif //__USE_GNU
    return CRTL_SUCCESS;
}


int crtl_thread_once(crtl_thread_once_t *__once_control, void (*__init_routine) (void))
{
    if(0 != pthread_once(__once_control, __init_routine)) {
        crtl_print_err("pthread_once error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_thread_setcancelstate(int __state, int *__oldstate)
{//PTHREAD_CANCEL_ENABLE（缺省）和PTHREAD_CANCEL_DISABLE
    if(0 != pthread_setcancelstate(__state, __oldstate)) {
        crtl_print_err("pthread_setcancelstate error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_thread_setcancelstate_enable(int *__oldstate)
{
    return crtl_thread_setcancelstate(PTHREAD_CANCEL_ENABLE, __oldstate);
}
int crtl_thread_setcancelstate_disable(int *__oldstate)
{
    return crtl_thread_setcancelstate(PTHREAD_CANCEL_DISABLE, __oldstate);
}

int crtl_thread_setcanceltype(int __type, int *__oldtype)
{//PTHREAD_CANCEL_DEFERRED和PTHREAD_CANCEL_ASYNCHRONOUS
    if(0 != pthread_setcanceltype(__type, __oldtype)) {
        crtl_print_err("pthread_setcanceltype error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}
int crtl_thread_setcanceltype_deferred(int *__oldtype)
{
    return crtl_thread_setcanceltype(PTHREAD_CANCEL_DEFERRED, __oldtype);
}
int crtl_thread_setcanceltype_asynchronous(int *__oldtype)
{
    return crtl_thread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, __oldtype);
}

int crtl_thread_cancel(crtl_thread_t thread)
{
    if(0 != pthread_cancel(thread)) {
        crtl_print_err("pthread_cancel error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

void crtl_thread_testcancel(void){pthread_testcancel();}



int crtl_threadattr_init(crtl_threadattr_t *attr)
{
    if(0 != pthread_attr_init(attr)) {
        crtl_print_err("pthread_attr_init error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_threadattr_destroy(crtl_threadattr_t *attr)
{
    if(0 != pthread_attr_destroy(attr)) {
        crtl_print_err("pthread_attr_destroy error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_threadattr_getdetachstate(const crtl_threadattr_t *__attr, int *__detachstate)
{
    if(0 != pthread_attr_getdetachstate(__attr, __detachstate)) {
        crtl_print_err("pthread_attr_getdetachstate error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_threadattr_setdetachstate_detached(crtl_threadattr_t *__attr)
{
    return crtl_threadattr_setdetachstate(__attr, PTHREAD_CREATE_DETACHED);
}

int crtl_threadattr_setdetachstate_joinable(crtl_threadattr_t *__attr)
{
    return crtl_threadattr_setdetachstate(__attr, PTHREAD_CREATE_JOINABLE);
}

int crtl_threadattr_setdetachstate(crtl_threadattr_t *__attr, int __detachstate)
{
    if(0 != pthread_attr_setdetachstate(__attr, __detachstate)) {
        crtl_print_err("pthread_attr_setdetachstate error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

int crtl_threadattr_getguardsize(const crtl_threadattr_t *__attr, size_t *__guardsize)
{
    if(0 != pthread_attr_getguardsize(__attr, __guardsize)) {
        crtl_print_err("pthread_attr_getguardsize error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}
int crtl_threadattr_setguardsize(crtl_threadattr_t *__attr, size_t __guardsize)
{
    if(0 != pthread_attr_setguardsize(__attr, __guardsize)) {
        crtl_print_err("pthread_attr_setguardsize error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}



int crtl_threadattr_getsched(crtl_threadattr_t *__attr, int *__sched_priority, int *__sched_policy)
{
    if(__sched_priority) {
        struct sched_param sched;
        
        if(0 != pthread_attr_getschedparam(__attr, &sched)) {
            crtl_print_err("pthread_attr_getschedparam error. %s\n", CRTL_SYS_ERROR);
        }
        
        *__sched_priority = sched.sched_priority;
    }
    
    if(__sched_policy) {
        if(0 != pthread_attr_getschedpolicy(__attr, __sched_policy)) {
            crtl_print_err("pthread_attr_getschedpolicy error. %s\n", CRTL_SYS_ERROR);
        }
    }
    return CRTL_SUCCESS;
}

int crtl_threadattr_setsched(crtl_threadattr_t *__attr, int __sched_priority/*0-99*/, int __sched_policy)
{
    crtl_threadattr_setinheritsched_explicit(__attr);
    
    if(0 != pthread_attr_setschedpolicy(__attr, __sched_policy)) {
        crtl_print_err("pthread_attr_setschedpolicy error. %s\n", CRTL_SYS_ERROR);
    }
    
    if(__sched_priority) {
        struct sched_param sched;
        sched.sched_priority = __sched_priority;
        
        if(0 != pthread_attr_setschedparam(__attr, &sched)) {
            crtl_print_err("pthread_attr_setschedparam error. %s\n", CRTL_SYS_ERROR);
        }
    }
    
    return CRTL_SUCCESS;
}

int crtl_threadattr_setsched_fifo(crtl_threadattr_t *__attr, int __sched_priority/*1-99*/)
{//SCHED_FIFO, SCHED_RR, and SCHED_OTHER
    return crtl_threadattr_setsched(__attr, __sched_priority, SCHED_FIFO);
}
int crtl_threadattr_setsched_rr(crtl_threadattr_t *__attr, int __sched_priority/*1-99*/)
{//SCHED_FIFO, SCHED_RR, and SCHED_OTHER
    return crtl_threadattr_setsched(__attr, __sched_priority, SCHED_RR);//轮询
}

int crtl_threadattr_setsched_other(crtl_threadattr_t *__attr)
{//SCHED_FIFO, SCHED_RR, and SCHED_OTHER
    return crtl_threadattr_setsched(__attr, 0, SCHED_OTHER);
}





int crtl_threadattr_getinheritsched(const crtl_threadattr_t *__attr, int *__inherit)
{
    if(0 != pthread_attr_getinheritsched(__attr, __inherit)) {
        crtl_print_err("pthread_attr_getinheritsched error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}
int crtl_threadattr_setinheritsched_inherit(crtl_threadattr_t *__attr)
{
    return crtl_threadattr_setinheritsched(__attr, PTHREAD_INHERIT_SCHED);//继承
}

int crtl_threadattr_setinheritsched_explicit(crtl_threadattr_t *__attr)
{
    return crtl_threadattr_setinheritsched(__attr, PTHREAD_EXPLICIT_SCHED);//显式
}

int crtl_threadattr_setinheritsched(crtl_threadattr_t *__attr, int __inherit)
{
    if(0 != pthread_attr_setinheritsched(__attr, __inherit)) {
        crtl_print_err("pthread_attr_setinheritsched error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}




int crtl_threadattr_getscope(const crtl_threadattr_t *__attr, int *__scope)
{
    if(0 != pthread_attr_getscope(__attr, __scope)) {
        crtl_print_err("pthread_attr_getscope error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}
int crtl_threadattr_setscope_system(crtl_threadattr_t *__attr)
{
    return crtl_threadattr_setscope(__attr, PTHREAD_SCOPE_SYSTEM);
}

int crtl_threadattr_setscope_process(crtl_threadattr_t *__attr)
{
    return crtl_threadattr_setscope(__attr, PTHREAD_SCOPE_PROCESS);
}

int crtl_threadattr_setscope(crtl_threadattr_t *__attr, int __scope)
{
    if(0 != pthread_attr_setscope(__attr, __scope)) {
        crtl_print_err("pthread_attr_setscope error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


int crtl_threadattr_getstack(const crtl_threadattr_t *__attr, void ** __stackaddr, size_t * __stacksize)
{
#ifdef __USE_XOPEN2K
    if(0 != pthread_attr_getstack(__attr, __stackaddr, __stacksize)) {
        crtl_print_err("pthread_attr_getstack error. %s\n", CRTL_SYS_ERROR);
    }
#else
    if(0 != pthread_attr_getstackaddr(__attr, __stackaddr)) {
        crtl_print_err("pthread_attr_getstackaddr error. %s\n", CRTL_SYS_ERROR);
    }
    if(0 != pthread_attr_getstacksize(__attr, __stacksize)) {
        crtl_print_err("pthread_attr_getstacksize error. %s\n", CRTL_SYS_ERROR);
    }
#endif    
    return CRTL_SUCCESS;
}


int crtl_threadattr_setstack(crtl_threadattr_t *__attr, void *__stackaddr, size_t __stacksize)
{
#ifdef __USE_XOPEN2K
    if(__stackaddr && __stacksize)
    if(0 != pthread_attr_setstack(__attr, __stackaddr, __stacksize)) {
        crtl_print_err("pthread_attr_setstack error. %s\n", CRTL_SYS_ERROR);
    }
#else
    if(__stackaddr)
    if(0 != pthread_attr_setstackaddr(__attr, __stackaddr)) {
        crtl_print_err("pthread_attr_setstackaddr error. %s\n", CRTL_SYS_ERROR);
    }
    if(__stacksize)
    if(0 != pthread_attr_setstacksize(__attr, __stacksize)) {
        crtl_print_err("pthread_attr_setstacksize error. %s\n", CRTL_SYS_ERROR);
    }
#endif  
    return CRTL_SUCCESS;
}





int crtl_threadattr_getaffinity(const crtl_threadattr_t *__attr, cpu_set_t *__cpuset)
{
#ifdef __USE_GNU
    if(0 != pthread_attr_getaffinity_np(__attr, sizeof(cpu_set_t), __cpuset)) {
        crtl_print_err("pthread_attr_getaffinity_np error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#endif //__USE_GNU
    return CRTL_SUCCESS;
}

int crtl_threadattr_setaffinity(crtl_threadattr_t *__attr, const cpu_set_t *__cpuset)
{
#ifdef __USE_GNU
    if(0 != pthread_attr_setaffinity_np(__attr, sizeof(cpu_set_t), __cpuset)) {
        crtl_print_err("pthread_attr_setaffinity_np error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
#endif //__USE_GNU
    return CRTL_SUCCESS;
}



int crtl_thread_key_create(crtl_thread_key_t *key, void (*destructor_fn)(void *))
{
    if(0 != pthread_key_create(key, destructor_fn)) {
        crtl_print_err("pthread_key_create error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}
int crtl_thread_key_delete(crtl_thread_key_t key)
{
    if(0 != pthread_key_delete(key)) {
        crtl_print_err("pthread_key_delete error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

void* crtl_thread_key_getspecific(crtl_thread_key_t key)
{
    void *ret = NULL;
    if(NULL == (ret = pthread_getspecific(key))) {
        crtl_print_err("pthread_getspecific error. %s\n", CRTL_SYS_ERROR);
        return NULL;
    }
    return ret;
}

int crtl_thread_key_setspecific(crtl_thread_key_t key, const void *ptr)
{
    if(0 != pthread_setspecific(key, ptr)) {
        crtl_print_err("pthread_setspecific error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}


