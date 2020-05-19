#ifndef __CRTL_TIMER_H
#define __CRTL_TIMER_H 1


typedef long crtl_timer_id_t;


/* contants */
typedef enum {
    CRTL_TIMER_LOOP = 1,
    CRTL_TIMER_NONLOOP,    
}crtl_timer_loop_t;

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


/* 一类定时器 **********************************************************************************************************/

/**
 * create a timer
 * @param is_loop: loop timer or not
 * @param callback: callback of timeout
 * @param arg: argument of timeout callback
 * @param sec: second of timeout
 * @param nanosec: nano second of timeout
 * @return crtl_timer_id_t timer id
 */
crtl_timer_id_t crtl_timer_create(int is_loop, void (*callback)(void *arg), void *arg, long sec, long nanosec);

/* 获取定时器时间 */
/**
 * get timer's timeout interval
 * @param timerid: crtl_timer_id_t timer id
 * @param sec: second of timeout
 * @param nanosec: nano second of timeout
 * @return error number
 */
int crtl_timer_gettime_interval(crtl_timer_id_t timerid, long *sec, long *nanosec);

/* 更新定时器时间 */
/**
 * set timer's timeout interval
 * @param timerid: crtl_timer_id_t timer id
 * @param sec: second of timeout
 * @param nanosec: nano second of timeout
 * @return error number
 */
int crtl_timer_settime_interval(crtl_timer_id_t timerid, long sec, long nanosec);

/* 更新定时器 */
/**
 * set timer's loop type to none
 * @param timerid: crtl_timer_id_t timer id
 * @return error number
 */
int crtl_timer_set_nonloop(crtl_timer_id_t timerid);

/* 删除定时器 */
/**
 * destroy a timer
 * @param timerid: crtl_timer_id_t timer id
 * @return error number
 */
int crtl_timer_delete(crtl_timer_id_t timerid);


/* 二类定时器 **********************************************************************************************************/

/* 创建fd定时器 */
/**
 * create a file descriptor class timer
 * @param is_loop: loop timer or not
 * @param callback: callback of timeout
 * @param arg: argument of timeout callback
 * @param sec: second of timeout
 * @param nanosec: nano second of timeout
 * @return timer file descriptor
 */
int crtl_timerfd_create(int is_loop, void (* callback)(void *arg), void *arg, int sec, int nsec);
//#define TIMER_GET_ID            RT_TIMER_CTRL_GETID
//#define TIMER_GET_FD            RT_TIMER_CTRL_GETFD
//#define TIMER_GET_NAME          RT_TIMER_CTRL_GETNAME
//#define TIMER_GET_FN            RT_TIMER_CTRL_GETFN
//#define TIMER_GET_ARG           RT_TIMER_CTRL_GETARG
//#define TIMER_GET_LOOPTYPE      RT_TIMER_CTRL_GETLOOPTYPE
//#define TIMER_GET_NLOOP         RT_TIMER_CTRL_GETNLOOP
//#define TIMER_GET_INTERVAL      RT_TIMER_CTRL_GETINTERVAL
//#define TIMER_SET_ID            RT_TIMER_CTRL_SETID
//#define TIMER_SET_FD            RT_TIMER_CTRL_SETFD
//#define TIMER_SET_NAME          RT_TIMER_CTRL_SETNAME
//#define TIMER_SET_FN            RT_TIMER_CTRL_SETFN
//#define TIMER_SET_ARG           RT_TIMER_CTRL_SETARG
//#define TIMER_SET_LOOPTYPE      RT_TIMER_CTRL_SETLOOPTYPE
//#define TIMER_SET_NLOOP         RT_TIMER_CTRL_SETNLOOP
//#define TIMER_SET_INTERVAL      RT_TIMER_CTRL_SETINTERVAL

/* 控制fd定时器时间 */
/**
 * set a file descriptor class timer's attribute
 * @param timerfd: timer file descriptor
 * @param request: TIMER_GET_XXX or TIMER_SET_XXX
 * @return error number
 *
 *  request and argument list
 *  TIMER_GET_FD            long int *
 *  TIMER_GET_LOOPTYPE      int *       (true or false)
 *  TIMER_GET_INTERVAL      struct itimerspec*
 *  TIMER_SET_LOOPTYPE      struct itimerspec
 */
int crtl_timerfd_ctrl(int timerfd, int request/*RT_TIMER_CTRL_XXX*/, ...);

/* 删除fd定时器 */
/**
 * destroy a file descriptor class timer
 * @param timerfd: timer file descriptor
 * @return error number
 */
int crtl_timerfd_delete(int timerfd);

/* 删除所有fd定时器 */
/**
 * destroy all file descriptor class timers
 * @param timerfd: timer file descriptor
 * @return error number
 */
int crtl_timerfds_destroy();


#endif /*<__CRTL_TIMER_H>*/


