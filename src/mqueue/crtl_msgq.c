
#include "crtl/log.h"
#include "crtl/assert.h"
#include "crtl/bits/types_basic.h"
#include "crtl/file.h"
#include "crtl/alloc.h"
#include "crtl/tree.h"

#include "crtl/bits/msgq.h"

#include "crtl/bits/rwlock.h"

#include "crtl/easy/macro.h"

#include "crtl/msgq.h"

#include "crtl_mute_dbg.h" //去除所有debug打印

#include "crypto/atomic/cas.h"


/* 同步消息队列消息中-临时消息队列传递的消息结构 */
struct __crtl_msgq_sync_msg_body {
    crtl_mqd_t __ack_snd_mqd;
    size_t msg_size;
    char msg_body[0];   /* 实际传递的消息体 */
#define __CRTL_MSGQ_SYNC_TMP_MSGQ_MAXMSG    1 /* 临时消息队列中只允许有一个消息 */
#define __CRTL_MSGQ_SYNC_TMP_MSGQ_MAXMSGSIZE    1024
};

/* 消息队列存放树的节点结构体 */
struct __crtl_msgq_tree_node {
    crtl_mqd_t mqd;
    crtl_msgq_name_t mqname;
};


/* 消息队列列表的读写锁 */
static crtl_lock_rw_t _unused __crtl_msgqs_tree_rwlock = CRTL_LOCK_RWLOCK_INITIALIZER;
#define __crtl_msgqs_rdlock crtl_rwlock_rdlock(&__crtl_msgqs_tree_rwlock, 0,0,0,0)
#define __crtl_msgqs_wrlock crtl_rwlock_wrlock(&__crtl_msgqs_tree_rwlock, 0,0,0,0)
#define __crtl_msgqs_unlock crtl_rwlock_unlock(&__crtl_msgqs_tree_rwlock)

/* 消息队列列表是否初始化标识位 */
static volatile long _unused __crtl_msgqs_tree_init_flag = 0;

/* 消息队列列表 */
static crtl_rbtree_t _unused __crtl_msgqs_tree = NULL;  //所有消息队列（除了临时消息队列）__crtl_msgq_tree_node

/* 消息队列存放树的节点结构体-结构体比较 */
inline static int _unused __crtl_msgq_node_cmp(const void *data1, const void *data2)
{
    struct __crtl_msgq_tree_node *msgq1 = (struct __crtl_msgq_tree_node *)data1;
    struct __crtl_msgq_tree_node *msgq2 = (struct __crtl_msgq_tree_node *)data2;
    
    if(msgq1->mqd > msgq2->mqd) return CRTL_GT;
    else if(msgq1->mqd == msgq2->mqd) return CRTL_EQ;
    else if(msgq1->mqd < msgq2->mqd) return CRTL_LT;
    else return CRTL_EQ;
}

/* 获取msgq , 线程不安全 */
static struct __crtl_msgq_tree_node _unused* __crtl_msgq_getmsgqbymqd(crtl_mqd_t mqd)
{
    crtl_rbtree_node_t rbtree_node = NULL;
    
    struct __crtl_msgq_tree_node msgq;
    memset(&msgq, 0, sizeof(struct __crtl_msgq_tree_node));
    msgq.mqd = mqd;

    rbtree_node = crtl_rbtree_search(__crtl_msgqs_tree, &msgq);
    if(!rbtree_node) {
        return NULL;
    }
    
    return crtl_rbtree_node_data(rbtree_node);    
}


/* 创建消息队列 */
_api crtl_mqd_t crtl_msgq_create(long mq_maxmsg, long mq_msgsize)
{
    /* 入参有误 */
    if(unlikely(!mq_maxmsg) || unlikely(!mq_msgsize)) {
        crtl_print_err("wrong params error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    /* 当保存所有msgq信息的标志被设定，查找是否冲突- 此处好像不用判断 */
    if(CAS(&__crtl_msgqs_tree_init_flag, 1, 1)) {
        __crtl_dbg("__crtl_msgqs_tree_init_flag = %ld.\n", __crtl_msgqs_tree_init_flag);
        
    }

    __crtl_msgqs_wrlock;
    
    /* 当保存所有msgq信息的标志未被设定，初始化这个树 */
    if(CAS(&__crtl_msgqs_tree_init_flag, 0, 1)) {
        __crtl_dbg("__crtl_msgqs_tree_init_flag = %ld.\n", __crtl_msgqs_tree_init_flag);
        
        /* 初始化红黑树 */
        __crtl_msgqs_tree = crtl_rbtree_init(&__crtl_msgq_node_cmp);
        crtl_assert_fp(stderr, __crtl_msgqs_tree);
        if(!__crtl_msgqs_tree) {/* 初始化失败 */
            crtl_print_err("crtl_rbtree_init error.\n");
            crtl_assert_fp(stderr, 0);
            __crtl_msgqs_unlock;
            return CRTL_ERROR;
        }
        
    }
    __crtl_msgqs_unlock;
    
    struct __crtl_msgq_tree_node *this_msgq = crtl_malloc1(1, sizeof(struct __crtl_msgq_tree_node));
    if(unlikely(!this_msgq)) {
        /* 申请失败，退出 */
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    strcpy(this_msgq->mqname, __CRTL_MSGQ_NAME_GEN("allmsgq", "tree"));

    __crtl_dbg("CREATE Msgq: %s\n", this_msgq->mqname);
    
    crtl_mqd_t this_mqd = crtl_mq_open_blk(this_msgq->mqname, mq_maxmsg, mq_msgsize);;
    if((int)this_mqd == CRTL_ERROR) {
        crtl_print_err("create msgq error.\n");
        crtl_mq_unlink(this_msgq->mqname);
        crtl_mfree1(this_msgq);
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    };
    
    this_msgq->mqd = this_mqd;

    __crtl_msgqs_wrlock;
    
    /* 将msgq添加至msgq树中 */
    int ret = crtl_rbtree_insert(__crtl_msgqs_tree, this_msgq, sizeof(struct __crtl_msgq_tree_node));
    if(ret != CRTL_SUCCESS) {
        __crtl_msgqs_unlock;
        crtl_print_err("insert msgq %d,%s to msgqtree error.\n", this_msgq->mqd, this_msgq->mqname);
        
        crtl_mq_close(this_msgq->mqd);
        crtl_mq_unlink(this_msgq->mqname);
        crtl_mfree1(this_msgq);
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    __crtl_msgqs_unlock;

    
    return this_mqd;
}


/* 销毁msgq */
_api int crtl_msgq_destroy(crtl_mqd_t *mqd)
{
    /* 监测参数合法性 */
    if(unlikely(!mqd) || unlikely(!(*mqd))) {
        crtl_print_err("params error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    /* 当保存所有mqd信息的标志未被设定，直接返回失败*/
    if(CAS(&__crtl_msgqs_tree_init_flag, 0, 0)) {
        __crtl_dbg("__crtl_msgqs_tree_init_flag = %ld.\n", __crtl_msgqs_tree_init_flag);
        crtl_print_err("none of msgq be founded error.\n");
        return CRTL_ERROR;
    }

    struct __crtl_msgq_tree_node *__msgq = NULL;

    __crtl_msgqs_rdlock;
    
    /* 如果msgq不存在 */
    if(NULL == (__msgq = __crtl_msgq_getmsgqbymqd(*mqd))) {
        __crtl_msgqs_unlock;
        crtl_print_err("msgq not exist mqd=%d.\n", *mqd);
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
        
    /* 如果msgq存在 */
    } else {
        /* 获取消息队列树的写权限 */
        __crtl_msgqs_unlock;
        __crtl_msgqs_wrlock;
        crtl_rbtree_delete(__crtl_msgqs_tree, __msgq);
        
        __crtl_dbg("DELETE Msgq: %d - %s\n", __msgq->mqd, __msgq->mqname);

        *mqd = -1;
        
        crtl_mq_close(__msgq->mqd);
        crtl_mq_unlink(__msgq->mqname);
        crtl_mfree1(__msgq);

        /* 当保存所有msgq信息的标志被设定，查找msgq树是否为空，如果为空，销毁这个树*/
        if(CAS(&__crtl_msgqs_tree_init_flag, 1, 1)) {
            if(crtl_rbtree_is_empty(__crtl_msgqs_tree) == CRTL_SUCCESS) {
                crtl_rbtree_destroy(__crtl_msgqs_tree);
                __crtl_msgqs_tree = NULL;
                CAS(&__crtl_msgqs_tree_init_flag, 1, 0);
                __crtl_dbg("Destroy MsgqTree. __crtl_msgqs_tree_init_flag = %d\n", __crtl_msgqs_tree_init_flag);
            }
        }
    }
    __crtl_msgqs_unlock;
    
    return CRTL_SUCCESS;
}



/* 发送异步消息队列 */
_api int crtl_msgq_async_send(crtl_mqd_t dst_mqd, const char *m_ptr, const size_t m_len)
{
    /* 入参有误 */
    if(unlikely(!m_ptr) || unlikely(!m_len)) {
        crtl_print_err("wrong params error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }

    /* 发送消息队列 */
    int ready_to_send_len = m_len;
    int send_size = crtl_mq_send(dst_mqd, (char*)m_ptr, ready_to_send_len, 10, 0,0,0);
    if(send_size < ready_to_send_len) {
        /* 如果失败，清空消息队列，删除文件，退出 */
        crtl_print_err("send size not equal, need %d, but is %d, error(mqd %d).\n", ready_to_send_len, send_size, dst_mqd);
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    return send_size;
}


/* 发送同步消息队列 */
_api int crtl_msgq_sync_send(crtl_mqd_t dst_mqd, const char *m_ptr, const size_t m_len, 
                            void *ack, size_t *ack_len, int timeout_sec, long timeout_nanosec)
{
    /* 入参有误 */
    if(unlikely(!m_ptr) || unlikely(!m_len) || unlikely(!ack) || unlikely(!ack_len)) {
        crtl_print_err("wrong params error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    /* 为同步消息队列申请内存 */
    struct __crtl_msgq_sync_msg_body _unused *__msgq_sync_msg = crtl_malloc1(1, sizeof(struct __crtl_msgq_sync_msg_body)+m_len);
    if(unlikely(!__msgq_sync_msg)) {
        /* 申请失败，退出 */
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }

    int _unused ret = CRTL_ERROR;
    crtl_msgq_name_t __ack_snd_mq_name;
    
    /* 消息队列名称 */
    sprintf(__ack_snd_mq_name, "%s", __CRTL_MSGQ_NAME_GEN("TMP", "ACK"));

    __crtl_dbg("MSGQ: %s\n", __ack_snd_mq_name);

    /* 创建消息队列 -     非阻塞-需要提供超时机制 */
    __msgq_sync_msg->__ack_snd_mqd = crtl_mq_open_nonblk(__ack_snd_mq_name, 
                                                                     __CRTL_MSGQ_SYNC_TMP_MSGQ_MAXMSG, 
                                                                     __CRTL_MSGQ_SYNC_TMP_MSGQ_MAXMSGSIZE);
    if(__msgq_sync_msg->__ack_snd_mqd == CRTL_ERROR) {
        /* 创建消息队列失败，释放内存，并退出 */
        crtl_print_err("Create tmp msgq error.\n");
        sleep(1);
        crtl_mfree1(__msgq_sync_msg);
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    __crtl_dbg("OPEN: mqd %d, \n", __msgq_sync_msg->__ack_snd_mqd);
    
    /* 创建临时消息队列成功，拷贝需要发送的数据 */
    __msgq_sync_msg->msg_size = m_len;
    memcpy(__msgq_sync_msg->msg_body, m_ptr, m_len);

    __crtl_dbg("SEND: mqd %d, \n", dst_mqd);

    /* 发送消息队列 */
    int ready_to_send_len = sizeof(struct __crtl_msgq_sync_msg_body)+m_len;
    int send_size = crtl_mq_send(dst_mqd, (char*)__msgq_sync_msg, ready_to_send_len, 10, 0,0,0);
    if(send_size < ready_to_send_len) {
        /* 如果失败，清空消息队列，删除文件，退出 */
        crtl_print_err("send size not equal, need %d, but is %d, error(mqd %d).\n", ready_to_send_len, send_size, dst_mqd);
//        crtl_assert_fp(stderr, 0);
        crtl_mq_close(__msgq_sync_msg->__ack_snd_mqd);
        crtl_mq_unlink(__ack_snd_mq_name);
        crtl_mfree1(__msgq_sync_msg);
        return CRTL_ERROR;
    }

    __crtl_dbg("SEND: mqd %d, send_size %d , ready_to_send_len %d\n", dst_mqd, send_size, ready_to_send_len);

    /* 从临时消息队列接收消息（等待消息接收端调用 crtl_msgq_sync_send_ack ） */
    char __buf[__CRTL_MSGQ_SYNC_TMP_MSGQ_MAXMSGSIZE] = {0};
    unsigned int m_prio;
    int recv_ack_size = crtl_mq_receive(__msgq_sync_msg->__ack_snd_mqd, 
                                        __buf, __CRTL_MSGQ_SYNC_TMP_MSGQ_MAXMSGSIZE, 
                                        &m_prio, 1, timeout_sec, timeout_nanosec);
    if(recv_ack_size <= 0) {
        /* 如果接收失败或者超时，关闭临时消息队列，清空临时消息队列文件，释放内存 */
        crtl_print_err("recv ack error: recv %d error.\n", recv_ack_size);
        crtl_assert_fp(stderr, 0);
        crtl_mq_close(__msgq_sync_msg->__ack_snd_mqd);
        crtl_mq_unlink(__ack_snd_mq_name);
        crtl_mfree1(__msgq_sync_msg);
        return CRTL_ERROR;
    }
    
    __crtl_dbg("RECV ACK: mqd %d, ack_len %d\n", __msgq_sync_msg->__ack_snd_mqd, recv_ack_size);

    /* 如果从临时消息队列接收消息成功，向ack赋值， */
    *ack_len = recv_ack_size;
    memcpy(ack, __buf, recv_ack_size);

    __crtl_dbg("RECV ACK: mqd %d, ack_len %d\n", __msgq_sync_msg->__ack_snd_mqd, recv_ack_size);

    /* 整体流程结束，释放资源 */
    __crtl_dbg("MSGQ Close: %s\n", __ack_snd_mq_name);
    crtl_mq_close(__msgq_sync_msg->__ack_snd_mqd);
    crtl_mq_unlink(__ack_snd_mq_name);
    crtl_mfree1(__msgq_sync_msg);
    
    return send_size-sizeof(struct __crtl_msgq_sync_msg_body);
}


/* 发送同步消息队列ack */
_api int crtl_msgq_sync_send_ack(const void *const src_sync_msg, const void *ack_msg, size_t ack_len)
{
    /* 入参有误 */
    if(unlikely(!src_sync_msg) || unlikely(!ack_msg) || unlikely(!ack_len)) {
        crtl_print_err("wrong params error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }

    /* 解析同步消息中的msgq ID */
    struct __crtl_msgq_sync_msg_body *sync_msgq_msg = (struct __crtl_msgq_sync_msg_body *)src_sync_msg;
    crtl_mqd_t __tmp_mqd = sync_msgq_msg->__ack_snd_mqd;
    
    /* 向临时消息队列发送ACK消息 */
    int send_ack_size = crtl_mq_send(__tmp_mqd, (char*)ack_msg, ack_len, 0, 0,0,0);
    if(send_ack_size < ack_len) {
        crtl_print_err("send ack size not equal, need %d, but is %d, error.\n", ack_len, send_ack_size);
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    __crtl_dbg("MSGQ ACK SEND: mqd %d\n", __tmp_mqd);

    return send_ack_size;
}





_api int crtl_msgq_recv(crtl_mqd_t my_mqd, char *m_buf, const int m_len)
{
    /* 入参有误 */
    if(unlikely(!my_mqd) || unlikely(!m_buf) || unlikely(!m_len)) {
        crtl_print_err("wrong params error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    unsigned int m_prio = 0;
    int recv_size = crtl_mq_receive(my_mqd, m_buf, m_len, &m_prio, 0,7,0);
    if(recv_size <= 0) {
        crtl_print_err("recv mqd %d bytes %d, error.\n", my_mqd, recv_size);
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    return recv_size;
}



