#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <time.h>

#include "crtl/time.h"

#include "crtl/bits/types_basic.h"
#include "crtl/log.h"

#include "crypto/attribute.h"


static const char _unused *month_names[] = {
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"
};

static const char _unused *weekday_names[] = {
	"Sundays", "Mondays", "Tuesdays", "Wednesdays", "Thursdays", "Fridays", "Saturdays"
};
    
/*
 * Check these. And note how it doesn't do the summer-time conversion.
 *
 * In my world, it's always summer, and things are probably a bit off
 * in other ways too.
 */
static const struct {
    const char *name;
    int offset;
    int dst;
} _unused timezone_names[]  = {
    { "IDLW", -12, 0, },    /* International Date Line West */
    { "NT",   -11, 0, },    /* Nome */
    { "CAT",  -10, 0, },    /* Central Alaska */
    { "HST",  -10, 0, },    /* Hawaii Standard */
    { "HDT",  -10, 1, },    /* Hawaii Daylight */
    { "YST",   -9, 0, },    /* Yukon Standard */
    { "YDT",   -9, 1, },    /* Yukon Daylight */
    { "PST",   -8, 0, },    /* Pacific Standard */
    { "PDT",   -8, 1, },    /* Pacific Daylight */
    { "MST",   -7, 0, },    /* Mountain Standard */
    { "MDT",   -7, 1, },    /* Mountain Daylight */
    { "CST",   -6, 0, },    /* Central Standard */
    { "CDT",   -6, 1, },    /* Central Daylight */
    { "EST",   -5, 0, },    /* Eastern Standard */
    { "EDT",   -5, 1, },    /* Eastern Daylight */
    { "AST",   -3, 0, },    /* Atlantic Standard */
    { "ADT",   -3, 1, },    /* Atlantic Daylight */
    { "WAT",   -1, 0, },    /* West Africa */

    { "GMT",    0, 0, },    /* Greenwich Mean */
    { "UTC",    0, 0, },    /* Universal (Coordinated) */
    { "Z",      0, 0, },    /* Zulu, alias for UTC */

    { "WET",    0, 0, },    /* Western European */
    { "BST",    0, 1, },    /* British Summer */
    { "CET",   +1, 0, },    /* Central European */
    { "MET",   +1, 0, },    /* Middle European */
    { "MEWT",  +1, 0, },    /* Middle European Winter */
    { "MEST",  +1, 1, },    /* Middle European Summer */
    { "CEST",  +1, 1, },    /* Central European Summer */
    { "MESZ",  +1, 1, },    /* Middle European Summer */
    { "FWT",   +1, 0, },    /* French Winter */
    { "FST",   +1, 1, },    /* French Summer */
    { "EET",   +2, 0, },    /* Eastern Europe, USSR Zone 1 */
    { "EEST",  +2, 1, },    /* Eastern European Daylight */
    { "WAST",  +7, 0, },    /* West Australian Standard */
    { "WADT",  +7, 1, },    /* West Australian Daylight */
    { "CCT",   +8, 0, },    /* China Coast, USSR Zone 7 */
    { "JST",   +9, 0, },    /* Japan Standard, USSR Zone 8 */
    { "EAST", +10, 0, },    /* Eastern Australian Standard */
    { "EADT", +10, 1, },    /* Eastern Australian Daylight */
    { "GST",  +10, 0, },    /* Guam Standard, USSR Zone 9 */
    { "NZT",  +12, 0, },    /* New Zealand */
    { "NZST", +12, 0, },    /* New Zealand Standard */
    { "NZDT", +12, 1, },    /* New Zealand Daylight */
    { "IDLE", +12, 0, },    /* International Date Line East */
};



/* double second sleep */
_api inline int crtl_dsleep (double sleep_sec)
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

_api inline int crtl_delayed_exit_thread(int seconds)
{
  struct timespec delay = { seconds, 0 };
  struct timespec remaining = { 0 };
  if (nanosleep (&delay, &remaining) != 0)
    crtl_print_err("nanosleep: %d seconds.\n", seconds);
  /* Exit the process sucessfully.  */
  exit (0);
}

_api inline int crtl_timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
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
	
	return x->tv_sec > y->tv_sec;
}


/* 两个timeval时间间隔 */
_api inline long crtl_timeval_interval_usec(t_timeval *t1, t_timeval *t2)
{
    long _t1uSec = t1->tv_sec*1000000000 + t1->tv_usec;
    long _t2uSec = t2->tv_sec*1000000000 + t2->tv_usec;
    
    return labs(_t1uSec-_t2uSec);
}

_api inline void crtl_gettimeofday_timespec(struct timespec *time)
{
    struct timeval __currenttv;
    /* 获取当前时间TV */
    gettimeofday(&__currenttv,NULL);
    /* 获取当前时间 */
    time->tv_sec = __currenttv.tv_sec;
    time->tv_nsec = 1000 * (__currenttv.tv_usec);
}


_api inline void crtl_gettimeofday_timeval(struct timeval *time)
{
    gettimeofday(time,NULL);
}


_api inline int crtl_timeval_cmp(struct timeval *in, struct timeval *in2)
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


_api inline int crtl_timespec_cmp(struct timespec *in, struct timespec *in2)
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

_api inline void crtl_timespec_add(struct timespec *in, struct timespec *in2, struct timespec *out)
{
    out->tv_sec = in->tv_sec + in2->tv_sec + (in->tv_nsec+in2->tv_nsec)/1000000000;
	out->tv_nsec = (in->tv_nsec+in2->tv_nsec)%1000000000;
}

#define __CRTL_NSEC_PER_SEC 1000000000ULL




_api long long crtl_timespec_sub_nsec(struct timespec a, struct timespec b)
{
	long long ret = __CRTL_NSEC_PER_SEC * b.tv_sec + b.tv_nsec;

	ret -= __CRTL_NSEC_PER_SEC * a.tv_sec + a.tv_nsec;
	return ret;
}

_api struct timespec crtl_timespec_add_nsec(struct timespec ts, unsigned long long ns)
{
	ts.tv_nsec += ns;
	while (ts.tv_nsec >= __CRTL_NSEC_PER_SEC) {
		ts.tv_nsec -= __CRTL_NSEC_PER_SEC;
		ts.tv_sec++;
	}
	return ts;
}



_api inline void crtl_timeval_add(struct timeval *in, struct timeval *in2, struct timeval *out)
{
    out->tv_sec = in->tv_sec + in2->tv_sec + (in->tv_usec+in2->tv_usec)/1000000;
	out->tv_usec = (in->tv_usec+in2->tv_usec)%1000000;
}

_api inline int crtl_timespec_subabs(struct timespec *in1, struct timespec *in2, struct timespec *out)
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

_api inline int crtl_timeval_subabs(struct timeval *in1, struct timeval *in2, struct timeval *out)
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


_api inline void crtl_timespec_generate(struct timespec *in, long sec, long nanosec)
{
    in->tv_sec    = sec + nanosec/1000000000;
    in->tv_nsec   = nanosec%1000000000;
}
_api inline void crtl_timeval_generate(struct timeval *in, long sec, long microsec)
{
    in->tv_sec    = sec + microsec/1000000;
    in->tv_usec   = microsec%1000000;
}


/*
 * This is like mktime, but without normalization of tm_wday and tm_yday.
 */
_api time_t crtl_tm_to_time_t(const struct tm *tm)
{
	static const int mdays[] = {
	    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
	};
	int year = tm->tm_year - 70;
	int month = tm->tm_mon;
	int day = tm->tm_mday;

	if (year < 0 || year > 129) /* algo only works for 1970-2099 */
		return -1;
	if (month < 0 || month > 11) /* array bounds */
		return -1;
	if (month < 2 || (year + 2) % 4)
		day--;
	if (tm->tm_hour < 0 || tm->tm_min < 0 || tm->tm_sec < 0)
		return -1;
	return (year * 365 + (year + 1) / 4 + mdays[month] + day) * 24*60*60UL +
		tm->tm_hour * 60*60 + tm->tm_min * 60 + tm->tm_sec;
}



_api int crtl_is_date(int year, int month, int day, struct tm *now_tm, time_t now, struct tm *tm)
{
	if (month > 0 && month < 13 && day > 0 && day < 32) {
		struct tm check = *tm;
		struct tm *r = (now_tm ? &check : tm);
		time_t specified;

		r->tm_mon = month - 1;
		r->tm_mday = day;
		if (year == -1) {
			if (!now_tm)
				return 1;
			r->tm_year = now_tm->tm_year;
		}
		else if (year >= 1970 && year < 2100)
			r->tm_year = year - 1900;
		else if (year > 70 && year < 100)
			r->tm_year = year;
		else if (year < 38)
			r->tm_year = year + 100;
		else
			return 0;
		if (!now_tm)
			return 1;

		specified = crtl_tm_to_time_t(r);

		/* Be it commit time or author time, it does not make
		 * sense to specify timestamp way into the future.  Make
		 * sure it is not later than ten days from now...
		 */
		if ((specified != -1) && (now + 10*24*3600 < specified))
			return 0;
		tm->tm_mon = r->tm_mon;
		tm->tm_mday = r->tm_mday;
		if (year != -1)
			tm->tm_year = r->tm_year;
		return 1;
	}
	return 0;
}


/*
 * Relative time update (eg "2 days ago").  If we haven't set the time
 * yet, we need to set it from current time.
 */
_api time_t crtl_update_tm(struct tm *tm, struct tm *now, time_t sec)
{
	time_t n;

	if (tm->tm_mday < 0)
		tm->tm_mday = now->tm_mday;
	if (tm->tm_mon < 0)
		tm->tm_mon = now->tm_mon;
	if (tm->tm_year < 0) {
		tm->tm_year = now->tm_year;
		if (tm->tm_mon > now->tm_mon)
			tm->tm_year--;
	}

	n = mktime(tm) - sec;
	localtime_r(&n, tm);
	return n;
}


_api float crtl_cpusecond()
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

_api float crtl_cputime()
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



_api inline uint64_t crtl_asm_rdtsc(void)
{/*   <RT comment: 2019.09.29>*/
    union {
        uint64_t tsc_64;
        struct {
            uint32_t lo_32;
            uint32_t hi_32;
        };
    } tsc;
    /* RDTSC 将时间标签计数器读入 EDX:EAX  <RT comment: 2019.09.29>*/
    asm volatile("rdtsc" :
             "=a" (tsc.lo_32),
             "=d" (tsc.hi_32));
    return tsc.tsc_64;
}

