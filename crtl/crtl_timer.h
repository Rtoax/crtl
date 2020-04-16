#ifndef __CRTL_TIMER_H
#define __CRTL_TIMER_H 1


#include "crtl/bits/crtl_timer_def.h"
#include "crtl/easy/macro.h"


typedef __crtl_timer_id_t crtl_timer_id_t;


/* 一类定时器 **********************************************************************************************************/

/* 创建定时器 */
_api crtl_timer_id_t crtl_timer_create(int is_loop, void (*callback)(void *arg), void *arg, long sec, long nanosec);

/* 获取定时器时间 */
_api int crtl_timer_gettime_interval(crtl_timer_id_t timerid, long *sec, long *nanosec);

/* 更新定时器时间 */
_api int crtl_timer_settime_interval(crtl_timer_id_t timerid, long sec, long nanosec);

/* 更新定时器 */
_api int crtl_timer_set_nonloop(crtl_timer_id_t timerid);

/* 删除定时器 */
_api int crtl_timer_delete(crtl_timer_id_t timerid);


/* 二类定时器 **********************************************************************************************************/

/* 创建fd定时器 */
_api int crtl_timerfd_create(int is_loop, __crtl_timer_cb_fn_t callback, void *arg, int sec, int nsec);

/* 控制fd定时器时间 */
_api int crtl_timerfd_ctrl(int timerfd, int request/*RT_TIMER_CTRL_XXX*/, ...);

/* 删除fd定时器 */
_api int crtl_timerfd_delete(int timerfd);

/* 删除所有fd定时器 */
_api int crtl_timerfds_destroy();


#endif /*<__CRTL_TIMER_H>*/


