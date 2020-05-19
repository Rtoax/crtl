#include "crtl/crtl_task.h"
#include "crtl/log.h"
#include "crtl/assert.h"
#include "crtl/types.h"

//void demo_thread_create_cleanup_fn(void*arg)
//{
//    crtl_print_debug("Rong.\n");
//}


crtl_thread_once_t once = CRTL_THREAD_ONCE_INIT;
void init_routine()
{
    crtl_print_debug(">>>once. %ld\n", crtl_thread_self());
}

void *demo_thread_create_fn(void*arg)
{
    crtl_print_debug("Rong. %ld\n", crtl_thread_self());

//    
//    pthread_cleanup_push(&demo_thread_create_cleanup_fn, NULL);

    crtl_print_debug("crtl_thread_getconcurrency. %d\n", crtl_thread_getconcurrency());
    
    crtl_thread_once(&once, &init_routine);
    crtl_thread_once(&once, &init_routine);//只调用一次

    crtl_lock_cond_t cond;
    crtl_cond_init(&cond, NULL);
    crtl_lock_mutex_t mutex;
    crtl_mutex_init(&mutex, NULL);
    crtl_cond_wait(&cond, &mutex, 0,0,0);
    crtl_thread_exit(NULL);
    
    return NULL;
}


void demo_thread_create()
{
    crtl_thread_t thread, thread2;
    crtl_thread_normal(&thread, demo_thread_create_fn, NULL);
    crtl_thread_normal(&thread2, demo_thread_create_fn, NULL);
    
    crtl_print_debug("thread. %ld\n", thread);

    crtl_thread_join(thread, NULL, 0,0,0,0);
    crtl_print_debug("crtl_thread_join. %ld\n", thread);

    crtl_threadattr_t attr;
    crtl_print_debug("crtl_thread_getattr. %d\n", crtl_thread_getattr(thread, &attr));
    crtl_print_debug("crtl_thread_equal. %d\n", crtl_thread_equal(thread, thread));

    int __sched_priority;
    int __sched_policy;
    crtl_thread_getschedparam(thread, &__sched_priority, &__sched_policy);
    crtl_print_debug("crtl_thread_getschedparam. %d, %d\n", __sched_priority, __sched_policy);

    crtl_print_debug("thread. %ld\n", thread);

    crtl_thread_setschedparam_fifo(thread, 80);
    crtl_thread_getschedparam(thread, &__sched_priority, &__sched_policy);
    crtl_print_debug("crtl_thread_getschedparam. %d, %d\n", __sched_priority, __sched_policy);

    crtl_print_debug("thread. %ld\n", thread);

    char name[256];
    crtl_thread_getname(thread, name, 256);
    crtl_print_debug("crtl_thread_getname. %s\n", name);
    
    
    char rename[256] = {"Rongtao"};
    crtl_thread_setname(thread, rename);
    crtl_thread_getname(thread, name, 256);
    crtl_print_debug("crtl_thread_getname. %s\n", name);

    cpu_set_t cpuset;
    crtl_thread_getaffinity(thread, &cpuset);

    crtl_thread_cancel(thread);
    crtl_thread_cancel(thread2);

    
}


int main()
{
    demo_thread_create();
    
    while(1)
    {
        sleep(1);
    }

    return 0;
}

