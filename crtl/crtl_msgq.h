#ifndef __CRTL_MSGQ_H
#define __CRTL_MSGQ_H 1


#include "crtl/bits/crtl_msgq_posix.h"
#include "crtl/bits/crtl_msgq_systemV.h"


/* 生成消息队列名称-字符串 */
#define CRTL_MSGQ_NAME_GEN(str1, str2) __CRTL_MSGQ_NAME_GEN(str1, str2)

#define CRTL_MSGQ_MSGS_MAX        __CRTL_MSGQ_MSGS_MAX
#define CRTL_MSGQ_MSG_MAX_SIZE    __CRTL_MSGQ_MSG_MAX_SIZE



_api crtl_mqd_t crtl_msgq_create(long mq_maxmsg, long mq_msgsize);

_api int crtl_msgq_destroy(crtl_mqd_t *mqd);

_api int crtl_msgq_async_send(crtl_mqd_t dst_mqd, const char *m_ptr, const size_t m_len);

_api int crtl_msgq_sync_send(crtl_mqd_t dst_mqd, const char *m_ptr, const size_t m_len, 
                            void *ack, size_t *ack_len, int timeout_sec, long timeout_nanosec);
_api int crtl_msgq_sync_send_ack(const void *const src_sync_msg, const void *ack_msg, size_t ack_len);

_api int crtl_msgq_recv(crtl_mqd_t my_mqd, char *m_buf, const int m_len);


#endif /*<__CRTL_MSGQ_H>*/


