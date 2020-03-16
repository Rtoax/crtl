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
    crtl_print_debug("Rong.\n");

//    
//    pthread_cleanup_push(&demo_thread_create_cleanup_fn, NULL);

    pthread_exit(NULL);
    
    return NULL;
}

void demo_thread_create()
{
    crtl_thread_t thread;
    crtl_thread_normal(&thread, demo_thread_create_fn, NULL, NULL, NULL);
    
    
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

