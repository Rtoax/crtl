#ifndef __CRTL_BITS_BITOPS_H
#define __CRTL_BITS_BITOPS_H

#include <stdbool.h>
#include <stdint.h>
#include "crtl/bits/bits.h"
#include "crtl/easy/round.h"
#include "crtl/easy/macro.h"
#include "crtl/bits/hweight.h"


#define CRTL_BITS_PER_TYPE(type)	(sizeof(type) * CRTL_BITS_PER_BYTE)
#define CRTL_BITS_TO_LONGS(nr)	    DIV_ROUND_UP(nr, CRTL_BITS_PER_TYPE(long))
#define CRTL_BITS_TO_U64(nr)		DIV_ROUND_UP(nr, CRTL_BITS_PER_TYPE(uint64_t))
#define CRTL_BITS_TO_U32(nr)		DIV_ROUND_UP(nr, CRTL_BITS_PER_TYPE(uint32_t))
#define CRTL_BITS_TO_BYTES(nr)	    DIV_ROUND_UP(nr, CRTL_BITS_PER_TYPE(char))





/*
 * Include this here because some architectures need generic_ffs/fls in
 * scope
 */

#define crtl_for_each_set_bit(bit, addr, size) \
	for ((bit) = crtl_find_first_bit((addr), (size));		\
	     (bit) < (size);					\
	     (bit) = crtl_find_next_bit((addr), (size), (bit) + 1))

/* same as crtl_for_each_set_bit() but use bit as value to start with */
#define crtl_for_each_set_bit_from(bit, addr, size) \
	for ((bit) = crtl_find_next_bit((addr), (size), (bit));	\
	     (bit) < (size);					\
	     (bit) = crtl_find_next_bit((addr), (size), (bit) + 1))

#define crtl_for_each_clear_bit(bit, addr, size) \
	for ((bit) = crtl_find_first_zero_bit((addr), (size));	\
	     (bit) < (size);					\
	     (bit) = crtl_find_next_zero_bit((addr), (size), (bit) + 1))

/* same as crtl_for_each_clear_bit() but use bit as value to start with */
#define crtl_for_each_clear_bit_from(bit, addr, size) \
	for ((bit) = crtl_find_next_zero_bit((addr), (size), (bit));	\
	     (bit) < (size);					\
	     (bit) = crtl_find_next_zero_bit((addr), (size), (bit) + 1))

/**
 * crtl_for_each_set_clump8 - iterate over bitmap for each 8-bit clump with set bits
 * @start: bit offset to start search and to store the current iteration offset
 * @clump: location to store copy of current 8-bit clump
 * @bits: bitmap address to base the search on
 * @size: bitmap size in number of bits
 */
#define crtl_for_each_set_clump8(start, clump, bits, size) \
	for ((start) = crtl_find_first_clump8(&(clump), (bits), (size)); \
	     (start) < (size); \
	     (start) = crtl_find_next_clump8(&(clump), (bits), (size), (start) + 8))


static inline int crtl_get_bitmask_order(unsigned int count)
{
    int order;

    order = fls(count);
    return order;   /* We could be slightly more clever with -1 here... */
}


/**
 * rol64 - rotate a 64-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline uint64_t crtl_rol64(uint64_t word, unsigned int shift)
{
	return (word << (shift & 63)) | (word >> ((-shift) & 63));
}

/**
 * ror64 - rotate a 64-bit value right
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline uint64_t crtl_ror64(uint64_t word, unsigned int shift)
{
	return (word >> (shift & 63)) | (word << ((-shift) & 63));
}

/**
 * rol32 - rotate a 32-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline uint32_t crtl_rol32(uint32_t word, unsigned int shift)
{
	return (word << (shift & 31)) | (word >> ((-shift) & 31));
}

/**
 * ror32 - rotate a 32-bit value right
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline uint32_t crtl_ror32(uint32_t word, unsigned int shift)
{
	return (word >> (shift & 31)) | (word << ((-shift) & 31));
}

/**
 * rol16 - rotate a 16-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline uint16_t crtl_rol16(uint16_t word, unsigned int shift)
{
	return (word << (shift & 15)) | (word >> ((-shift) & 15));
}

/**
 * ror16 - rotate a 16-bit value right
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline uint16_t crtl_ror16(uint16_t word, unsigned int shift)
{
	return (word >> (shift & 15)) | (word << ((-shift) & 15));
}

/**
 * rol8 - rotate an 8-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline uint8_t crtl_rol8(uint8_t word, unsigned int shift)
{
	return (word << (shift & 7)) | (word >> ((-shift) & 7));
}

/**
 * ror8 - rotate an 8-bit value right
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline uint8_t crtl_ror8(uint8_t word, unsigned int shift)
{
	return (word >> (shift & 7)) | (word << ((-shift) & 7));
}

/**
 * sign_extend32 - sign extend a 32-bit value using specified bit as sign-bit
 * @value: value to sign extend
 * @index: 0 based bit index (0<=index<32) to sign bit
 *
 * This is safe to use for 16- and 8-bit types as well.
 */
static inline int32_t crtl_sign_extend32(uint32_t value, int index)
{
	uint8_t shift = 31 - index;
	return (int32_t)(value << shift) >> shift;
}

/**
 * sign_extend64 - sign extend a 64-bit value using specified bit as sign-bit
 * @value: value to sign extend
 * @index: 0 based bit index (0<=index<64) to sign bit
 */
static inline int64_t crtl_sign_extend64(uint64_t value, int index)
{
	uint8_t shift = 63 - index;
	return (int64_t)(value << shift) >> shift;
}




static inline int crtl_get_count_order(unsigned int count)
{
	int order;

	order = fls(count) - 1;
	if (count & (count - 1))
		order++;
	return order;
}

/**
 * get_count_order_long - get order after rounding @l up to power of 2
 * @l: parameter
 *
 * it is same as get_count_order() but with long type parameter
 */
static inline int crtl_get_count_order_long(unsigned long l)
{
	if (l == 0UL)
		return -1;
	else if (l & (l - 1UL))
		return (int)fls_long(l);
	else
		return (int)fls_long(l) - 1;
}






/**
 * assign_bit - Assign value to a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 * @value: the value to assign
 */
static inline void crtl_assign_bit(long nr, volatile unsigned long *addr, bool value)
{
	if (value)
		crtl_set_bit(nr, addr);
	else
		crtl_clear_bit(nr, addr);
}

static inline void __crtl_assign_bit(long nr, volatile unsigned long *addr, bool value)
{
	if (value)
		__crtl_set_bit(nr, addr);
	else
		__crtl_clear_bit(nr, addr);
}


#ifndef crtl_set_mask_bits
#define crtl_set_mask_bits(ptr, mask, bits)	\
({								\
	const typeof(*(ptr)) mask__ = (mask), bits__ = (bits);	\
	typeof(*(ptr)) old__, new__;				\
								\
	do {							\
		old__ = READ_ONCE(*(ptr));			\
		new__ = (old__ & ~mask__) | bits__;		\
	} while (cmpxchg(ptr, old__, new__) != old__);		\
								\
	old__;							\
})
#endif

#ifndef crtl_bit_clear_unless
#define crtl_bit_clear_unless(ptr, clear, test)	\
({								\
	const typeof(*(ptr)) clear__ = (clear), test__ = (test);\
	typeof(*(ptr)) old__, new__;				\
								\
	do {							\
		old__ = READ_ONCE(*(ptr));			\
		new__ = old__ & ~clear__;			\
	} while (!(old__ & test__) &&				\
		 cmpxchg(ptr, old__, new__) != old__);		\
								\
	!(old__ & test__);					\
})
#endif



#endif /*<__CRTL_BITS_BITOPS_H>*/
