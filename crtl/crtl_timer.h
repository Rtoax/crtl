#ifndef __CRTL_TIMER_H
#define __CRTL_TIMER_H 1


#include "crtl/bits/crtl_timer_def.h"
#include "crtl/easy/macro.h"


typedef __crtl_timer_id_t crtl_timer_id_t;


/* ������ʱ�� */
_api crtl_timer_id_t crtl_timer_create(int is_loop, void (*callback)(void *arg), void *arg, long sec, long nanosec);

/* ��ȡ��ʱ��ʱ�� */
_api int crtl_timer_gettime_interval(crtl_timer_id_t timerid, long *sec, long *nanosec);

/* ���¶�ʱ��ʱ�� */
_api int crtl_timer_settime_interval(crtl_timer_id_t timerid, long sec, long nanosec);

/* ���¶�ʱ�� */
_api int crtl_timer_nonloop(crtl_timer_id_t timerid);

/* ɾ����ʱ�� */
_api int crtl_timer_delete(crtl_timer_id_t timerid);



#endif /*<__CRTL_TIMER_H>*/


