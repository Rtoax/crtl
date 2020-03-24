#include <crtl/crtl_tree.h>
#include <crtl/crtl_log.h>
#include <crtl/crtl_timer.h>
#include <crtl/crtl_task.h>


void timer_callback(void * arg)
{
    long *timerid = (long *)arg;
    crtl_print_info("timeout timerid %ld\n", *timerid);
}

void init_routine()
{
    crtl_print_debug(">>>once. %ld\n", crtl_thread_self());
}

void demo_timer_create_test()
{
    int i;
    long timerid[10];
    for(i=0;i<sizeof(timerid)/sizeof(timerid[0]);i++) {
        timerid[i] = crtl_timer_create(crtl_false, &timer_callback, &timerid[i], i+1, 0);
        crtl_print_info("timerid = %ld\n", timerid[i]);
    }

    sleep(13);
    for(i=0;i<sizeof(timerid)/sizeof(timerid[0]);i++) {
        timerid[i] = crtl_timer_create(crtl_false, &timer_callback, &timerid[i], i+1, 0);
        crtl_print_info("timerid = %ld\n", timerid[i]);
    }
    
    while(1) {
        sleep(1);
//        crtl_print_info(">>>>>>>>>>>>>>>\n");
    }
}

void *demo_timer_create_test_multi_thread_fn(void*arg)
{
    crtl_print_debug("Rong. %ld\n", crtl_thread_self());

    int i;
    long timerid[30];
    for(i=0;i<sizeof(timerid)/sizeof(timerid[0]);i++) {
        timerid[i] = crtl_timer_create(i%2, &timer_callback, &timerid[i], i+1, 0);
        crtl_print_info("timerid = %ld\n", timerid[i]);
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


