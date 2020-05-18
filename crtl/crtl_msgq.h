#ifndef __CRTL_MSGQ_H
#define __CRTL_MSGQ_H 1


#include "crtl/bits/crtl_msgq_posix.h"
#include "crtl/bits/crtl_msgq_systemV.h"


/* 生成消息队列名称-字符串 */
#define CRTL_MSGQ_NAME_GEN(str1, str2) __CRTL_MSGQ_NAME_GEN(str1, str2)

#define CRTL_MSGQ_MSGS_MAX        __CRTL_MSGQ_MSGS_MAX
#define CRTL_MSGQ_MSG_MAX_SIZE    __CRTL_MSGQ_MSG_MAX_SIZE




/**
 * create a message queue
 * @param mq_maxmsg: max number of message
 * @param mq_msgsize: max size of message
 * @return error number
 */
_api crtl_mqd_t crtl_msgq_create(long mq_maxmsg, long mq_msgsize);

/**
 * destroy a message queue
 * @param mqd: id of message queue, created by crtl_msgq_create()
 * @return error number
 */
_api int crtl_msgq_destroy(crtl_mqd_t *mqd);


/**
 * send a async message to message queue
 * @param dst_mqd: destination message queue
 * @param m_ptr: message address pointer
 * @param m_len: message size
 * @return CRTL_ERROR if failed, send size if success
 */
_api int crtl_msgq_async_send(crtl_mqd_t dst_mqd, const char *m_ptr, const size_t m_len);


/**
 * send a sync message to message queue
 * @param dst_mqd: destination message queue
 * @param m_ptr: message address pointer
 * @param m_len: message size
 * @param ack: acknowledge message from destination message queue
 * @param ack_len: acknowledge message length
 * @param timeout_sec: get ack message timeout second
 * @param timeout_nanosec: get ack message timeout nanosecond
 * @return CRTL_ERROR if failed, send size if success
 */
_api int crtl_msgq_sync_send(crtl_mqd_t dst_mqd, const char *m_ptr, const size_t m_len, 
                            void *ack, size_t *ack_len, int timeout_sec, long timeout_nanosec);

                            
/**
 * send a sync acknowledge message to message queue
 * @param src_sync_msg: recv message from self message queue
 * @param ack_msg: acknowledge message from destination message queue
 * @param ack_len: acknowledge message length
 * @return CRTL_ERROR if failed, send size if success
 */
_api int crtl_msgq_sync_send_ack(const void *const src_sync_msg, const void *ack_msg, size_t ack_len);


/**
 * receive a message from message queue
 * @param my_mqd: message queue ID
 * @param m_buf: message buffer
 * @param m_len: message size
 * @return CRTL_ERROR if failed, receive size if success
 */
_api int crtl_msgq_recv(crtl_mqd_t my_mqd, char *m_buf, const int m_len);


#endif /*<__CRTL_MSGQ_H>*/


