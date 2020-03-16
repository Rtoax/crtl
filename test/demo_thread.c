#include "crtl/crtl_task.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"
#include "crtl/crtl_types.h"

//void demo_thread_create_cleanup_fn(void*arg)
//{
//    crtl_print_debug("Rong.\n");
//}


void *demo_thread_create_fn(void*arg)
{
    crtl_print_debug("Rong. %ld\n", crtl_thread_self());

//    
//    pthread_cleanup_push(&demo_thread_create_cleanup_fn, NULL);

    crtl_thread_exit(NULL);
    
    return NULL;
}

void demo_thread_create()
{
    crtl_thread_t thread;
    crtl_thread_normal(&thread, demo_thread_create_fn, NULL, NULL, NULL);
    crtl_print_debug("Rong. %ld\n", thread);

    crtl_thread_join(thread, NULL, 0,0,0,0);
    crtl_print_debug("crtl_thread_join. %ld\n", thread);

    crtl_threadattr_t attr;
    crtl_print_debug("crtl_thread_getattr. %d\n", crtl_thread_getattr(thread, &attr));
    crtl_print_debug("crtl_thread_equal. %d\n", crtl_thread_equal(thread, thread));

    
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

