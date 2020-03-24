#ifndef __CRTL_TIME_H
#define __CRTL_TIME_H 1


#include "crtl/bits/crtl_time_def.h"


inline int crtl_dsleep(double sleep_sec);

inline int crtl_timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y);

/* 两个timeval时间间隔 */
inline long crtl_timeval_interval_usec(struct timeval *t1, struct timeval *t2);

inline void crtl_gettimeofday_timespec(struct timespec *time);
inline void crtl_gettimeofday_timeval(struct timeval *time);

inline int crtl_timespec_cmp(struct timespec *in, struct timespec *in2);
inline int crtl_timeval_cmp(struct timeval *in, struct timeval *in2);


inline void crtl_timeval_add(struct timeval *in, struct timeval *in2, struct timeval *out);
inline int crtl_timeval_subabs(struct timeval *in1, struct timeval *in2, struct timeval *out);
inline void crtl_timeval_generate(struct timeval *in, long sec, long microsec);


inline void crtl_timespec_add(struct timespec *in, struct timespec *in2, struct timespec *out);
inline int crtl_timespec_subabs(struct timespec *in1, struct timespec *in2, struct timespec *out);
inline void crtl_timespec_generate(struct timespec *in, long sec, long nanosec);


#endif /*<__CRTL_TIME_H>*/

