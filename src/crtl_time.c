#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "crtl_time.h"


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
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000) 
	{
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
	}
	
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;
	
	return x->tv_sec < y->tv_sec;
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


