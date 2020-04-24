#ifndef __CRTL_BITS_ATOMIC_H
#define __CRTL_BITS_ATOMIC_H 1

#include <stdbool.h>

#include "crtl/easy/attribute.h"
#include "crtl/easy/macro.h"


typedef struct {
	volatile int val;
}crtl_atomic_t;

#define CRTL_ATOMIC_INITIALIZER { 0 }


#define crtl_atomic_read(v)		READ_ONCE((v)->val)
//#define crtl_atomic_set(v, i)	WRITE_ONCE(((v)->val), (i))


/**
 * atomic_cmpxchg() - Atomic compare and exchange
 * @uaddr:	The address of the futex to be modified
 * @oldval:	The expected value of the futex
 * @newval:	The new value to try and assign the futex
 *
 * Return the old value of addr->val.
 */
static inline int _unused crtl_atomic_cmpxchg(crtl_atomic_t *addr, int oldval, int newval)
{
	return __sync_val_compare_and_swap(&addr->val, oldval, newval);
}

/**
 * atomic_inc() - Atomic incrememnt
 * @addr:	Address of the variable to increment
 *
 * Return the new value of addr->val.
 */
static inline int _unused crtl_atomic_inc(crtl_atomic_t *addr)
{
	return __sync_add_and_fetch(&addr->val, 1);
}

/**
 * atomic_dec() - Atomic decrement
 * @addr:	Address of the variable to decrement
 *
 * Return the new value of addr-val.
 */
static inline int _unused crtl_atomic_dec(crtl_atomic_t *addr)
{
	return __sync_sub_and_fetch(&addr->val, 1);
}

/**
 * atomic_set() - Atomic set
 * @addr:	Address of the variable to set
 * @newval:	New value for the atomic_t
 *
 * Return the new value of addr->val.
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


#endif /*<__CRTL_BITS_ATOMIC_H>*/

