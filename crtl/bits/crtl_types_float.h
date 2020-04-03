#ifndef __RT_MATH_FLOAT_H
#define __RT_MATH_FLOAT_H 1

#include <math.h>


typedef float           FLOAT;
typedef double          DOUBLE;
typedef long double     LDOUBLE;

#ifndef FLT_MAX
#   ifdef MAXFLOAT
#	define FLT_MAX MAXFLOAT
#   else
#	define FLT_MAX 3.402823466E+38F
#   endif
#endif
#ifndef FLT_MIN
#   ifdef MINFLOAT
#	define FLT_MIN MINFLOAT
#   else
#	define FLT_MIN 1.175494351E-38F
#   endif
#endif

#ifndef DBL_MIN
#define DBL_MIN		4.94065645841246544e-324
#endif
#ifndef DBL_MAX
#define DBL_MAX		1.79769313486231470e+308
#endif


/**
 *  There are some old version
 *  Rongtao 2019.07.20
 */
/*	file name: 		float.h
 *	author:			Rong Tao
 *	create time:	2018.11.18
 * 	
 */
#ifndef _T_MATH_FLOAT_H__

#define _T_MATH_FLOAT_H__





/**
 *	float point type function
 *	define macro and typedef.
 *	Author:	Rongtao
 *	Time:	2018.11.18
 */
/*int fpclassify (float-type x)*/
#define t_fpclassify(float_type) fpclassify(float_type)
/*return: FP_NAN, FP_INFINITE, FP_ZERO, FP_SUBNORMAL, FP_NORMAL*/

/*int isfinite (float-type x)*/
#define t_isfinite(float_type) isfinite(float_type)

/*int isnormal (float-type x)*/
#define t_isnormal(float_type) isnormal(float_type)

/*int isnan (float-type x)*/
#define t_isnan(float_type) isnan(float_type)

/*
int isinf (double x)
int isinff (float x)
int isinfl (long double x)
int isnan (double x)
int isnanf (float x)
int isnanl (long double x)
int finite (double x)
int finitef (float x)
int finitel (long double x)
*/
#define t_isinf(x) isinf(x)
#define t_isinff(x) isinff(x)
#define t_isinfl(x) isinfl(x)
//#define t_isnan(x) isnan(x)
#define t_isnanf(x) isnanf(x)
#define t_isnanl(x) isnanl(x)
#define t_finite(x) finite(x)
#define t_finitef(x) finitef(x)
#define t_finitel(x) finitel(x)

/* Some older hardware does not support infinities. */
#define T_HUGE_VAL HUGE_VAL /*double*/
#define T_HUGE_VALF HUGE_VALF /*float*/
#define T_HUGE_VALL HUGE_VALL /*log float*/

/**
 *	Errors in Floating-Point Calculations
 *	just from glic/libc pdf, i just use it for a tools.
 *	Author:	Rongtao
 *	Time:	2018.11.18
 * 
 *	there are some exceptions come from float arithmetic
 *	1.Invalid Operation;
 *	2.Division by Zero;
 *	3.Overflow;
 *	4.Underflow;
 *	5.Inexact;
 */
/*Infinity and NaN*/
#define T_INFINITY INFINITY
#define T_NAN NAN




#endif /*<_T_MATH_FLOAT_H__>*/


#ifndef __RT_MATH_FLOAT_EXCEPTION_H
#define __RT_MATH_FLOAT_EXCEPTION_H 1


/**
 *  There are some old version
 *  Rongtao 2019.07.20
 */
/*	file name: 		fexcepts.h
 *	author:			Rong Tao
 *	create time:	2018.11.18
 * 	
 */
#ifndef _T_MATH_FLOAT_EXCEPT_H__
#define _T_MATH_FLOAT_EXCEPT_H__

#include <math.h>

/*some Examining the FPU status word*/
#include <fenv.h>
#include <stdlib.h>
#include <stdio.h>

#include "crtl/crtl_log.h"

#define T_FE_ALL_EXCEPT FE_ALL_EXCEPT
#define T_FE_INEXACT FE_INEXACT
#define T_FE_DIVBYZERO FE_DIVBYZERO
#define T_FE_UNDERFLOW FE_UNDERFLOW
#define T_FE_OVERFLOW FE_OVERFLOW
#define T_FE_INVALID FE_INVALID


/*int feclearexcept (int excepts)*/
#define t_feclearexcept(excepts) feclearexcept(excepts)
/*int feraiseexcept (int excepts)*/
#define t_feraiseexcept(excepts) feraiseexcept(excepts)
/*int fetestexcept (int excepts)*/
#define t_fetestexcept(excepts) fetestexcept(excepts)
/*int fegetexceptflag (fexcept t *flagp, int excepts)*/
#define t_fegetexceptflag(flagp, excepts) fegetexceptflag(flagp, excepts)
/*int fesetexceptflag (const fexcept t *flagp, int excepts)*/
#define t_fesetexceptflag(flagp, excepts) fesetexceptflag(flagp, excepts)

/*int fegetround (void)*/
#define t_fegetround() fegetround()
/*int fesetround (int round)*/
#define t_fesetround(round) fesetround(round)

/*int fegetenv (fenv t *envp)*/
#define t_fegetenv(envp) fegetenv(envp)
/*int feholdexcept (fenv t *envp)*/
#define t_feholdexcept(envp) feholdexcept(envp)
/*int fesetenv (const fenv t *envp)*/
#define t_fesetenv(envp) fesetenv(envp)
/*int feupdateenv (const fenv t *envp)*/
#define t_feupdateenv(envp) feupdateenv(envp)
/*int feenableexcept (int excepts)*/
#define t_feenableexcept(excepts) feenableexcept(excepts)
/*int fedisableexcept (int excepts)*/
#define t_fedisableexcept(excepts) fedisableexcept(excepts)
/*int fegetexcept (int excepts)*/
#define t_fegetexcept(excepts) fegetexcept(excepts)


/**
 *	just like try catch throw of java or c# etc,
 *	but is not like that.
 *	Author:	Rongtao
 *	Time:	2018.11.18
 */

#define t_float_try \
	t_feclearexcept(T_FE_ALL_EXCEPT); 

#define t_float_catch(excepts) \
	do{\
		if(T_FE_INEXACT&t_fetestexcept (excepts)){\
			log_error("T_FE_INEXACT\n"); \
		}\
		if(T_FE_DIVBYZERO&t_fetestexcept (excepts)){\
			log_error("T_FE_DIVBYZERO\n"); \
		}\
		if(T_FE_UNDERFLOW&t_fetestexcept (excepts)){\
			log_error("T_FE_UNDERFLOW\n"); \
		}\
		if(T_FE_OVERFLOW&t_fetestexcept (excepts)){\
			log_error("T_FE_OVERFLOW\n"); \
		}\
		if(T_FE_INVALID&t_fetestexcept (excepts)){\
			log_error("T_FE_INVALID\n"); \
		}\
	}while(0)
	
#define t_float_catchall(...) t_float_catch(T_FE_ALL_EXCEPT)
#define t_float_throw(excepts) t_feraiseexcept(excepts)

/* use the exception macro like this:
	t_float_try{
	t_float_throw(T_FE_OVERFLOW);
	}t_float_catchall(T_FE_ALL_EXCEPT);
*/

#endif /*<_T_MATH_FLOAT_EXCEPT_H__>*/



#endif /*<__RT_MATH_FLOAT_EXCEPTION_H>*/




#endif /*<__RT_MATH_FLOAT_H>*/

