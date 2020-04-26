#ifndef __CRTL_MSGQ_POSIX_H
#define __CRTL_MSGQ_POSIX_H 1

//Posix
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

#include <time.h>
#include <pthread.h>

#include <signal.h>

#include "crtl/crtl_file.h"

#include "crtl/bits/crtl_types_basic.h"
#include "crtl/bits/crtl_types_string.h"



typedef mqd_t crtl_mqd_t;
    
typedef     crtl_str128_t    crtl_msgq_name_t;


#define __CRTL_MSGQ_PRIO_MAX  MQ_PRIO_MAX

#define __CRTL_MSGQ_MSGS_MAX        256
#define __CRTL_MSGQ_MSG_MAX_SIZE    1024


#define __CRTL_MSGQ_NAME_GEN(str1, str2) ({\
    char __mqname[1024] = {0};char tempfile[256] = {0};\
    sprintf(__mqname, "/crtl_mq_%s_%s_%s", str1, str2, crtl_mktemp_string(tempfile, "", ""));\
    __mqname;\
})


crtl_mqd_t crtl_mq_open(const char *name, int oflag, mode_t mode, long mq_flags, long mq_maxmsg, long mq_msgsize);
crtl_mqd_t crtl_mq_open_blk(const char *name, long mq_maxmsg, long mq_msgsize);
crtl_mqd_t crtl_mq_open_nonblk(const char *name, long mq_maxmsg, long mq_msgsize);


int crtl_mq_close(crtl_mqd_t mqd);
int crtl_mq_unlink(const char *__name);


int crtl_mq_getattr(crtl_mqd_t mqd, long *mq_flags, long *mq_maxmsg, long *mq_msgsize, long *mq_curmsgs);
int crtl_mq_setattr(crtl_mqd_t mqd, long mq_flags, long mq_maxmsg, long mq_msgsize);
int crtl_mq_setattr_nonblk(crtl_mqd_t mqd, long mq_maxmsg, long mq_msgsize);


int crtl_mq_notify(crtl_mqd_t mqd, int _sigev_notify, int _sigev_signo, union sigval _sigev_value,
                        void (*_sigev_notify_function)(union sigval), void *_sigev_notify_attributes);
int crtl_mq_notify_none(crtl_mqd_t mqd, int _sigev_signo, union sigval _sigev_value,void (*_sigev_notify_function)(union sigval));
int crtl_mq_notify_signal(crtl_mqd_t mqd, int _sigev_signo, union sigval _sigev_value,void (*_sigev_notify_function)(union sigval));
int crtl_mq_notify_thread(crtl_mqd_t mqd, int _sigev_signo, union sigval _sigev_value,void (*_sigev_notify_function)(union sigval));

ssize_t crtl_mq_receive(crtl_mqd_t mqd, char *m_ptr, size_t m_len, unsigned int *m_prio, int timedrecv, int seconds, long nanoseconds);
ssize_t crtl_mq_send(crtl_mqd_t mqd, const char *m_ptr, size_t m_len, unsigned int m_prio, int timedsend, int seconds, long nanoseconds);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MQUEUE_H

/* Establish connection between a process and a message queue NAME and
   return message queue descriptor or (mqd_t) -1 on error.  OFLAG determines
   the type of access used.  If O_CREAT is on OFLAG, the third argument is
   taken as a `mode_t', the mode of the created message queue, and the fourth
   argument is taken as `struct mq_attr *', pointer to message queue
   attributes.  If the fourth argument is NULL, default attributes are
   used.  */
//    S_IRWXU
//           00700 允许 文件 的 属主 读 , 写 和 执行 文件
//    S_IRUSR (S_IREAD)
//           00400 允许 文件 的 属主 读 文件
//    S_IWUSR (S_IWRITE)
//           00200 允许 文件 的 属主 写 文件
//    S_IXUSR (S_IEXEC)
//           00100 允许 文件 的 属主 执行 文件
//    S_IRWXG
//           00070 允许 文件 所在 的 分组 读 , 写 和 执行 文件
//    S_IRGRP
//           00040 允许 文件 所在 的 分组 读 文件
//    S_IWGRP
//           00020 允许 文件 所在 的 分组 写 文件
//    S_IXGRP
//           00010 允许 文件 所在 的 分组 执行 文件
//    S_IRWXO
//           00007 允许 其他 用户 读 , 写 和 执行 文件
//    S_IROTH
//           00004 允许 其他 用户 读 文件
//    S_IWOTH
//           00002 允许 其他 用户 写 文件
//    S_IXOTH
//           00001 允许 其他 用户 执行 文件
extern mqd_t mq_open (const char *__name, int __oflag, ...);
extern mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);

/* Removes the association between message queue descriptor MQDES and its
   message queue.  */
extern int mq_close (mqd_t __mqdes);

/* Query status and attributes of message queue MQDES.  */
struct mq_attr {
   long mq_flags;       /* Flags: 0 or O_NONBLOCK */
   long mq_maxmsg;      /* Max. # of messages on queue */
   long mq_msgsize;     /* Max. message size (bytes) */
   long mq_curmsgs;     /* # of messages currently in queue */
};

extern int mq_getattr (mqd_t __mqdes, struct mq_attr *__mqstat);

/* Set attributes associated with message queue MQDES and if OMQSTAT is
   not NULL also query its old attributes.  */
extern int mq_setattr (mqd_t __mqdes,
		       const struct mq_attr *__restrict __mqstat,
		       struct mq_attr *__restrict __omqstat);

/* Remove message queue named NAME.  */
extern int mq_unlink (const char *__name);

/* Register notification issued upon message arrival to an empty
   message queue MQDES.  */
union sigval {          /* Data passed with notification */
    int     sival_int;         /* Integer value */
    void   *sival_ptr;         /* Pointer value */
};

struct sigevent {
    int          sigev_notify; /* Notification method */
    int          sigev_signo;  /* Notification signal */
    union sigval sigev_value;  /* Data passed with notification */
    void       (*sigev_notify_function) (union sigval);
    /* Function used for thread notification (SIGEV_THREAD) */
    void        *sigev_notify_attributes;
    /* Attributes for notification thread (SIGEV_THREAD) */
    pid_t        sigev_notify_thread_id;
    /* ID of thread to signal (SIGEV_THREAD_ID) */
};

extern int mq_notify (mqd_t __mqdes, const struct sigevent *__notification);

#ifdef ______TEST_MQ_notify_rongtao

#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <cstring>

const char *mq_name = "/mq_test";


inline void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}


static void tfunc(union sigval sv) {
    struct mq_attr attr;
    ssize_t nr;
    void *buf;
    mqd_t mqdes = *((mqd_t *) sv.sival_ptr);

    if (mq_getattr(mqdes, &attr) == -1) {
        handle_error("mq_getattr() error\n");
    }

    buf = malloc(attr.mq_msgsize);
    if (buf == nullptr) {
        handle_error("malloc() error\n");
    }

    nr = mq_receive(mqdes, (char *) buf, attr.mq_msgsize, nullptr);
    if (nr == -1) {
        handle_error("mq_receive() error\n");
    }

    printf("Read %zd bytes from MQ\n", nr);
    free(buf);
    exit(EXIT_SUCCESS);
}


int main() {
    mqd_t mqdes = mq_open(mq_name, O_CREAT | O_RDWR, 0777, nullptr);
    if (mqdes < (mqd_t) 0) {
        handle_error("mq_open() error\n");
    }

    struct sigevent sev;
    bzero(&sev, sizeof(sev));
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = tfunc;
    sev.sigev_notify_attributes = nullptr;
    sev.sigev_value.sival_ptr = &mqdes;  // 传递给tfunc的参数

    if (mq_notify(mqdes, &sev) == -1) {
        handle_error("mq_notify() error\n");
    }
    puts("sleep for 2 second...");
    sleep(2);

    const char *msg = "hello world!";
    mq_send(mqdes, msg, strlen(msg), 1);
    pause();

    exit(EXIT_SUCCESS);
}

#endif

/* Receive the oldest from highest priority messages in message queue
   MQDES.  */
extern ssize_t mq_receive (mqd_t __mqdes, char *__msg_ptr, size_t __msg_len,
			   unsigned int *__msg_prio);

/* Add message pointed by MSG_PTR to message queue MQDES.  */
//如果msg_prio不为NULL，则将其指向的缓冲区用于返回与接收到的消息关联的优先级。
extern int mq_send (mqd_t __mqdes, const char *__msg_ptr, size_t __msg_len,
		    unsigned int __msg_prio);

#ifdef __USE_XOPEN2K
/* Receive the oldest from highest priority messages in message queue
   MQDES, stop waiting if ABS_TIMEOUT expires.  */
extern ssize_t mq_timedreceive (mqd_t __mqdes, char *__restrict __msg_ptr,
				size_t __msg_len,
				unsigned int *__restrict __msg_prio,
				const struct timespec *__restrict __abs_timeout);

/* Add message pointed by MSG_PTR to message queue MQDES, stop blocking
   on full message queue if ABS_TIMEOUT expires.  */
extern int mq_timedsend (mqd_t __mqdes, const char *__msg_ptr,
			 size_t __msg_len, unsigned int __msg_prio,
			 const struct timespec *__abs_timeout);
#endif

#endif//_MQUEUE_H



#endif /*<__CRTL_MSGQ_POSIX_H>*/

