#ifndef __CRTL_EASY_MACRO_H
#define __CRTL_EASY_MACRO_H 1

#include <sys/cdefs.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>

/*
 * Common definitions for all gcc versions go here.
 */
#define __GCC_VERSION (__GNUC__ * 10000		\
		     + __GNUC_MINOR__ * 100	        \
		     + __GNUC_PATCHLEVEL__)

#if __GCC_VERSION < 40600
 #error Sorry, your compiler is too old - please upgrade it.
#endif


#ifdef __GNUC__
 #define SYSTEM_VERSION      __VERSION__/* like: 4.8.5 20150623 (Red Hat 4.8.5-36) */
 #define COMPILER_VERSION    GCC_VERSION
#else
 #define SYSTEM_VERSION      "unknown"
 #define COMPILER_VERSION    0
#endif


#ifndef alignof
#define alignof(s) __alignof__(s)
#endif

#ifndef weak_alias
#define weak_alias(name, aliasname) extern typeof (name) aliasname __attribute__ ((weak, alias(#name)))
#endif

#ifndef weak_hidden_alias
#define weak_hidden_alias(name, aliasname) extern __typeof (name) aliasname __attribute__ ((weak, alias (#name), __visibility__ ("hidden")));
#endif

#ifndef strong_alias
#define strong_alias(name, aliasname)  extern __typeof (name) aliasname __attribute__ ((alias (#name)));
#endif

#define weak_function __attribute__ ((weak))
#define weak_const_function __attribute__ ((weak, __const__))

       
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif


#ifndef sizeof_field
#define sizeof_field(TYPE, MEMBER) sizeof((((TYPE *)0)->MEMBER))
#endif

#ifndef offsetofend
#define offsetofend(TYPE, MEMBER) (offsetof(TYPE, MEMBER) + sizeof_field(TYPE, MEMBER))
#endif

#ifndef containof
 #ifdef offsetof
  #define containof(ptr, type, member) ((type *) ((char *)(ptr) - offsetof(type, member)))
 #else
  #define containof(ptr, type, member) ((type *)((char *)(ptr) - (size_t)&((type *)0)->member))
 #endif
#endif 


/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#ifndef container_of
#define container_of(ptr, type, member) ({          \
	const typeof(((type *)0)->member)*__mptr = (ptr);    \
		     (type *)((char *)__mptr - offsetof(type, member)); })
#endif

/**
 * offsetofend(TYPE, MEMBER)
 *
 * @TYPE: The type of the structure
 * @MEMBER: The member within the structure to get the end offset of
 */
#ifndef offsetofend
#define offsetofend(TYPE, MEMBER) (offsetof(TYPE, MEMBER)	+ sizeof(((TYPE *)0)->MEMBER))
#endif

/*
 *	#define container_of(ptr, type, member) ({ \
 *	const typeof (((type*)0)->member) *__mptr=(ptr);\
 *	(type*)((char*)__mptr-offsetof(type, member));})
 */
#ifndef indexofarray 
#define indexofarray(type, startAddr, endAddr) (abs(((ULONG)endAddr)-((ULONG)startAddr))/sizeof(type))
#endif


#ifndef WRITE_ONCE
#define WRITE_ONCE(var, val) (*((volatile typeof(val) *)(&(var))) = (val))
#endif
#ifndef READ_ONCE
#define READ_ONCE(var) (*((volatile typeof(var) *)(&(var))))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#ifndef CALL_ONCE
#define CALL_ONCE(x) static volatile int __call_once##x = 0; if(__call_once##x++==0)
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
#define typecheck_fn(type,function) \
({	typeof(type) __tmp = function; \
	(void)__tmp; \
})
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

/* put addr to the Cache before use */
#ifndef prefetch
#define prefetch(x) __builtin_prefetch(x)
#endif
 
#ifndef prefetchw
#define prefetchw(x) __builtin_prefetch(x,1)
#endif


/**
 *  x=8: 0000 0000 0000 1000; location of suffix "1" . 
 *  1000 -> 4 
 */
#ifndef ffs
#define ffs(integer) __builtin_ffs(integer)
#endif

/**
 *  x=8: 0000 0000 0000 1000; number of "1" 
 *  8 -> 1
 */
#ifndef popcount
#define popcount(integer) __builtin_popcount(integer)
#endif

/**
 *  x=8: 0000 0000 0000 1000;  number of low "0" 
 *  8 -> 3
 */
#ifndef ctz
#define ctz(x) __builtin_ctz(x)
#endif


/**
 *  x=8: 0000 0000 0000 1000;  number of high "0" 
 *  8 -> 28
 */
#ifndef clz
#define clz(x) __builtin_clz(x)
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

#ifndef __OPTIMIZE__
 #ifndef frame_address
  #define frame_address(level_integer) __builtin_frame_address(level_integer)
  #define frame_address0               __builtin_frame_address(0)
  #define frame_address1               __builtin_frame_address(1)
  #define frame_address2               __builtin_frame_address(2)
  #define frame_address3               __builtin_frame_address(3)
  /*...*/
 #endif
 #ifndef return_address
  #define return_address(level_integer) __builtin_return_address(level_integer)
  #define return_address0 __builtin_return_address(0)
  #define return_address1 __builtin_return_address(1)
  #define return_address2 __builtin_return_address(2)
  #define return_address3 __builtin_return_address(3)
  /*...*/
 #endif
#else //__OPTIMIZE__
 #ifndef frame_address
  #define frame_address(level_integer) (NULL)
  #define frame_address0               (NULL)
  #define frame_address1               (NULL)
  #define frame_address2               (NULL)
  #define frame_address3               (NULL)
  /*...*/
 #endif
 #ifndef return_address
  #define return_address(level_integer)   (NULL)
  #define return_address0                 (NULL)
  #define return_address1                 (NULL)
  #define return_address2                 (NULL)
  #define return_address3                 (NULL)
  /*...*/
 #endif
#endif //__OPTIMIZE__

/**
 *  To use , you must compile like "gcc XXX.c -O2" (优化选项: -O,-O0,-O1,-O2,-O3)
 */
#ifndef object_size
 #define object_size(p_obj, type)    __builtin_object_size(p_obj, type)
 #define object_size0(p_obj)         __builtin_object_size(p_obj, 0)
 #define object_size1(p_obj)         __builtin_object_size(p_obj, 1)
 #define object_size2(p_obj)         __builtin_object_size(p_obj, 2)
 #define object_size3(p_obj)         __builtin_object_size(p_obj, 3)
#endif


/* is constant during compiled:  yes -> 1, no -> 0 */
#ifndef constant_p
#define constant_p(v) __builtin_constant_p(v)
#endif

/* compare t1 with t2 is same as each other: yes -> 1, no -> 0 */
#ifndef types_cmp_p
#define types_cmp_p(t1, t2) __builtin_types_compatible_p(t1, t2)
#endif



#define _fetch_and_add(ptr, v)  __sync_fetch_and_add(ptr, v)
#define _fetch_and_sub(ptr, v)  __sync_fetch_and_sub(ptr, v)
#define _fetch_and_or(ptr, v)  __sync_fetch_and_or(ptr, v)
#define _fetch_and_and(ptr, v)  __sync_fetch_and_and(ptr, v)
#define _fetch_and_xor(ptr, v)  __sync_fetch_and_xor(ptr, v)
#define _fetch_and_nand(ptr, v)  __sync_fetch_and_nand(ptr, v)


#define _add_and_fetch(ptr, v)  __sync_add_and_fetch(ptr, v)
#define _sub_and_fetch(ptr, v)  __sync_sub_and_fetch(ptr, v)
#define _or_and_fetch(ptr, v)  __sync_or_and_fetch(ptr, v)
#define _and_and_fetch(ptr, v)  __sync_and_and_fetch(ptr, v)
#define _xor_and_fetch(ptr, v)  __sync_xor_and_fetch(ptr, v)
#define _nand_and_fetch(ptr, v)  __sync_nand_and_fetch(ptr, v)

#define _synchronize(...)  __sync_synchronize(__VA_ARGS__)
#define _bool_compare_and_swap(ptr, oldval, newold) __sync_bool_compare_and_swap(ptr, oldval, newold)
#define _val_compare_and_swap(ptr, oldval, newold) __sync_val_compare_and_swap(ptr, oldval, newold)
#define CAS(loc, old_value, new_value) __sync_bool_compare_and_swap((void **)loc, old_value, new_value)






#endif //__CRTL_EASY_MACRO_H

