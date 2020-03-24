#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "crtl/crtl_time.h"
#include "crtl/crtl_types.h"



/* double second sleep */
inline int crtl_dsleep (double sleep_sec)
{
    t_timespec tv;
    /* Construct the timespec from the number of whole seconds... */
    tv.tv_sec = (t_time_t) sleep_sec;
    /* ... and the remainder in nanoseconds. */
    tv.tv_nsec = (long) ((sleep_sec - tv.tv_sec) * 1e+9);
    while (1)
    {
        /* Sleep for the time specified in tv. If interrupted by a
        signal, place the remaining time left to sleep back into tv. */
        int rval = t_nanosleep (&tv, &tv);
        if (rval == 0)
            /* Completed the entire sleep time; all done. */
            return 0;
        else if (errno == EINTR)
            /* Interrupted by a signal. Try again. */
            continue;
        else
            /* Some other error; bail out. */
            return rval;
    }
    return 0;
}

inline int crtl_timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{
	if (x->tv_usec < y->tv_usec) 
	{
		int nsec = (y->tv_usec - x->tv_usec) / 1000000000 + 1;
		y->tv_usec -= 1000000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000000) 
	{
		int nsec = (x->tv_usec - y->tv_usec) / 1000000000;
		y->tv_usec += 1000000000 * nsec;
		y->tv_sec -= nsec;
	}
	
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;
	
	return x->tv_sec < y->tv_sec;
}


/* 两个timeval时间间隔 */
inline long crtl_timeval_interval_usec(t_timeval *t1, t_timeval *t2)
{
    long _t1uSec = t1->tv_sec*1000000000 + t1->tv_usec;
    long _t2uSec = t2->tv_sec*1000000000 + t2->tv_usec;
    
    return labs(_t1uSec-_t2uSec);
}

inline void crtl_gettimeofday_timespec(struct timespec *time)
{
    struct timeval __currenttv;
    /* 获取当前时间TV */
    gettimeofday(&__currenttv,NULL);
    /* 获取当前时间 */
    time->tv_sec = __currenttv.tv_sec;
    time->tv_nsec = 1000 * (__currenttv.tv_usec);
}


inline void crtl_gettimeofday_timeval(struct timeval *time)
{
    gettimeofday(time,NULL);
}


inline int crtl_timeval_cmp(struct timeval *in, struct timeval *in2)
{
    if(in->tv_sec > in2->tv_sec) {
        return CRTL_GT;
    } else if(in->tv_sec == in2->tv_sec) {
        if( in->tv_usec > in2->tv_usec ) {
            return CRTL_GT;
        } else if (in->tv_usec == in2->tv_usec) {
            return CRTL_EQ;
        } else {
            return CRTL_LT;
        }
    } else {
        return CRTL_LT;
    } 
    return CRTL_EQ;
}


inline int crtl_timespec_cmp(struct timespec *in, struct timespec *in2)
{
    if(in->tv_sec > in2->tv_sec) {
        return CRTL_GT;
    } else if(in->tv_sec == in2->tv_sec) {
        if( in->tv_nsec > in2->tv_nsec ) {
            return CRTL_GT;
        } else if (in->tv_nsec == in2->tv_nsec) {
            return CRTL_EQ;
        } else {
            return CRTL_LT;
        }
    } else {
        return CRTL_LT;
    } 
    return CRTL_EQ;
}

inline void crtl_timespec_add(struct timespec *in, struct timespec *in2, struct timespec *out)
{
    out->tv_sec = in->tv_sec + in2->tv_sec + (in->tv_nsec+in2->tv_nsec)/1000000000;
	out->tv_nsec = (in->tv_nsec+in2->tv_nsec)%1000000000;
}
inline void crtl_timeval_add(struct timeval *in, struct timeval *in2, struct timeval *out)
{
    out->tv_sec = in->tv_sec + in2->tv_sec + (in->tv_usec+in2->tv_usec)/1000000;
	out->tv_usec = (in->tv_usec+in2->tv_usec)%1000000;
}

inline int crtl_timespec_subabs(struct timespec *in1, struct timespec *in2, struct timespec *out)
{
    time_t in1Sec = in1->tv_sec;
    time_t in2Sec = in2->tv_sec;
    long in1nanoSec = in1->tv_nsec;
    long in2nanoSec = in2->tv_nsec;

    int cmp = crtl_timespec_cmp(in1, in2);
    if(cmp == CRTL_GT) {
        out->tv_nsec = in1nanoSec+((in1nanoSec<in2nanoSec)?1000000000:0) - in2nanoSec;
        out->tv_sec = in1Sec + ((in1nanoSec<in2nanoSec)?-1:0) - in2Sec;
    } else if(cmp == CRTL_EQ) {
        out->tv_sec = 0;
	    out->tv_nsec = 0;
    } else if(cmp == CRTL_LT) {
        out->tv_nsec = in2nanoSec+((in2nanoSec<in1nanoSec)?1000000000:0) - in1nanoSec;
        out->tv_sec = in2Sec + ((in2nanoSec<in1nanoSec)?-1:0) - in1Sec;
    }
    
    return cmp;
}

inline int crtl_timeval_subabs(struct timeval *in1, struct timeval *in2, struct timeval *out)
{
    time_t in1Sec = in1->tv_sec;
    time_t in2Sec = in2->tv_sec;
    long in1uSec = in1->tv_usec;
    long in2uSec = in2->tv_usec;

    int cmp = crtl_timeval_cmp(in1, in2);
    if(cmp == CRTL_GT) {
        out->tv_usec = in1uSec+((in1uSec<in2uSec)?1000000:0) - in2uSec;
        out->tv_sec = in1Sec + ((in1uSec<in2uSec)?-1:0) - in2Sec;
    } else if(cmp == CRTL_EQ) {
        out->tv_sec = 0;
	    out->tv_usec = 0;
    } else if(cmp == CRTL_LT) {
        out->tv_usec = in2uSec+((in2uSec<in1uSec)?1000000:0) - in1uSec;
        out->tv_sec = in2Sec + ((in2uSec<in1uSec)?-1:0) - in1Sec;
    }
    
    return cmp;
}


inline void crtl_timespec_generate(struct timespec *in, long sec, long nanosec)
{
    in->tv_sec    = sec + nanosec/1000000000;
    in->tv_nsec   = nanosec%1000000000;
}
inline void crtl_timeval_generate(struct timeval *in, long sec, long microsec)
{
    in->tv_sec    = sec + microsec/1000000;
    in->tv_usec   = microsec%1000000;
}


float crtl_cpusecond()
/*****************************************************************************
return cpu time (UNIX user time) in seconds
******************************************************************************
Input:

Returned:	cpu time in seconds
******************************************************************************
Notes:
	Uses system call and include file.
	See also: cputime() in this library.
******************************************************************************
Author:  Dave Hale, Colorado School of Mines, 04/29/89
*****************************************************************************/
{
	struct rusage rusage;
	getrusage(RUSAGE_SELF,&rusage);
	return ((float)((double)(rusage.ru_utime.tv_sec)+
		1.0e-6*(double)(rusage.ru_utime.tv_usec)));
}



#ifndef CLK_TCK
#define CLK_TCK	64
#endif

/* ANSI C book says clock() units are CLK_TCK, but IBM docs say millisecs */
/* under solaris change CLK_TCK to _SC_CLK_TCK */
#if (_IBMR2||__hp9000s800)
 #define CLOCK_UNIT	1000000
#else
 #define CLOCK_UNIT	CLK_TCK
#endif

float crtl_cputime()
/*****************************************************************************
return cpu time (UNIX user time) in seconds using ANSI C built-ins
******************************************************************************
Input:

Returned:	cpu time in seconds
******************************************************************************
NOTES:
	Relies on CLK_TCK being defined in time.h per ANSI C.
	See also: cpusec() in this library.
******************************************************************************
Author:		Jack K. Cohen, Colorado School of Mines, 07/27/90
*****************************************************************************/
{
	return clock() / (float) CLOCK_UNIT;
}


