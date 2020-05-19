#ifndef __CRTL_BITS_TIME_DEF_H
#define __CRTL_BITS_TIME_DEF_H 1



#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/timex.h>


#define T_CLOCKS_PER_SEC    CLOCKS_PER_SEC
#define T_CLK_TCK           CLK_TCK

#ifndef _BITS_TIME_H
struct timeval
{
    __time_t tv_sec;		/* Seconds.  */
    __suseconds_t tv_usec;	/* Microseconds.  */
};
#endif//_BITS_TIME_H

#ifndef _SYS_TIME_H
struct itimerval
{
    /* Value to put into `it_value' when the timer expires.  */
    struct timeval it_interval;
    /* Time to the next timer expiration.  */
    struct timeval it_value;
};
#endif //_SYS_TIME_H

#ifdef __itimerspec_defined_rongtao
/* POSIX.1b structure for timer start values and intervals.  */
struct itimerspec {
   struct timespec it_interval;  /* Interval for periodic timer */
   struct timespec it_value;     /* Initial expiration */
};

struct timespec {
   time_t tv_sec;                /* Seconds */
   long   tv_nsec;               /* Nanoseconds */
};

/* ISO C `broken-down time' structure.  */
struct tm
{
  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year	- 1900.  */
  int tm_wday;			/* Day of week.	[0-6] */
  int tm_yday;			/* Days in year.[0-365]	*/
  int tm_isdst;			/* DST.		[-1/0/1]*/

# ifdef	__USE_MISC
  long int tm_gmtoff;		/* Seconds east of UTC.  */
  const char *tm_zone;		/* Timezone abbreviation.  */
# else
  long int __tm_gmtoff;		/* Seconds east of UTC.  */
  const char *__tm_zone;	/* Timezone abbreviation.  */
# endif
};



#endif//__itimerspec_defined_rongtao


typedef struct timeval t_timeval; /* tv_sec, tv_usec */
typedef struct timespec t_timespec; /* tv_sec, tv_usec */
typedef struct tms t_tms; /* tms_utime,tms_stime, tms_cutime, tms_cstime */
typedef struct timezone t_timezone; /* int tz_minuteswest, int tz_dsttime */
typedef struct tm t_tm; /* tm_sec,tm_min,tm_hour,tm_mday,tm_mon,tm_year,tm_wday,tm_yday,tm_isdst,tm_gmtoff,char *tm_zone */
typedef struct ntptimeval t_ntptimeval; /* struct timeval time, long int maxerror esterror */
typedef struct timex t_timex; /* unsigned int modes,long int offset frequency maxerror esterror, int status ... */
typedef struct itimerval t_itimerval; /* struct timeval it_interval, it_value */


typedef clock_t t_clock_t;
typedef time_t t_time_t; /* long int */


#define t_difftime(time1, time0) difftime(time1, time0)
#define t_clock() clock()
#define t_times(buffer) times(buffer)
#define t_time(result) time(result) /* time_t time (time t *result) */
#define t_stime(newtime) stime(newtime) /* int stime (time t *newtime) */
#define t_gettimeofday(tp, tzp) gettimeofday(tp, tzp) /* int gettimeofday (struct timeval *tp, struct timezone *tzp) */
#define t_settimeofday(tp, tzp) settimeofday(tp, tzp) /* int settimeofday (const struct timeval *tp, const struct timezone *tzp) */
#define t_localtime(time) localtime(time) /* struct tm * localtime(const time_t *time) */
#define t_localtime_r(time, resultp) localtime_r(time, resultp) /* struct tm * localtime_r (const time t *time, struct tm *resultp) */
#define t_gmtime(time) gmtime(time) /* struct tm * gmtime (const time_t *time) */
#define t_gmtime_r(time, resultp) gmtime_r(time, resultp) /* struct tm * gmtime_r (const time t *time, struct tm *resultp) */
#define t_mktime(brokentime) mktime(brokentime) /* time_t mktime (struct tm *brokentime) */
#define t_timelocal(brokentime) timelocal(brokentime) /* time_t timelocal (struct tm *brokentime) */
#define t_timegm(brokentime) timegm(brokentime) /* time_t timegm (struct tm *brokentime) */
#define t_ntp_gettime(tptr) ntp_gettime(tptr) /* int ntp_gettime (struct ntptimeval *tptr) */
#define t_ntp_adjtime(tptr) ntp_adjtime(tptr) /* int ntp_adjtime (struct timex *tptr) */
#define t_asctime(brokentime) asctime(brokentime) /* char * asctime (const struct tm *brokentime) */
#define t_asctime_r(brokentime, buffer) asctime_r(brokentime, buffer) /*char * asctime_r (const struct tm *brokentime, char *buffer)*/
#define t_ctime(ctime) ctime(ctime) /*char * ctime (const time t *time)*/
#define t_ctime_r(time, buffer) ctime_r(time, buffer) /*char * ctime_r (const time t *time, char *buffer)*/
#define t_strftime(s, size, template, brokentime) strftime(s, size, template, brokentime)/*size_t strftime (char *s, size t size, const char *template, const struct tm *brokentime)*/
/** for example
	buf[0] = ’\1’;
	len = strftime (buf, bufsize, format, tp);
	if (len == 0 && buf[0] != ’\0’)
	{
		// Something went wrong in the strftime call. 
		...
	}
*/

#define t_wcsftime(s, size, template, brokentime) wcsftime(s, size, template, brokentime)/*size_t wcsftime (wchar t *s, size t size, const wchar t *template, const struct tm *brokentime)*/
#define t_strptime(s, fmt, tp) strptime(s, fmt, tp) /*char * strptime (const char *s, const char *fmt, struct tm *tp)*/
/**
	const char *
	parse_date (const char *input, struct tm *tm)
	{
		const char *cp;
		// First clear the result structure. 
		memset (tm, ’\0’, sizeof (*tm));
		// Try the ISO format first. 
		cp = strptime (input, "%F", tm);
		if (cp == NULL)
		{
			// Does not match. Try the US form. 
			cp = strptime (input, "%D", tm);
		}
		return cp;
	}
*/
#define t_getdate(string) getdate(string) /*struct tm * getdate (const char *string)*/
/**
	getdate_err [Variable]
	This variable of type int contains the error code of the last unsuccessful call to
	getdate. Defined values are:
		1 The environment variable DATEMSK is not defined or null.
		2 The template file denoted by the DATEMSK environment variable cannot be opened.
		3 Information about the template file cannot retrieved.
		4 The template file is not a regular file.
		5 An I/O error occurred while reading the template file.
		6 Not enough memory available to execute the function.
		7 The template file contains no matching template.
		8 The input date is invalid, but would match a template otherwise. This
			includes dates like February 31st, and dates which cannot be represented
			in a time_t variable.
*/

#define t_getdate_r(string, tp) getdate_r(string, tp) /*int getdate_r (const char *string, struct tm *tp)*/

#define t_tzset() tzset() /*void tzset (void)*/

#define t_setitimer(which, new, old) setitimer(which, new, old) /*int setitimer (int which, struct itimerval *new, struct itimerval *old)*/

#define t_getitimer(which, old) getitimer(which, old) /*int getitimer (int which, struct itimerval *old)*/
/**
	The return value and error conditions are the same as for setitimer.
	ITIMER_REAL: This constant can be used as the which argument to the setitimer and
					getitimer functions to specify the real-time timer.
	ITIMER_VIRTUAL: This constant can be used as the which argument to the setitimer and
					getitimer functions to specify the virtual timer.
	ITIMER_PROF: This constant can be used as the which argument to the setitimer and
					getitimer functions to specify the profiling timer.
*/

#define t_alarm(seconds) alarm(seconds) /*unsigned int alarm (unsigned int seconds)*/
#define t_alarm2(seconds) 	\
	do{ 					\
	t_itimerval old, new; 	\
	new.it_interval.tv_usec = 0;	\
	new.it_interval.tv_sec = 0;		\
	new.it_value.tv_usec = 0;		\
	new.it_value.tv_sec = (long int) seconds;		\
	if (t_setitimer (ITIMER_REAL, &new, &old) < 0){	\
		return 0;	\
	}else{	\
		return old.it_value.tv_sec;}	\
	}while(0)
/**
	unsigned int
	alarm (unsigned int seconds)
	{
		struct itimerval old, new;
		
		new.it_interval.tv_usec = 0;
		new.it_interval.tv_sec = 0;
		new.it_value.tv_usec = 0;
		new.it_value.tv_sec = (long int) seconds;
		
		if (setitimer (ITIMER_REAL, &new, &old) < 0)
			return 0;
		else
			return old.it_value.tv_sec;
	}
*/

#define t_sleep(seconds) sleep(seconds) /* unsigned int sleep (unsigned int seconds) */
#define t_nanosleep(requested_time, remaining) nanosleep(requested_time, remaining) /*int nanosleep (const struct timespec *requested_time, struct timespec *remaining)*/

#endif /*<__CRTL_BITS_TIME_DEF_H>*/
