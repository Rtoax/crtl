#ifndef __CRTL_BITS_TIMER_DEF_H
#define __CRTL_BITS_TIMER_DEF_H 1

#include <signal.h>
#include <sys/timerfd.h>

#include "crtl/bits/crtl_time_def.h"
#include "crtl/crtl_list.h"


#define __CRTL_TIMER_IS_NanoSEC 0


/* contants */
typedef enum {
    CRTL_TIMER_LOOP = 1,
    CRTL_TIMER_NONLOOP,    
}__crtl_timer_loop_t;




/* type define */
typedef void (* __crtl_timer_cb_fn_t)(void *arg);


/* callback of timer */
typedef struct __rt_timer_cb_type {
    __crtl_timer_cb_fn_t fn;
    void *arg;
}__rt_timer_cb_t;

typedef  long __crtl_timer_id_t;

/* update frequency */
#define __CRTL_TIMER_REFRESH_FREQUENCY_NanoSEC     10000000 /* nanosec */

/* 定时器结构体 */
struct crtl_timer_struct {
    union {
        __crtl_timer_id_t timer_id;
        int timer_fd; /* for #include <sys/timerfd.h> */
        timer_t timerid; //POSIX api
    };

    __crtl_timer_loop_t timer_loop;
    
    struct itimerspec itimerspec_value;
    /** 
     *********************************************************************
     *  定时器创建（插入定时器链表）机制
     *  
     *  create(回调函数，时间间隔)
     *      1.获取当前时间 current_time 
     *      2.保存间隔时间 interval_time
     *      3.计算到期时间 arrival_time
     *      3.加入定时器队列
     *          3.1.轮训链表，获取到期时间  
     *          3.2.当到期时间介于轮训的二者之间时，插入该定时器节点
     *  
     *********************************************************************
     *  定时器链表轮训机制
     *  
     *  
     *  
     *********************************************************************
     *  定时器删除机制
     *  
     *  
     *  
     *********************************************************************
     *  定时器修改机制
     *  
     *  
     *  
     *  
     */
    __crtl_timer_cb_fn_t timer_sche_callback;
    void *timer_sche_arg;
    struct list_head list;
};


typedef enum {
    RT_TIMER_CTRL_GETID,
    RT_TIMER_CTRL_GETFD,
    RT_TIMER_CTRL_GETNAME,
    RT_TIMER_CTRL_GETFN,
    RT_TIMER_CTRL_GETARG,
    RT_TIMER_CTRL_GETLOOPTYPE,
    RT_TIMER_CTRL_GETNLOOP,
    RT_TIMER_CTRL_GETINTERVAL,
    RT_TIMER_CTRL_SETID,
    RT_TIMER_CTRL_SETFD,
    RT_TIMER_CTRL_SETNAME,
    RT_TIMER_CTRL_SETFN,
    RT_TIMER_CTRL_SETARG,
    RT_TIMER_CTRL_SETLOOPTYPE,
    RT_TIMER_CTRL_SETNLOOP,
    RT_TIMER_CTRL_SETINTERVAL,
}__rt_timer_ctrl_t;
    
    /* for __rt_rawtimer_ctrl */
#define TIMER_GET_ID            RT_TIMER_CTRL_GETID
#define TIMER_GET_FD            RT_TIMER_CTRL_GETFD
#define TIMER_GET_NAME          RT_TIMER_CTRL_GETNAME
#define TIMER_GET_FN            RT_TIMER_CTRL_GETFN
#define TIMER_GET_ARG           RT_TIMER_CTRL_GETARG
#define TIMER_GET_LOOPTYPE      RT_TIMER_CTRL_GETLOOPTYPE
#define TIMER_GET_NLOOP         RT_TIMER_CTRL_GETNLOOP
#define TIMER_GET_INTERVAL      RT_TIMER_CTRL_GETINTERVAL
#define TIMER_SET_ID            RT_TIMER_CTRL_SETID
#define TIMER_SET_FD            RT_TIMER_CTRL_SETFD
#define TIMER_SET_NAME          RT_TIMER_CTRL_SETNAME
#define TIMER_SET_FN            RT_TIMER_CTRL_SETFN
#define TIMER_SET_ARG           RT_TIMER_CTRL_SETARG
#define TIMER_SET_LOOPTYPE      RT_TIMER_CTRL_SETLOOPTYPE
#define TIMER_SET_NLOOP         RT_TIMER_CTRL_SETNLOOP
#define TIMER_SET_INTERVAL      RT_TIMER_CTRL_SETINTERVAL


#endif /*<__CRTL_BITS_TIMER_DEF_H>*/

