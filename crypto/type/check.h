

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

