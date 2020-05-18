#ifndef __CRTL_TIMER_H
#define __CRTL_TIMER_H 1


#include "crtl/bits/crtl_timer_def.h"
#include "crtl/easy/macro.h"


typedef __crtl_timer_id_t crtl_timer_id_t;


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
_api crtl_timer_id_t crtl_timer_create(int is_loop, void (*callback)(void *arg), void *arg, long sec, long nanosec);

/* 获取定时器时间 */
/**
 * get timer's timeout interval
 * @param timerid: crtl_timer_id_t timer id
 * @param sec: second of timeout
 * @param nanosec: nano second of timeout
 * @return error number
 */
_api int crtl_timer_gettime_interval(crtl_timer_id_t timerid, long *sec, long *nanosec);

/* 更新定时器时间 */
/**
 * set timer's timeout interval
 * @param timerid: crtl_timer_id_t timer id
 * @param sec: second of timeout
 * @param nanosec: nano second of timeout
 * @return error number
 */
_api int crtl_timer_settime_interval(crtl_timer_id_t timerid, long sec, long nanosec);

/* 更新定时器 */
/**
 * set timer's loop type to none
 * @param timerid: crtl_timer_id_t timer id
 * @return error number
 */
_api int crtl_timer_set_nonloop(crtl_timer_id_t timerid);

/* 删除定时器 */
/**
 * destroy a timer
 * @param timerid: crtl_timer_id_t timer id
 * @return error number
 */
_api int crtl_timer_delete(crtl_timer_id_t timerid);


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
_api int crtl_timerfd_create(int is_loop, __crtl_timer_cb_fn_t callback, void *arg, int sec, int nsec);
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
_api int crtl_timerfd_ctrl(int timerfd, int request/*RT_TIMER_CTRL_XXX*/, ...);

/* 删除fd定时器 */
/**
 * destroy a file descriptor class timer
 * @param timerfd: timer file descriptor
 * @return error number
 */
_api int crtl_timerfd_delete(int timerfd);

/* 删除所有fd定时器 */
/**
 * destroy all file descriptor class timers
 * @param timerfd: timer file descriptor
 * @return error number
 */
_api int crtl_timerfds_destroy();


#endif /*<__CRTL_TIMER_H>*/


