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




#ifndef likely
#define likely(exp) __builtin_expect(!!(exp), 1)
#endif 

#ifndef unlikely
#define unlikely(exp) __builtin_expect(!!(exp), 0)
#endif


/**
 *  x=8: 0000 0000 0000 1000; location of suffix "1" . 
 *  1000 -> 4 
 */
#ifndef _ffs
#define _ffs(integer) __builtin_ffs(integer)
#endif

/**
 * _ctzl - find first bit in word.
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
#ifndef _ctzl
#define _ctzl(long_word) __builtin_ctzl(long_word)
#endif

/*
 * _ffz - find first zero in word.
 * @word: The word to search
 *
 * Undefined if no zero exists, so code should check against ~0UL first.
 */
#define _ffz(x)  _ctzl(~(x))

/**
 * _ffs64 - find first set bit in a 64 bit word
 * @word: The 64 bit word
 *
 * On 64 bit arches this is a synomyn for _ctzl
 * The result is not defined if no bits are set, so check that @word
 * is non-zero before calling this.
 */
#define _ffs64(word)  _ctzl((unsigned long)word)

/**
 * _fls32 - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as _ffs.
 * Note _fls32(0) = 0, _fls32(1) = 1, _fls32(0x80000000) = 32.
 */
#define _fls32(x) (x ? sizeof(int) * 8 - __builtin_clz(x) : 0)

/**
 * _fls64 - find last (most-significant) set bit in a long word
 * @word: the word to search
 *
 * Undefined if no set bit exists, so code should check against 0 first.
 */
#define _fls64(word) ((sizeof(long) * 8) - 1 - __builtin_clzl(word))

/**
 * fls64 - find last set bit in a 64-bit word
 * @x: the word to search
 *
 * This is defined in a similar way as the libc and compiler builtin
 * ffsll, but returns the position of the most significant set bit.
 *
 * fls64(value) returns 0 if value is 0 or the position of the last
 * set bit if value is nonzero. The last (most significant) bit is
 * at position 64.
 */
#define fls64(x) (x?_fls64(x) + 1:0)



/**
 *  x=8: 0000 0000 0000 1000; number of "1" 
 *  8 -> 1
 */
#ifndef _popcount
#define _popcount(integer) __builtin_popcount(integer)
#endif

/**
 *  x=8: 0000 0000 0000 1000;  number of low "0" 
 *  8 -> 3
 */
#ifndef _ctz
#define _ctz(x) __builtin_ctz(x)
#endif


/**
 *  x=8: 0000 0000 0000 1000;  number of high "0" 
 *  8 -> 28
 */
#ifndef _clz
#define _clz(x) __builtin_clz(x)
#endif

/**
 *  x=8: 0000 0000 0000 1000;  parity of "1" 
 *  8 -> 0
 *  12 -> 0
 *  13 -> 1
 *  1 -> 1
 */
#ifndef parity
#define parity(x) __builtin_parity(x)
#endif




/* is constant during compiled:  yes -> 1, no -> 0 */
#ifndef constant_p
#define constant_p(v) __builtin_constant_p(v)
#endif

/* compare t1 with t2 is same as each other: yes -> 1, no -> 0 */
#ifndef types_cmp_p
#define types_cmp_p(t1, t2) __builtin_types_compatible_p(t1, t2)
#endif






#ifndef cmpxchg
#define cmpxchg(ptr, o, n)		    \
({									\
	__typeof__(*(ptr)) __o = (o);					\
	__typeof__(*(ptr)) __n = (n);					\
	(__typeof__(*(ptr))) __sync_val_compare_and_swap((ptr),__o,__n);\
})
#endif



#endif //__CRTL_EASY_MACRO_H

