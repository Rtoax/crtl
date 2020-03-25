#ifndef __CRTL_TIMER_H
#define __CRTL_TIMER_H 1


#include "crtl/bits/crtl_timer_def.h"
#include "crtl/easy/macro.h"


typedef __crtl_timer_id_t crtl_timer_id_t;


/* 创建定时器 */
_api crtl_timer_id_t crtl_timer_create(int is_loop, void (*callback)(void *arg), void *arg, long sec, long nanosec);

/* 获取定时器时间 */
_api int crtl_timer_gettime_interval(crtl_timer_id_t timerid, long *sec, long *nanosec);

/* 更新定时器时间 */
_api int crtl_timer_settime_interval(crtl_timer_id_t timerid, long sec, long nanosec);

/* 更新定时器 */
_api int crtl_timer_nonloop(crtl_timer_id_t timerid);

/* 删除定时器 */
_api int crtl_timer_delete(crtl_timer_id_t timerid);



#endif /*<__CRTL_TIMER_H>*/


