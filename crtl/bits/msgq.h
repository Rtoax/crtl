#ifndef __CRTL_MSGQ_POSIX_H
#define __CRTL_MSGQ_POSIX_H 1

//Posix
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

#include <time.h>
#include <pthread.h>

#include <signal.h>

#include "crtl/file.h"

#include "crtl/bits/types_basic.h"



typedef mqd_t crtl_mqd_t;
    
typedef     char    crtl_msgq_name_t[128];


#define __CRTL_MSGQ_PRIO_MAX  MQ_PRIO_MAX



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





#endif /*<__CRTL_MSGQ_POSIX_H>*/

