
#include "crtl/bits/crtl_module_types.h"



//struct __crtl_module_main_task_arg {
//    crtl_mqd_t  main_msgqd;//消息队列
//    crtl_thread_start_routine_fn main_task_fn;
//    void *main_task_fn_arg;
//};
//
//void *__crtl_module_recvQueue_task(void *arg)
//{
//
//    return NULL;
//}
//
//
//int crtl_module_recvQueue_create(const char *app_name, const char *module_name, long mq_maxmsg, long mq_msgsize,
//                         int (*recv_cb_fn)(void* msg_body, size_t msg_len))
//{
//    /* 如果app不存在，返回失败 */
//    if(CRTL_SUCCESS != crtl_application_exist(app_name)) {
//        return CRTL_ERROR;
//    }
//    int crtl_module_create(app_name, module_name, mq_maxmsg, mq_msgsize, __crtl_module_recvQueue_task, void *main_task_arg);
//    
//    
//    return CRTL_SUCCESS;
//}

