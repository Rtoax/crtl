#include "crtl/easy/round.h"
#include "crtl/easy/attribute.h"
#include "crtl/easy/macro.h"
#include "crtl/easy/operator.h"

#include "crtl/bitmap.h"




/*
 * This is a common helper function for crtl_find_next_bit, crtl_find_next_zero_bit, and
 * crtl_find_next_and_bit. The differences are:
 *  - The "invert" argument, which is XORed with each fetched word before
 *    searching it for one bits.
 *  - The optional "addr2", which is anded with "addr1" if present.
 */
static inline unsigned long __crtl_find_next_bit(const unsigned long *addr1,
		const unsigned long *addr2, unsigned long nbits,
		unsigned long start, unsigned long invert)
{
	unsigned long tmp;

	if (unlikely(start >= nbits))
		return nbits;

	tmp = addr1[start / CRTL_BITS_PER_LONG];
	if (addr2)
		tmp &= addr2[start / CRTL_BITS_PER_LONG];
	tmp ^= invert;

	/* Handle 1st word. */
	tmp &= CRTL_BITMAP_FIRST_WORD_MASK(start);
	start = round_down(start, CRTL_BITS_PER_LONG);

	while (!tmp) {
		start += CRTL_BITS_PER_LONG;
		if (start >= nbits)
			return nbits;

		tmp = addr1[start / CRTL_BITS_PER_LONG];
		if (addr2)
			tmp &= addr2[start / CRTL_BITS_PER_LONG];
		tmp ^= invert;
	}

	return MIN(start + __ffs(tmp), nbits);
}


/*
 * Find the next set bit in a memory region.
 */
unsigned long crtl_find_next_bit(const unsigned long *addr, unsigned long size,
                unsigned long offset)
{
    return __crtl_find_next_bit(addr, NULL, size, offset, 0UL);
}


/*
 * Find the first set bit in a memory region.
 */
unsigned long crtl_find_first_bit(const unsigned long *addr, unsigned long size)
{
    unsigned long idx;

    for (idx = 0; idx * CRTL_BITS_PER_LONG < size; idx++) {
        if (addr[idx])
            return MIN(idx * CRTL_BITS_PER_LONG + __ffs(addr[idx]), size);
    }

    return size;
}

/*
 * Find the first cleared bit in a memory region.
 */
unsigned long crtl_find_first_zero_bit(const unsigned long *addr, unsigned long size)
{
	unsigned long idx;

	for (idx = 0; idx * CRTL_BITS_PER_LONG < size; idx++) {
		if (addr[idx] != ~0UL)
			return MIN(idx * CRTL_BITS_PER_LONG + ffz(addr[idx]), size);
	}

	return size;
}

unsigned long crtl_find_next_zero_bit(const unsigned long *addr, unsigned long size,
				 unsigned long offset)
{
	return __crtl_find_next_bit(addr, NULL, size, offset, ~0UL);
}


unsigned long crtl_find_next_and_bit(const unsigned long *addr1,
     const unsigned long *addr2, unsigned long size,
     unsigned long offset)
{
    return __crtl_find_next_bit(addr1, addr2, size, offset, 0UL);
}


unsigned long crtl_find_next_clump8(unsigned long *clump, const unsigned long *addr,
                unsigned long size, unsigned long offset)
{
    offset = crtl_find_next_bit(addr, size, offset);
    if (offset == size)
        return size;

    offset = round_down(offset, 8);
    *clump = crtl_bitmap_get_value8(addr, offset);

    return offset;
}


unsigned long crtl_find_last_bit(const unsigned long *addr, unsigned long size)
{
    if (size) {
        unsigned long val = CRTL_BITMAP_LAST_WORD_MASK(size);
        unsigned long idx = (size-1) / CRTL_BITS_PER_LONG;

        do {
            val &= addr[idx];
            if (val)
                return idx * CRTL_BITS_PER_LONG + __fls(val);

            val = ~0ul;
        } while (idx--);
    }
    return size;
}

