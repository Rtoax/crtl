#include "crtl/bits/msgq.h"
#include "crtl/log.h"
#include "crtl/assert.h"

#include "crtl/bits/types_basic.h"
#include "crtl/time.h"
#include "crtl/easy/macro.h"


crtl_mqd_t crtl_mq_open(const char *name, int oflag, mode_t mode, long mq_flags, long mq_maxmsg, long mq_msgsize)
{
    if(unlikely(name == NULL)) { 
        crtl_print_err("null pointer error. %s\n");
        return CRTL_ERROR;
    }
    crtl_mqd_t mqd = 0;
    
    struct mq_attr attr;

    attr.mq_flags = mq_flags;
    attr.mq_maxmsg = mq_maxmsg;
    attr.mq_msgsize = mq_msgsize;
	attr.mq_curmsgs = 0;
    
    if(-1 == (mqd_t)(mqd = mq_open(name, oflag, mode, &attr))) {
        crtl_print_err("mq_open error. name %s,mqd %d, oflag 0x%x, mode 0x%x, error %s\n", name,mqd,oflag,mode, CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return mqd;
}


crtl_mqd_t crtl_mq_open_blk(const char *name, long mq_maxmsg, long mq_msgsize)
{
    return crtl_mq_open(name, CRTL_O_MSGQ, CRTL_MSGQ_MODE, 0/* BLOCK */, mq_maxmsg, mq_msgsize);
}

crtl_mqd_t crtl_mq_open_nonblk(const char *name, long mq_maxmsg, long mq_msgsize)
{
    return crtl_mq_open(name, CRTL_O_MSGQ, CRTL_MSGQ_MODE, O_NONBLOCK /* O_NONBLOCK  */, mq_maxmsg, mq_msgsize);
}

int crtl_mq_close(crtl_mqd_t mqd)
{   
    if(0 != mq_close(mqd)) {
        crtl_print_err("mq_close error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}
int crtl_mq_unlink(const char *__name)
{   
    if(0 != mq_unlink(__name)) {
        crtl_print_err("mq_unlink error. %s\n", CRTL_SYS_ERROR);
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}



int crtl_mq_getattr(crtl_mqd_t mqd, long *mq_flags, long *mq_maxmsg, long *mq_msgsize, long *mq_curmsgs)
{
    struct mq_attr attr;
    if(0 != mq_getattr(mqd, &attr)) {
        crtl_print_err("mq_getattr error. %s\n", CRTL_SYS_ERROR);
        
        *mq_flags = 0;
        *mq_maxmsg = 0;
        *mq_msgsize = 0;
        *mq_curmsgs = 0;
        
        return CRTL_ERROR;
    }
    *mq_flags = attr.mq_flags;
    *mq_maxmsg = attr.mq_maxmsg;
    *mq_msgsize = attr.mq_msgsize;
    *mq_curmsgs = attr.mq_curmsgs;
    
    return CRTL_SUCCESS;
}



int crtl_mq_setattr(crtl_mqd_t mqd, long mq_flags, long mq_maxmsg, long mq_msgsize)
{
    struct mq_attr attr, old_attr;

    attr.mq_flags = mq_flags;
    attr.mq_maxmsg = mq_maxmsg;
    attr.mq_msgsize = mq_msgsize;
    
    if(0 != mq_setattr(mqd, &attr, &old_attr)) {
        crtl_print_err("mq_setattr error. %s\n", CRTL_SYS_ERROR);        
        return CRTL_ERROR;
    }    
    return CRTL_SUCCESS;
}




int crtl_mq_setattr_nonblk(crtl_mqd_t mqd, long mq_maxmsg, long mq_msgsize)
{
    struct mq_attr attr;
    
    if(CRTL_SUCCESS != crtl_mq_getattr(mqd, &attr.mq_flags, &attr.mq_maxmsg, &attr.mq_msgsize, &attr.mq_curmsgs)) {
        crtl_print_err("crtl_mq_getattr error. %s\n", CRTL_SYS_ERROR);  
    }
    
    attr.mq_maxmsg = mq_maxmsg;
    attr.mq_msgsize = mq_msgsize;
    
    if(CRTL_SUCCESS != crtl_mq_setattr(mqd, O_NONBLOCK, attr.mq_maxmsg, attr.mq_msgsize)) {
        crtl_print_err("crtl_mq_setattr error. %s\n", CRTL_SYS_ERROR);  
    }
    return CRTL_SUCCESS;
}

//SIGEV_NONE
//SIGEV_SIGNAL
//SIGEV_THREAD

int crtl_mq_notify(crtl_mqd_t mqd, int _sigev_notify, int _sigev_signo, union sigval _sigev_value,
                        void (*_sigev_notify_function)(union sigval), void *_sigev_notify_attributes)
{
    struct sigevent sigevt;
    sigevt.sigev_notify = _sigev_notify;
    sigevt.sigev_signo = _sigev_signo;
    sigevt.sigev_value = _sigev_value;
    sigevt.sigev_notify_function = _sigev_notify_function;
    sigevt.sigev_notify_attributes = _sigev_notify_attributes;

    if(0 != mq_notify(mqd, &sigevt)) {
        crtl_print_err("mq_notify error. %s\n", CRTL_SYS_ERROR);        
        return CRTL_ERROR;
    } 
    return CRTL_SUCCESS;
}

               
                        
int crtl_mq_notify_none(crtl_mqd_t mqd, int _sigev_signo, union sigval _sigev_value,void (*_sigev_notify_function)(union sigval))
{
    return crtl_mq_notify(mqd, SIGEV_NONE, _sigev_signo, _sigev_value, _sigev_notify_function, NULL);
}                        
int crtl_mq_notify_signal(crtl_mqd_t mqd, int _sigev_signo, union sigval _sigev_value,void (*_sigev_notify_function)(union sigval))
{
    return crtl_mq_notify(mqd, SIGEV_SIGNAL, _sigev_signo, _sigev_value, _sigev_notify_function, NULL);
}                       
int crtl_mq_notify_thread(crtl_mqd_t mqd, int _sigev_signo, union sigval _sigev_value,void (*_sigev_notify_function)(union sigval))
{
    return crtl_mq_notify(mqd, SIGEV_THREAD, _sigev_signo, _sigev_value, _sigev_notify_function, NULL);
}



ssize_t crtl_mq_receive(crtl_mqd_t mqd, char *m_ptr, size_t m_len, unsigned int *m_prio, int timedrecv, int seconds, long nanoseconds)
{
    ssize_t m_recv_size = 0;
    
    __crtl_dbg("RECV: mqd %d, O_NONBLOCK %d\n", mqd, O_NONBLOCK);
    
#ifdef __USE_XOPEN2K
    if (timedrecv) {
        __crtl_dbg("TIMED RECV: mqd %d\n", mqd);
        //struct timespec {
        //    time_t tv_sec;      /* Seconds */
        //    long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
        //};
        #if 1
        struct timeval tv;
        gettimeofday(&tv,NULL);
        struct timespec timespec = {0,0};
		timespec.tv_sec = tv.tv_sec + seconds;
		timespec.tv_nsec = 1000 * (tv.tv_usec) + nanoseconds;
        #else
        struct timespec timespec = {seconds, nanoseconds};
        #endif
        if(-1 == (m_recv_size = mq_timedreceive(mqd,m_ptr,m_len,m_prio, &timespec))) {
            crtl_print_err("mq_timedreceive error. mqd %d, error %s, ETIMEDOUT %d(%d)\n", mqd, CRTL_SYS_ERROR, ETIMEDOUT, m_recv_size);
            return CRTL_ERROR;
        }
    } else 
#endif //  __USE_XOPEN2K  
    {
        if(-1 == (m_recv_size = mq_receive(mqd,m_ptr,m_len,m_prio))) {
            crtl_print_err("mq_receive error. mqd %d %s, E(%d)\n", mqd, CRTL_SYS_ERROR, m_recv_size);
            return CRTL_ERROR;
        }
    }
    return m_recv_size;
}


ssize_t crtl_mq_send(crtl_mqd_t mqd, const char *m_ptr, size_t m_len, unsigned int m_prio, int timedsend, int seconds, long nanoseconds)
{
    ssize_t m_send_size = 0;

    __crtl_dbg("SEND: mqd %d, \n", mqd);
    
#ifdef __USE_XOPEN2K
    if (timedsend) {
        //struct timespec {
        //    time_t tv_sec;      /* Seconds */
        //    long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
        //};
        struct timespec timespec = {seconds, nanoseconds};
        if((0 != mq_timedsend(mqd,m_ptr,m_len,m_prio, &timespec))) {
            crtl_print_err("mq_timedsend error mqd %d. %s\n", mqd, CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
    } else 
#endif //  __USE_XOPEN2K  
    {
        if((0 != mq_send(mqd,m_ptr,m_len,m_prio))) {
            crtl_print_err("mq_send error mqd %d, send size %d. %s\n", mqd, m_send_size, CRTL_SYS_ERROR);
            return CRTL_ERROR;
        }
    }
    m_send_size = m_len;

    return m_send_size;
}
