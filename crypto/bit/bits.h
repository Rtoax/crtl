/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __CRTL_BITS_BITS_H
#define __CRTL_BITS_BITS_H

#include "crypto/const.h"


#ifndef CRTL_BITS_PER_LONG_LONG
#define CRTL_BITS_PER_LONG_LONG 64
#endif
#ifndef CRTL_BITS_PER_LONG
#define CRTL_BITS_PER_LONG 64
#endif




#define CRTL_BIT(nr)			(CRTL_UL(1) << (nr))/*   <rongtao 2019.10>*/
#define CRTL_BIT_ULL(nr)		(CRTL_ULL(1) << (nr))
#define CRTL_BIT_MASK(nr)		(CRTL_UL(1) << ((nr) % CRTL_BITS_PER_LONG))
#define CRTL_BIT_WORD(nr)		((nr) / CRTL_BITS_PER_LONG)
#define CRTL_BIT_ULL_MASK(nr)	(CRTL_ULL(1) << ((nr) % CRTL_BITS_PER_LONG_LONG))
#define CRTL_BIT_ULL_WORD(nr)	((nr) / CRTL_BITS_PER_LONG_LONG)
#define CRTL_BITS_PER_BYTE		8


/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 *//*   <rongtao 2019.10>*/
#define CRTL_GENMASK(h, l) \
	(((~ CRTL_UL(0)) - (CRTL_UL(1) << (l)) + 1) & \
	 (~ CRTL_UL(0) >> (CRTL_BITS_PER_LONG - 1 - (h))))


#define CRTL_GENMASK_ULL(h, l) \
	(((~ CRTL_ULL(0)) - (CRTL_ULL(1) << (l)) + 1) & \
	 (~ CRTL_ULL(0) >> (CRTL_BITS_PER_LONG_LONG - 1 - (h))))



/**
 * DOC: bitmap bitops
 *
 *
 *  crtl_set_bit(bit, addr)                  *addr |= bit
 *  crtl_clear_bit(bit, addr)                *addr &= ~bit
 *  crtl_change_bit(bit, addr)               *addr ^= bit
 *  crtl_test_bit(bit, addr)                 Is bit set in *addr?
 *  crtl_test_and_set_bit(bit, addr)         Set bit and return old value
 *  crtl_test_and_clear_bit(bit, addr)       Clear bit and return old value
 *  crtl_test_and_change_bit(bit, addr)      Change bit and return old value
 *  crtl_find_first_zero_bit(addr, nbits)    Position first zero bit in *addr
 *  crtl_find_first_bit(addr, nbits)         Position first set bit in *addr
 *  crtl_find_next_zero_bit(addr, nbits, bit)
 *                                      Position next zero bit in *addr >= bit
 *  crtl_find_next_bit(addr, nbits, bit)     Position next set bit in *addr >= bit
 *  crtl_find_next_and_bit(addr1, addr2, nbits, bit)
 *                                      Same as crtl_find_next_bit, but in
 *                                      (*addr1 & *addr2)
 *
 */

/* How many bits in an unsigned long */
#define ____BITS_PER_LONG (8 * sizeof(unsigned long))

static inline void crtl_set_bit(int nr, volatile void *addr)
{
    volatile unsigned long *laddr = addr;
	laddr[nr / ____BITS_PER_LONG] |= 1UL << (nr % ____BITS_PER_LONG);
}

static inline void crtl_clear_bit(int nr, volatile void *addr)
{
    volatile unsigned long *laddr = addr;
	laddr[nr / ____BITS_PER_LONG] &= ~(1UL << (nr % ____BITS_PER_LONG));
}

static inline void crtl_change_bit(int nr, volatile void *addr)
{
	int	mask;
	volatile unsigned int *a = addr;
	unsigned long tmp;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%1, %0	! crtl_change_bit		\n\t"
		"xor		%2, %0				\n\t"
		"movco.l	%0, @%1				\n\t"
		"bf		1b				\n\t"
		: "=&z" (tmp)
		: "r" (a), "r" (mask)
		: "t", "memory"
	);
}

static inline int crtl_test_and_set_bit(int nr, volatile void *addr)
{
	int	mask, retval;
	volatile unsigned int *a = addr;
	unsigned long tmp;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%2, %0	! crtl_test_and_set_bit	\n\t"
		"mov		%0, %1				\n\t"
		"or		%3, %0				\n\t"
		"movco.l	%0, @%2				\n\t"
		"bf		1b				\n\t"
		"and		%3, %1				\n\t"
		: "=&z" (tmp), "=&r" (retval)
		: "r" (a), "r" (mask)
		: "t", "memory"
	);

	return retval != 0;
}

static inline int crtl_test_and_clear_bit(int nr, volatile void *addr)
{
	int	mask, retval;
	volatile unsigned int *a = addr;
	unsigned long tmp;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%2, %0	! crtl_test_and_clear_bit	\n\t"
		"mov		%0, %1				\n\t"
		"and		%4, %0				\n\t"
		"movco.l	%0, @%2				\n\t"
		"bf		1b				\n\t"
		"and		%3, %1				\n\t"
		"synco						\n\t"
		: "=&z" (tmp), "=&r" (retval)
		: "r" (a), "r" (mask), "r" (~mask)
		: "t", "memory"
	);

	return retval != 0;
}

static inline int crtl_test_and_change_bit(int nr, volatile void *addr)
{
	int	mask, retval;
	volatile unsigned int *a = addr;
	unsigned long tmp;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%2, %0	! crtl_test_and_change_bit	\n\t"
		"mov		%0, %1				\n\t"
		"xor		%3, %0				\n\t"
		"movco.l	%0, @%2				\n\t"
		"bf		1b				\n\t"
		"and		%3, %1				\n\t"
		"synco						\n\t"
		: "=&z" (tmp), "=&r" (retval)
		: "r" (a), "r" (mask)
		: "t", "memory"
	);

	return retval != 0;
}


/**
 * __crtl_set_bit - Set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * Unlike crtl_set_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
static inline void __crtl_set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = CRTL_BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + CRTL_BIT_WORD(nr);

	*p  |= mask;
}

static inline void __crtl_clear_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = CRTL_BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + CRTL_BIT_WORD(nr);

	*p &= ~mask;
}

/**
 * __crtl_change_bit - Toggle a bit in memory
 * @nr: the bit to change
 * @addr: the address to start counting from
 *
 * Unlike crtl_change_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
static inline void __crtl_change_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = CRTL_BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + CRTL_BIT_WORD(nr);

	*p ^= mask;
}

/**
 * __crtl_test_and_set_bit - Set a bit and return its old value
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is non-atomic and can be reordered.
 * If two examples of this operation race, one can appear to succeed
 * but actually fail.  You must protect multiple accesses with a lock.
 */
static inline int __crtl_test_and_set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = CRTL_BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + CRTL_BIT_WORD(nr);
	unsigned long old = *p;

	*p = old | mask;
	return (old & mask) != 0;
}

/**
 * __crtl_test_and_clear_bit - Clear a bit and return its old value
 * @nr: Bit to clear
 * @addr: Address to count from
 *
 * This operation is non-atomic and can be reordered.
 * If two examples of this operation race, one can appear to succeed
 * but actually fail.  You must protect multiple accesses with a lock.
 */
static inline int __crtl_test_and_clear_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = CRTL_BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + CRTL_BIT_WORD(nr);
	unsigned long old = *p;

	*p = old & ~mask;
	return (old & mask) != 0;
}

/* WARNING: non atomic and it can be reordered! */
static inline int __crtl_test_and_change_bit(int nr,
					    volatile unsigned long *addr)
{
	unsigned long mask = CRTL_BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + CRTL_BIT_WORD(nr);
	unsigned long old = *p;

	*p = old ^ mask;
	return (old & mask) != 0;
}

/**
 * crtl_test_bit - Determine whether a bit is set
 * @nr: bit number to test
 * @addr: Address to start counting from
 */
static inline int crtl_test_bit(int nr, const volatile unsigned long *addr)
{
	return 1UL & (addr[CRTL_BIT_WORD(nr)] >> (nr & (CRTL_BITS_PER_LONG-1)));
}



#endif	/* __CRTL_BITS_BITS_H */
