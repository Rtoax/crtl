#ifndef __CRTL_TIME_H
#define __CRTL_TIME_H 1

#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/timex.h>

#include "crtl/bits/crtl_time_def.h"


inline int crtl_dsleep (double sleep_sec);
inline int crtl_timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y);


#endif /*<__CRTL_TIME_H>*/

