#include "crtl/log.h"
#include "crtl/assert.h"
#include "crtl/types.h"
#include "crtl/alloc.h"

#include "crtl/msgq.h"

#include "crtl/easy/macro.h"
#include "crtl/task.h"


crtl_mqd_t mqd1, mqd2;


void *demo_msgq_test11(void*arg)
{
    mqd1 = crtl_msgq_create(256, 1024);
    char __buf[1024] = {0};
    int m_prio = 0;
    char rongtao_ack[] = {"RT Ready to send ACK."};

    while(1)
    {
        int recv_size = crtl_msgq_recv(mqd1, __buf, 1024);
        if(recv_size <= 0) {
            crtl_print_err("recv size %d error. m_prio %d\n", recv_size, m_prio);
            crtl_assert_fp(stderr, 0);
            break;
        }
        crtl_print_info("recv %d bytes, m_prio %d, recv_size %d\n", recv_size, m_prio, recv_size);
        
        crtl_msgq_sync_send_ack(__buf, rongtao_ack, sizeof(rongtao_ack));

        crtl_mqd_t testmqd = crtl_msgq_create(256, 1024);
        crtl_print_notice("create mqd %d \n", testmqd);
        sleep(1);
        crtl_msgq_destroy(&testmqd);
    }
    
    while(1)
    {
        sleep(1);
        crtl_mqd_t testmqd = crtl_msgq_create(CRTL_MSGQ_MSGS_MAX, CRTL_MSGQ_MSG_MAX_SIZE);
        crtl_print_notice("create mqd %d \n", testmqd);
        sleep(1);
        crtl_msgq_destroy(&testmqd);
    }

    
    return NULL;
}

void *demo_msgq_test12(void*arg)
{
    mqd2 = crtl_msgq_create(256, 1024);
//
    char rongtao[] = {"RongTao nihao."};
    char buffer[1024] = {0};
    size_t ack_len = 0;
    
    sleep(2);
    while(1)
    {
        sleep(1);
        int send_size = crtl_msgq_sync_send(mqd1, rongtao, sizeof(rongtao), buffer, &ack_len, 2, 0);
        if(send_size <= 0) {
            crtl_print_err("send error. send_size %d\n", send_size);
            crtl_assert_fp(stderr, 0);
            break;
        }
        crtl_print_info("recv ack <%s>. ack_len %d, send_size %d\n", buffer, ack_len, send_size);
        
        crtl_mqd_t testmqd = crtl_msgq_create(CRTL_MSGQ_MSGS_MAX, CRTL_MSGQ_MSG_MAX_SIZE);
        crtl_print_notice("create mqd %d \n", testmqd);
        sleep(1);
        crtl_msgq_destroy(&testmqd);
    }
    
    while(1)
    {
        sleep(1);
        crtl_mqd_t testmqd = crtl_msgq_create(CRTL_MSGQ_MSGS_MAX, CRTL_MSGQ_MSG_MAX_SIZE);
        crtl_print_notice("create mqd %d \n", testmqd);
        sleep(1);
        crtl_msgq_destroy(&testmqd);
    }
    
    return NULL;
}

void *demo_msgq_test13(void*arg)
{
    while(1)
    {
        sleep(1);
        crtl_mqd_t testmqd = crtl_msgq_create(CRTL_MSGQ_MSGS_MAX, CRTL_MSGQ_MSG_MAX_SIZE);
        crtl_print_notice("create mqd %d \n", testmqd);
        sleep(1);
        crtl_msgq_destroy(&testmqd);
    }
    
    return NULL;
}


void demo_msgq_stress_test()
{
    crtl_thread_t thread1, thread2;
    crtl_thread_normal(&thread1, demo_msgq_test11, NULL);
    crtl_thread_normal(&thread2, demo_msgq_test12, NULL);

#define MSGQ_STRESS_TEST(id) crtl_thread_t thread__##id;crtl_thread_normal(&thread__##id, demo_msgq_test13, NULL)
    
    MSGQ_STRESS_TEST(1);
    MSGQ_STRESS_TEST(2);
    MSGQ_STRESS_TEST(3);
    MSGQ_STRESS_TEST(4);
    MSGQ_STRESS_TEST(5);
    MSGQ_STRESS_TEST(6);
    MSGQ_STRESS_TEST(7);

    sleep(10);
    crtl_print_warning("Destroy mqd %d and %d\n", mqd1, mqd2);
    crtl_msgq_destroy(&mqd1);
    crtl_msgq_destroy(&mqd2);

    while(1)
    {
        sleep(1);
        crtl_mqd_t testmqd = crtl_msgq_create(CRTL_MSGQ_MSGS_MAX, CRTL_MSGQ_MSG_MAX_SIZE);
        crtl_print_notice("create mqd %d \n", testmqd);
        sleep(10);
        crtl_msgq_destroy(&testmqd);
    }
}



int main()
{    
    demo_msgq_stress_test();
    
    while(1)
    {
        sleep(1);
    }

    return 0;
}




