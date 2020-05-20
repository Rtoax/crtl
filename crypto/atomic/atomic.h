#ifndef __CRYPTO_ATOMIC_H
#define __CRYPTO_ATOMIC_H 1

#include <stdbool.h>

#include "crtl/easy/attribute.h"
#include "crtl/easy/macro.h"

#include "crypto/atomic/fetch.h"

#include "crypto/expect.h"



#define __ATOMIC_OP(op_name, op_string)					\
static inline int op_name(int val, int *ptr)				\
{									\
	int old, new;							\
									\
	asm volatile(							\
		"0:	lr	%[new],%[old]\n"			\
		op_string "	%[new],%[val]\n"			\
		"	cs	%[old],%[new],%[ptr]\n"			\
		"	jl	0b"					\
		: [old] "=d" (old), [new] "=&d" (new), [ptr] "+Q" (*ptr)\
		: [val] "d" (val), "0" (*ptr) : "cc", "memory");	\
	return old;							\
}

#define __ATOMIC_OPS(op_name, op_string)				\
	__ATOMIC_OP(op_name, op_string)					\
	__ATOMIC_OP(op_name##_barrier, op_string)

__ATOMIC_OPS(__atomic_add, "ar");
__ATOMIC_OPS(__atomic_and, "nr");
__ATOMIC_OPS(__atomic_or,  "or");
__ATOMIC_OPS(__atomic_xor, "xr");

#undef __ATOMIC_OPS

#define __ATOMIC64_OP(op_name, op_string)				\
static inline long op_name(long val, long *ptr)				\
{									\
	long old, new;							\
									\
	asm volatile(							\
		"0:	lgr	%[new],%[old]\n"			\
		op_string "	%[new],%[val]\n"			\
		"	csg	%[old],%[new],%[ptr]\n"			\
		"	jl	0b"					\
		: [old] "=d" (old), [new] "=&d" (new), [ptr] "+Q" (*ptr)\
		: [val] "d" (val), "0" (*ptr) : "cc", "memory");	\
	return old;							\
}

#define __ATOMIC64_OPS(op_name, op_string)				\
	__ATOMIC64_OP(op_name, op_string)				\
	__ATOMIC64_OP(op_name##_barrier, op_string)

__ATOMIC64_OPS(__atomic64_add, "agr");
__ATOMIC64_OPS(__atomic64_and, "ngr");
__ATOMIC64_OPS(__atomic64_or,  "ogr");
__ATOMIC64_OPS(__atomic64_xor, "xgr");

#undef __ATOMIC64_OPS

#define __atomic_add_const(val, ptr)		__atomic_add(val, ptr)
#define __atomic_add_const_barrier(val, ptr)	__atomic_add(val, ptr)
#define __atomic64_add_const(val, ptr)		__atomic64_add(val, ptr)
#define __atomic64_add_const_barrier(val, ptr)	__atomic64_add(val, ptr)


static inline int __atomic_cmpxchg(int *ptr, int old, int new)
{
	return __sync_val_compare_and_swap(ptr, old, new);
}

static inline int __atomic_cmpxchg_bool(int *ptr, int old, int new)
{
	return __sync_bool_compare_and_swap(ptr, old, new);
}

static inline long __atomic64_cmpxchg(long *ptr, long old, long new)
{
	return __sync_val_compare_and_swap(ptr, old, new);
}

static inline long __atomic64_cmpxchg_bool(long *ptr, long old, long new)
{
	return __sync_bool_compare_and_swap(ptr, old, new);
}




typedef struct {
	volatile int val;
}crtl_atomic_t;

#define CRTL_ATOMIC_INITIALIZER { 0 }


#define crtl_atomic_read(v)		READ_ONCE((v)->val)
//#define crtl_atomic_set(v, i)	WRITE_ONCE(((v)->val), (i))


/**
 * crtl_atomic_cmpxchg() - Atomic compare and exchange
 * @param uaddr:	The address of the futex to be modified
 * @param oldval:	The expected value of the futex
 * @param newval:	The new value to try and assign the futex
 *
 * @return the old value of addr->val.
 */
static inline int _unused crtl_atomic_cmpxchg(crtl_atomic_t *addr, int oldval, int newval)
{
	return __sync_val_compare_and_swap(&addr->val, oldval, newval);
}

/**
 * crtl_atomic_inc() - Atomic incrememnt
 * @param addr:	Address of the variable to increment
 *
 * @return the new value of addr->val.
 */
static inline int _unused crtl_atomic_inc(crtl_atomic_t *addr)
{
	return __sync_add_and_fetch(&addr->val, 1);
}

/**
 * crtl_atomic_dec() - Atomic decrement
 * @param addr:	Address of the variable to decrement
 *
 * @return the new value of addr-val.
 */
static inline int _unused crtl_atomic_dec(crtl_atomic_t *addr)
{
	return __sync_sub_and_fetch(&addr->val, 1);
}

/**
 * crtl_atomic_set() - Atomic set
 * @param addr:	Address of the variable to set
 * @param newval:	New value for the atomic_t
 *
 * @return the new value of addr->val.
 */
static inline int _unused crtl_atomic_set(crtl_atomic_t *addr, int newval)
{
	addr->val = newval;
	return newval;
}

static inline int _unused crtl_atomic_cmpxchg_relaxed(crtl_atomic_t *ptr, int old, int new)
{
	int oldval;
	unsigned long res;

	prefetchw((void*)&ptr->val);

	do {
		__asm__ __volatile__("@ atomic_cmpxchg\n"
		"ldrex	%1, [%3]\n"
		"mov	%0, #0\n"
		"teq	%1, %4\n"
		"strexeq %0, %5, [%3]\n"
		    : "=&r" (res), "=&r" (oldval), "+Qo" (ptr->val)
		    : "r" (&ptr->val), "Ir" (old), "r" (new)
		    : "cc");
	} while (res);

	return oldval;
}

static inline bool _unused crtl_atomic_try_cmpxchg_relaxed(crtl_atomic_t *v, int *old, int new)
{
	int r, o = *old;
	r = crtl_atomic_cmpxchg_relaxed(v, o, new);
	if (unlikely(r != o))
		*old = r;
	return likely(r == o);
}

static int _unused crtl_atomic_cmpxchgi(int* ptr, int oldval, int newval) 
{
#if defined(__i386__) || defined(__x86_64__)
  int out;
  __asm__ __volatile__ ("lock; cmpxchg %2, %1;"
                        : "=a" (out), "+m" (*(volatile int*) ptr)
                        : "r" (newval), "0" (oldval)
                        : "memory");
  return out;
#elif defined(__MVS__)
  unsigned int op4;
  if (__plo_CSST(ptr, (unsigned int*) &oldval, newval,
                (unsigned int*) ptr, *ptr, &op4))
    return oldval;
  else
    return op4;
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
  return atomic_cas_uint((uint_t *)ptr, (uint_t)oldval, (uint_t)newval);
#else
  return __sync_val_compare_and_swap(ptr, oldval, newval);
#endif
}


#endif /*<__CRYPTO_ATOMIC_H>*/

