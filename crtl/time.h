#ifndef __CRTL_TIME_H
#define __CRTL_TIME_H 1


#include "crtl/bits/time.h"


/**
 * double sleep
 * @param sleep_sec: sleep second
 * @return void
 */
inline int crtl_dsleep(double sleep_sec);

/**
 * sleep and exit
 * @param seconds: sleep second
 * @return void
 */
inline int crtl_delayed_exit_thread(int seconds);
  
/**
 * Uses system call and include file. See also: cputime() in this library.
 * @param void
 * @return cpu time (UNIX user time) in seconds
 * @author Dave Hale, Colorado School of Mines, 04/29/89
 */
float crtl_cpusecond();


/**
 * subtract timeval
 * @param result: resault of subtract
 * @param x: one of timeval
 * @param y: another of timeval
 * @return if second x>y, return value greater than zero, else <= 0
 */
inline int crtl_timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y);

/* 两个timeval时间间隔 */
/**
 * interval of two timeval
 * @param t1: one of timeval
 * @param t2: another of timeval
 * @return useconds of interval
 */
inline long crtl_timeval_interval_usec(struct timeval *t1, struct timeval *t2);

/**
 * gettimeofday
 * @param time: timespec
 * @return void
 */
inline void crtl_gettimeofday_timespec(struct timespec *time);

/**
 * gettimeofday
 * @param time: timeval
 * @return void
 */
inline void crtl_gettimeofday_timeval(struct timeval *time);


/**
 * compare timespec
 * @param in: time 1
 * @param in2: time 2
 * @return if in = in2, return CRTL_EQ
 * @return if in > in2, return CRTL_GT
 * @return if in < in2, return CRTL_LT
 */
inline int crtl_timespec_cmp(struct timespec *in, struct timespec *in2);

/**
 * compare timeval
 * @param in: time 1
 * @param in2: time 2
 * @return if in = in2, return CRTL_EQ
 *         if in > in2, return CRTL_GT
 *         if in < in2, return CRTL_LT
 */
inline int crtl_timeval_cmp(struct timeval *in, struct timeval *in2);


/**
 * add timeval
 * @param in: time 1
 * @param in2: time 2
 * @param out: result
 * @return void
 */
inline void crtl_timeval_add(struct timeval *in, struct timeval *in2, struct timeval *out);

/**
 * subtract timeval into absulute value
 * @param in: time 1
 * @param in2: time 2
 * @param out: result
 * @return if in = in2, return CRTL_EQ
 *         if in > in2, return CRTL_GT
 *         if in < in2, return CRTL_LT
 */
inline int crtl_timeval_subabs(struct timeval *in1, struct timeval *in2, struct timeval *out);

/**
 * generate timeval
 * @param in: timeval
 * @param sec: second
 * @param microsec: micro second
 * @return void
 */
inline void crtl_timeval_generate(struct timeval *in, long sec, long microsec);



/**
 * add timespec
 * @param in: time 1
 * @param in2: time 2
 * @param out: result
 * @return void
 */
inline void crtl_timespec_add(struct timespec *in, struct timespec *in2, struct timespec *out);

/**
 * subtract timespec into absulute value
 * @param in: time 1
 * @param in2: time 2
 * @param out: result
 * @return if in = in2, return CRTL_EQ
 *         if in > in2, return CRTL_GT
 *         if in < in2, return CRTL_LT
 */
inline int crtl_timespec_subabs(struct timespec *in1, struct timespec *in2, struct timespec *out);

/**
 * generate timespec
 * @param in: timeval
 * @param sec: second
 * @param microsec: micro second
 * @return void
 */
inline void crtl_timespec_generate(struct timespec *in, long sec, long nanosec);


/**
 * subtract timespec
 * @param a: timespec
 * @param b: timespec
 * @return nano second of subtraction
 */
long long crtl_timespec_sub_nsec(struct timespec a, struct timespec b);

/**
 * add nano second to timespec
 * @param a: timespec
 * @param b: timespec
 * @return timespec
 */
struct timespec crtl_timespec_add_nsec(struct timespec ts, unsigned long long ns);

/*
 * This is like mktime, but without normalization of tm_wday and tm_yday.
 */
time_t crtl_tm_to_time_t(const struct tm *tm);


int crtl_is_date(int year, int month, int day, struct tm *now_tm, time_t now, struct tm *tm);

/*
 * Relative time update (eg "2 days ago").  If we haven't set the time
 * yet, we need to set it from current time.
 */
time_t crtl_update_tm(struct tm *tm, struct tm *now, time_t sec);


/**
 * RDTSC 将时间标签计数器读入 EDX:EAX
 * @param void
 * @return RDTSC
 */
inline uint64_t crtl_asm_rdtsc(void);


#endif /*<__CRTL_TIME_H>*/

