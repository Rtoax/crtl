#ifndef __CRTL_EASY_MACRO_H
#define __CRTL_EASY_MACRO_H 1

#include <sys/cdefs.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>




#ifndef WRITE_ONCE
#define WRITE_ONCE(var, val) (*((volatile typeof(val) *)(&(var))) = (val))
#endif
#ifndef READ_ONCE
#define READ_ONCE(var) (*((volatile typeof(var) *)(&(var))))
#endif

#ifndef CALL_ONCE
#define CALL_ONCE(x) static volatile int __call_once##x = 0; if(__call_once##x++==0)
#endif

#ifndef ACCESS_ONCE
#define ACCESS_ONCE(type, var) (*(volatile type*) &(var))
#endif


/*
 * Check at compile time that something is of a particular type.
 * Always evaluates to 1 so you may use it easily in comparisons.
 */
#ifndef typecheck
#define typecheck(type,x) \
    ({	type __dummy; \
    	typeof(x) __dummy2; \
    	(void)(&__dummy == &__dummy2); \
    	1; \
    })
#endif

/*
 * Check at compile time that 'function' is a certain type, or is a pointer
 * to that type (needs to use typedef for the function type.)
 */
#ifndef typecheck_fn
#define typecheck_fn(type,function) ({	typeof(type) __tmp = function; (void)__tmp; })
#endif


#ifndef is_native_word
#define is_native_word(t) \
	(sizeof(t) == sizeof(char) || sizeof(t) == sizeof(short) || \
	 sizeof(t) == sizeof(int) || sizeof(t) == sizeof(long))
#endif











#endif //__CRTL_EASY_MACRO_H

