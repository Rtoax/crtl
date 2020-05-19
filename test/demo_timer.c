#include <string.h>
#include <crtl/tree.h>
#include <crtl/log.h>
#include <crtl/timer.h>
#include <crtl/crtl_task.h>



/////////////////////////////////////////////////////////////////////////////////////////////TEST
/////////////////////////////////////////////////////////////////////////////////////////////TEST
/////////////////////////////////////////////////////////////////////////////////////////////TEST
/////////////////////////////////////////////////////////////////////////////////////////////TEST
/////////////////////////////////////////////////////////////////////////////////////////////TEST

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
        timerid[i] = crtl_timer_create(crtl_true, &timer_callback, &timerid[i], 1, 0);
        crtl_print_info("Create: timerid = %ld(%p)\n", timerid[i],(void*)timerid[i]);
    }
    
    sleep(3);
    for(i=0;i<sizeof(timerid)/sizeof(timerid[0]);i++) {
        crtl_timer_settime_interval(timerid[i], 2, 0);
    }
    sleep(10);
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


/////////////////////////////////////////////////////////////////////////////////////////////TEST
/////////////////////////////////////////////////////////////////////////////////////////////TEST
/////////////////////////////////////////////////////////////////////////////////////////////TEST
/////////////////////////////////////////////////////////////////////////////////////////////TEST
/////////////////////////////////////////////////////////////////////////////////////////////TEST
static void demo_timerfd_timer_callback(void *arg)
{
    crtl_print_warning("callback. %s\n", (char*)arg);
}

#define demo_timerfd_create(timerfd, cb, loop, sec, nsec) \
    int _unused timerfd = crtl_timerfd_create(loop, &cb, #timerfd, sec, nsec)

static void _unused demo_timerfd_demo1()
{

    demo_timerfd_create(t1, demo_timerfd_timer_callback, true, 1, 1);
    demo_timerfd_create(t2, demo_timerfd_timer_callback, true, 1, 1);
    demo_timerfd_create(t3, demo_timerfd_timer_callback, false, 1, 1);
    demo_timerfd_create(t4, demo_timerfd_timer_callback, true, 1, 1);
    
    sleep(4);
    crtl_print_warning("add new one.\n");
    demo_timerfd_create(t5, demo_timerfd_timer_callback, true, 1, 1);
    demo_timerfd_create(t6, demo_timerfd_timer_callback, false, 1, 1);
    

    sleep(4);
    crtl_timerfd_delete(t4);
    
    crtl_print_warning("add new one done.\n");
    while(1)sleep(1);

}
static void _unused demo_timerfd_demo2()
{

    demo_timerfd_create(t1, demo_timerfd_timer_callback, true, 1, 1);
    demo_timerfd_create(t2, demo_timerfd_timer_callback, true, 1, 1);
    demo_timerfd_create(t3, demo_timerfd_timer_callback, false, 2, 1);
    demo_timerfd_create(t4, demo_timerfd_timer_callback, true, 1, 1);


    sleep(3);
    crtl_print_warning("delete t2\n");
    crtl_timerfd_delete(t2);
    crtl_print_warning("delete t2 done\n");
    
    sleep(5);
//    crtl_print_warning("destroy\n");
    crtl_timerfds_destroy();
    
    demo_timerfd_create(t5, demo_timerfd_timer_callback, true, 1, 1);
    while(1)
        sleep(1);
    
}

static void _unused demo_timerfd_demo3()
{

    demo_timerfd_create(t1, demo_timerfd_timer_callback, true, 1, 1);
    demo_timerfd_create(t2, demo_timerfd_timer_callback, true, 1, 1);

    crtl_print_warning("timerfd = %d\n", t1);
    crtl_print_warning("timerfd = %d\n", t2);


    sleep(2);
    
    int loop_type = 0;
    crtl_timerfd_ctrl(t1, TIMER_GET_LOOPTYPE, &loop_type);
    crtl_print_warning("looptype = %ld\n", loop_type);
    crtl_timerfd_ctrl(t1, TIMER_SET_LOOPTYPE, CRTL_TIMER_NONLOOP);

    sleep(2);
    
    struct itimerspec itimerspec = {{2,2}, {3,3}};
    crtl_timerfd_ctrl(t2, TIMER_SET_INTERVAL, itimerspec);
    crtl_print_warning("TIMER_SET_INTERVAL\n");

    memset(&itimerspec, 0x00, sizeof(itimerspec));
    crtl_timerfd_ctrl(t2, TIMER_GET_INTERVAL, &itimerspec);
    crtl_print_warning("get Time Interval: {{%ld,%ld},{%ld,%ld}}\n",
                itimerspec.it_value.tv_sec,
                itimerspec.it_value.tv_nsec,
                itimerspec.it_interval.tv_sec,
                itimerspec.it_interval.tv_nsec);

//    char name[20];
//    crtl_timerfd_ctrl(t2, TIMER_GET_NAME, name);
//    crtl_print_warning("name  %s\n", name);
//    crtl_timerfd_ctrl(t2, TIMER_SET_NAME, "rongtao");
//    
//    crtl_timerfd_ctrl(t2, TIMER_GET_NAME, name);
//    crtl_print_warning("name  %s\n", name);
                
    while(1)sleep(1);
    
}

void timerfd_callback(void * arg)
{
    int *timerfd = (int *)arg;
    crtl_print_info("timeout callback timerfd %d\n", *timerfd);
}


void *demo_timerfd_create_test_multi_thread_fn(void*arg)
{
    crtl_print_debug("Rong. %ld\n", crtl_thread_self());

    int i;
    int timerfd[5] = {0};
    for(i=0;i<sizeof(timerfd)/sizeof(timerfd[0]);i++) {
        timerfd[i] = crtl_timerfd_create(crtl_true, &timerfd_callback, &timerfd[i], i+1, 0);
        crtl_print_info("Create: timerid = %d(%x)(%d)\n", timerfd[i], timerfd[i],sizeof(int));
    }
    
    sleep(3);
    for(i=0;i<sizeof(timerfd)/sizeof(timerfd[0]);i++) {
        sleep(1);
        crtl_timerfd_delete(timerfd[i]);
        crtl_print_notice("Delete: timerid = %d\n", timerfd[i]);
    }
    
    while(1) {
        sleep(1);
    }
    return NULL;
}

void demo_timerfd_create_test_multi_thread()
{
    crtl_thread_t thread1, thread2, thread3, thread4;
    crtl_thread_normal(&thread1, demo_timerfd_create_test_multi_thread_fn, NULL);
        sleep(1);
    crtl_thread_normal(&thread2, demo_timerfd_create_test_multi_thread_fn, NULL);
        sleep(1);
    crtl_thread_normal(&thread3, demo_timerfd_create_test_multi_thread_fn, NULL);
        sleep(1);
    crtl_thread_normal(&thread4, demo_timerfd_create_test_multi_thread_fn, NULL);


    sleep(13);
//    crtl_timerfds_destroy();
    
    int i;
    int timerfd[5] = {0};
    for(i=0;i<sizeof(timerfd)/sizeof(timerfd[0]);i++) {
        timerfd[i] = crtl_timerfd_create(crtl_true, &timerfd_callback, &timerfd[i], i+1, 0);
        crtl_print_info("Create: timerid = %d(%lx)(%d)\n", timerfd[i], timerfd[i],sizeof(int));
    }
    sleep(13);
    for(i=0;i<sizeof(timerfd)/sizeof(timerfd[0]);i++) {
        sleep(1);
        crtl_timerfd_delete(timerfd[i]);
        crtl_print_notice("Delete: timerid = %d\n", timerfd[i]);
    }
    
    
        sleep(1);
    crtl_thread_normal(&thread2, demo_timerfd_create_test_multi_thread_fn, NULL);
        sleep(1);
    crtl_thread_normal(&thread4, demo_timerfd_create_test_multi_thread_fn, NULL);
    
    sleep(13);
    for(i=0;i<sizeof(timerfd)/sizeof(timerfd[0]);i++) {
        sleep(1);
        crtl_timerfd_delete(timerfd[i]);
        crtl_print_notice("Delete: timerid = %d\n", timerfd[i]);
    }

    
    while(1) {
        sleep(1);
//        crtl_print_info(">>>>>>>>>>>>>>>\n");
    }
}


int main()
{
//    demo_timer_create_test();
//    demo_timer_create_test_multi_thread();
    
//    demo_timerfd_demo1();
//    demo_timerfd_demo2();
//    demo_timerfd_demo3();
    demo_timerfd_create_test_multi_thread();
    
    return 0;
}


