#include <crtl/crtl_module.h>
#include "crtl/crtl_task.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"
#include "crtl/crtl_types.h"


void *demo_module_create_omdu_main_task(void*arg)
{
    
}
void *demo_module_create_omcucp_main_task(void*arg)
{
    
}
void *demo_module_create_omcuup_main_task(void*arg)
{
    
}
void *demo_thread_fn21(void*arg)
{
    sleep(1);

    crtl_module_create("OM", "omcucp", 256, 1024, demo_module_create_omcucp_main_task, NULL);
    crtl_module_create("OM", "omcuup", 256, 1024, demo_module_create_omcuup_main_task, NULL);
    crtl_module_destroy("OM", "omcuup");
    return NULL;
}
void *demo_thread_fn22(void*arg)
{
    sleep(2);
    crtl_module_destroy("OM", "omcucp");
    crtl_module_create("OM", "omcucp", 256, 1024, demo_module_create_omcucp_main_task, NULL);
    crtl_module_create("OM", "omcuup", 256, 1024, demo_module_create_omcuup_main_task, NULL);

    sleep(3);
    crtl_module_create("OM", "omcucp", 256, 1024, demo_module_create_omcucp_main_task, NULL);

    
    sleep(3);
    crtl_module_destroy("OM", "omcucp");
    
    sleep(3);
    crtl_module_destroy("OM", "omdu");

    sleep(3);
    

    while(1)
    {
        sleep(1);
        crtl_application_create("OM");
    }

    
    return NULL;
}
void *demo_thread_fn23(void*arg)
{
    crtl_application_create("OM");
    crtl_module_create("OM", "omdu", 256, 1024, demo_module_create_omdu_main_task, NULL);
    
    sleep(3);
    crtl_module_destroy("OM", "omcucp");
    crtl_module_destroy("OM", "omcuup");
    
    crtl_module_create("OM", "omcucp", 256, 1024, demo_module_create_omcucp_main_task, NULL);
    crtl_module_create("OM", "omcuup", 256, 1024, demo_module_create_omcuup_main_task, NULL);
    
    crtl_module_destroy("OM", "omcucp");
    crtl_module_destroy("OM", "omcuup");



    
    return NULL;
}

void *demo_thread_fn24(void*arg)
{
    while(1)
    {
        sleep(1);
        crtl_application_destroy("OM");
    }
    
    return NULL;
}


void demo_module_create()
{
    crtl_thread_t thread1, thread2, thread3, thread4;
    crtl_thread_normal(&thread1, demo_thread_fn21, NULL);
    crtl_thread_normal(&thread2, demo_thread_fn22, NULL);
    crtl_thread_normal(&thread3, demo_thread_fn23, NULL);
    crtl_thread_normal(&thread4, demo_thread_fn24, NULL);
    
}



int main()
{    
    demo_module_create();

    
    
    while(1)
    {
        sleep(1);
    }

    return 0;
}


