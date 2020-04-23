#include "crtl/crtl_log.h"
#include "crtl/crtl_task.h"

#include "crtl/bits/crtl_sm.h"

enum {
    TEST_STATE_1 = 1,
    TEST_STATE_2,
    TEST_STATE_3,
    TEST_STATE_4,
    TEST_STATE_5,
    TEST_STATE_6,
};
enum {
    TEST_EVENT_1 = 1,
    TEST_EVENT_2,
    TEST_EVENT_3,
    TEST_EVENT_4,
    TEST_EVENT_5,
};
    
static int sm_argc = 2;
static char * sm_argv[] = {
    "rong",
    "tao",    
};

int sm_handler(const crtl_sm_state_t curr_state, const crtl_sm_event_t event, const crtl_sm_state_t next_state, int argc, char *argv[])
{
    crtl_print_info("%ld, %ld, %ld\n", curr_state, event, next_state);
    int i;
    for(i=0;i<argc;i++) {
        crtl_print_info("%s\n", argv[i]);
    }
    return 0;
}


crtl_sm_t sm_test1;

void *demo_multithread_sm_register_task1(void*arg)
{
    
    crtl_sm_state_register(sm_test1, TEST_STATE_2);
    crtl_sm_state_register(sm_test1, TEST_STATE_3);
    crtl_sm_state_register(sm_test1, TEST_STATE_4);

    
    sleep(2);
    
    crtl_sm_handler_register(sm_test1, TEST_STATE_2, TEST_EVENT_2, TEST_STATE_3, &sm_handler, sm_argc, sm_argv);
    crtl_sm_handler_register(sm_test1, TEST_STATE_3, TEST_EVENT_3, TEST_STATE_4, &sm_handler, sm_argc, sm_argv);
    crtl_sm_handler_register(sm_test1, TEST_STATE_4, TEST_EVENT_4, TEST_STATE_5, &sm_handler, sm_argc, sm_argv);
    crtl_sm_handler_register(sm_test1, TEST_STATE_5, TEST_EVENT_5, TEST_STATE_6, &sm_handler, sm_argc, sm_argv);
    
    return NULL;
}

void *demo_multithread_sm_register_task2(void*arg)
{
    sleep(1);
    crtl_sm_state_register(sm_test1, TEST_STATE_5);
    crtl_sm_state_register(sm_test1, TEST_STATE_6);

    
    sleep(1);
    
    crtl_sm_handler_register(sm_test1, TEST_STATE_1, TEST_EVENT_1, TEST_STATE_2, &sm_handler, sm_argc, sm_argv);
    crtl_sm_handler_register(sm_test1, TEST_STATE_1, TEST_EVENT_2, TEST_STATE_2, &sm_handler, sm_argc, sm_argv);
    crtl_sm_handler_register(sm_test1, TEST_STATE_1, TEST_EVENT_3, TEST_STATE_2, &sm_handler, sm_argc, sm_argv);
    crtl_sm_handler_register(sm_test1, TEST_STATE_1, TEST_EVENT_4, TEST_STATE_3, &sm_handler, sm_argc, sm_argv);

    
    return NULL;
}

void demo_sm_test1()
{
    
    crtl_thread_t thread, thread2;

    sm_test1 = crtl_sm_init(TEST_STATE_1);
    crtl_print_info("%ld\n", sm_test1);
    
    crtl_thread_normal(&thread, demo_multithread_sm_register_task1, NULL);
    crtl_thread_normal(&thread2, demo_multithread_sm_register_task2, NULL);

    sleep(3);


    
    crtl_sm_exec(sm_test1, TEST_EVENT_1);
    crtl_sm_exec(sm_test1, TEST_EVENT_2);
    crtl_sm_exec(sm_test1, TEST_EVENT_3);
    crtl_sm_exec(sm_test1, TEST_EVENT_4);
    crtl_sm_exec(sm_test1, TEST_EVENT_5);

    crtl_sm_destroy(sm_test1);
    
}

int main()
{
    demo_sm_test1();

    
    return 0;
}


