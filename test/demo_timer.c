#include <crtl/crtl_tree.h>
#include <crtl/crtl_log.h>
#include <crtl/crtl_timer.h>
#include <crtl/crtl_task.h>


void timer_callback(void * arg)
{
    crtl_timer_id_t *timerid = (crtl_timer_id_t *)arg;
    crtl_print_info("timeout callback timerid %ld\n", *timerid);
}

void init_routine()
{
    crtl_print_debug(">>>once. %ld\n", crtl_thread_self());
}

void demo_timer_create_test()
{
    int i;
    crtl_timer_id_t timerid[10];
    for(i=0;i<sizeof(timerid)/sizeof(timerid[0]);i++) {
        timerid[i] = crtl_timer_create(crtl_true, &timer_callback, &timerid[i], (i+1)%3+1, 0);
        crtl_print_info("Create: timerid = %ld(%p)\n", timerid[i],(void*)timerid[i]);
    }
    
    sleep(3);
    for(i=0;i<sizeof(timerid)/sizeof(timerid[0])-1;i++) {
        sleep(1);
        crtl_timer_delete(timerid[i]);
        crtl_print_notice("Delete: timerid = %ld\n", timerid[i]);
    }
    
    while(1) {
        sleep(1);
    }
}


void *demo_timer_create_test_multi_thread_fn(void*arg)
{
    crtl_print_debug("Rong. %ld\n", crtl_thread_self());

    int i;
    crtl_timer_id_t timerid[5] = {0};
    for(i=0;i<sizeof(timerid)/sizeof(timerid[0]);i++) {
        timerid[i] = crtl_timer_create(crtl_true, &timer_callback, &timerid[i], i+1, 0);
        crtl_print_info("Create: timerid = %ld(%lx)(%d)\n", timerid[i], timerid[i],sizeof(crtl_timer_id_t));
    }
    
    sleep(3);
    for(i=0;i<sizeof(timerid)/sizeof(timerid[0]);i++) {
        sleep(1);
        crtl_timer_delete(timerid[i]);
        crtl_print_notice("Delete: timerid = %ld\n", timerid[i]);
    }
    
    while(1) {
        sleep(1);
    }
    return NULL;
}

void demo_timer_create_test_multi_thread()
{
    crtl_thread_t thread1, thread2, thread3;
    crtl_thread_normal(&thread1, demo_timer_create_test_multi_thread_fn, NULL);
    crtl_thread_normal(&thread2, demo_timer_create_test_multi_thread_fn, NULL);
    crtl_thread_normal(&thread3, demo_timer_create_test_multi_thread_fn, NULL);


    sleep(13);
    int i;
    crtl_timer_id_t timerid[5] = {0};
    for(i=0;i<sizeof(timerid)/sizeof(timerid[0]);i++) {
        timerid[i] = crtl_timer_create(crtl_true, &timer_callback, &timerid[i], i+1, 0);
        crtl_print_info("Create: timerid = %ld(%lx)(%d)\n", timerid[i], timerid[i],sizeof(crtl_timer_id_t));
    }
    sleep(13);
    for(i=0;i<sizeof(timerid)/sizeof(timerid[0]);i++) {
        sleep(1);
        crtl_timer_delete(timerid[i]);
        crtl_print_notice("Delete: timerid = %ld\n", timerid[i]);
    }
    

    while(1) {
        sleep(1);
//        crtl_print_info(">>>>>>>>>>>>>>>\n");
    }
}

int main()
{
//    demo_timer_create_test();
    demo_timer_create_test_multi_thread();

    
    
    return 0;
}


