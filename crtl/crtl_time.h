#ifndef __CRTL_TIME_H
#define __CRTL_TIME_H 1


#include "crtl/bits/crtl_time_def.h"


inline int crtl_dsleep(double sleep_sec);
inline int crtl_timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y);


#endif /*<__CRTL_TIME_H>*/

