#include <crtl/crtl_module.h>
#include "crtl/crtl_task.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"
#include "crtl/crtl_types.h"

void apps_loop_show()
{
//    char **name;
//    int i, napp;
//    crtl_application_getallappname(name, &napp);
//    for(i=0;i<napp;i++)
//    {
//        printf("%s - ", name[i]);
//    }
//    printf("\n");
}

void *demo_thread_fn1(void*arg)
{
    crtl_application_create("DU1");
    crtl_application_create("DU4");
    
    crtl_application_destroy("DU4");
    apps_loop_show();
    return NULL;
}
void *demo_thread_fn2(void*arg)
{
    crtl_application_create("DU2");
    crtl_application_create("DU3");
    crtl_application_destroy("DU1");
    apps_loop_show();
    return NULL;
}

void *demo_thread_fn3_createmodule(void*arg)
{
    
}

void *demo_thread_fn3(void*arg)
{
    crtl_application_create("DU5");
    crtl_application_create("DU6");
    crtl_application_destroy("DU2");
    apps_loop_show();

    //插入一个模块
    crtl_module_create("DU5", "omdu", 256, 1024, demo_thread_fn3_createmodule, NULL);
    
    return NULL;
}

void demo_app_create()
{
    crtl_thread_t thread1, thread2, thread3;
    crtl_thread_normal(&thread1, demo_thread_fn1, NULL);
    crtl_thread_normal(&thread2, demo_thread_fn2, NULL);
    crtl_thread_normal(&thread3, demo_thread_fn3, NULL);
}

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
    
    return NULL;
}


void demo_module_create()
{
    crtl_thread_t thread1, thread2, thread3;
    crtl_thread_normal(&thread1, demo_thread_fn21, NULL);
    crtl_thread_normal(&thread2, demo_thread_fn22, NULL);
    crtl_thread_normal(&thread3, demo_thread_fn23, NULL);
    
    

    
}



int main()
{    
//    demo_app_create();
    demo_module_create();
    
    
    while(1)
    {
        sleep(1);
    }

    return 0;
}


