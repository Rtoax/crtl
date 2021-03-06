#ifndef __CRTL_BITS_TIMER_DEF_H
#define __CRTL_BITS_TIMER_DEF_H 1

#include <signal.h>
#include <sys/timerfd.h>

#include "crtl/timer.h"

#include "crypto/list/list.h"



/* type define */
typedef void (* __crtl_timer_cb_fn_t)(void *arg);


/* callback of timer */
typedef struct __rt_timer_cb_type {
    __crtl_timer_cb_fn_t fn;
    void *arg;
}__rt_timer_cb_t;


/* update frequency */
#define __CRTL_TIMER_REFRESH_FREQUENCY_NanoSEC     crtl_timer_interval_nanosec /* nanosec */

extern volatile unsigned long int crtl_timer_interval_nanosec;


/* 定时器结构体 */
struct crtl_timer_struct {
    union {
        crtl_timer_id_t timer_id;
        int timer_fd; /* for #include <sys/timerfd.h> */
        timer_t timerid; //POSIX api
    };

    crtl_timer_loop_t timer_loop;
    
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





#endif /*<__CRTL_BITS_TIMER_DEF_H>*/


