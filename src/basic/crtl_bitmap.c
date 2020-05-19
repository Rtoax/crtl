
#include <errno.h>
#include <ctype.h>
#include <crtl/easy/compare.h>
#include <crtl/log.h>

#include "crtl/bits/crtl_bitmap.h"

#include <crtl/bits/ctype.h>

#include "crtl_mute_dbg.h"

#define IS_ERR(ptr) (!ptr)
#define PTR_ERR(ptr) ((long)ptr)


// SPDX-License-Identifier: GPL-2.0-only
/*
 * lib/bitmap.c
 * Helper functions for bitmap.h.
 */

/**
 * DOC: bitmap introduction
 *
 * bitmaps provide an array of bits, implemented using an an
 * array of unsigned longs.  The number of valid bits in a
 * given bitmap does _not_ need to be an exact multiple of
 * BITS_PER_LONG.
 *
 * The possible unused bits in the last, partially used word
 * of a bitmap are 'don't care'.  The implementation makes
 * no particular effort to keep them zero.  It ensures that
 * their value will not affect the results of any operation.
 * The bitmap operations that return Boolean (crtl_bitmap_empty,
 * for example) or scalar (crtl_bitmap_weight, for example) results
 * carefully filter out these unused bits from impacting their
 * results.
 *
 * The byte ordering of bitmaps is more natural on little
 * endian architectures.  See the big-endian headers
 * include/asm-ppc64/bitops.h and include/asm-s390/bitops.h
 * for the best explanations of this ordering.
 */

_hidden int __bitmap_equal(const unsigned long *bitmap1, const unsigned long *bitmap2, unsigned int bits)
{
	unsigned int k, lim = bits/CRTL_BITS_PER_LONG;
	for (k = 0; k < lim; ++k)
		if (bitmap1[k] != bitmap2[k])
			return 0;

	if (bits % CRTL_BITS_PER_LONG)
		if ((bitmap1[k] ^ bitmap2[k]) & CRTL_BITMAP_LAST_WORD_MASK(bits))
			return 0;

	return 1;
}

_api inline int crtl_bitmap_equal(const unsigned long *src1, const unsigned long *src2, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        return !((*src1 ^ *src2) & CRTL_BITMAP_LAST_WORD_MASK(nbits));
    if (__builtin_constant_p(nbits & CRTL_BITMAP_MEM_MASK) &&
        IS_ALIGNED(nbits, CRTL_BITMAP_MEM_ALIGNMENT))
        return !memcmp(src1, src2, nbits / 8);
    return __bitmap_equal(src1, src2, nbits);
}

_hidden bool __bitmap_or_equal(const unsigned long *bitmap1,
		       const unsigned long *bitmap2,
		       const unsigned long *bitmap3,
		       unsigned int bits)
{
	unsigned int k, lim = bits / CRTL_BITS_PER_LONG;
	unsigned long tmp;

	for (k = 0; k < lim; ++k) {
		if ((bitmap1[k] | bitmap2[k]) != bitmap3[k])
			return false;
	}

	if (!(bits % CRTL_BITS_PER_LONG))
		return true;

	tmp = (bitmap1[k] | bitmap2[k]) ^ bitmap3[k];
	return (tmp & CRTL_BITMAP_LAST_WORD_MASK(bits)) == 0;
}
               
_api inline bool crtl_bitmap_or_equal(const unsigned long *src1,
                  const unsigned long *src2,
                  const unsigned long *src3,
                  unsigned int nbits)
{
   if (!small_const_nbits(nbits))
       return __bitmap_or_equal(src1, src2, src3, nbits);

   return !(((*src1 | *src2) ^ *src3) & CRTL_BITMAP_LAST_WORD_MASK(nbits));
}

_hidden void __bitmap_complement(unsigned long *dst, const unsigned long *src, unsigned int bits)
{
	unsigned int k, lim = CRTL_BITS_TO_LONGS(bits);
	for (k = 0; k < lim; ++k)
		dst[k] = ~src[k];
}

_api inline void crtl_bitmap_complement(unsigned long *dst, const unsigned long *src,
			unsigned int nbits)
{
	if (small_const_nbits(nbits))
		*dst = ~(*src);
	else
		__bitmap_complement(dst, src, nbits);
}


/**
 * __bitmap_shift_right - logical right shift of the bits in a bitmap
 *   @dst : destination bitmap
 *   @src : source bitmap
 *   @shift : shift by this many bits
 *   @nbits : bitmap size, in bits
 *
 * Shifting right (dividing) means moving bits in the MS -> LS bit
 * direction.  Zeros are fed into the vacated MS positions and the
 * LS bits shifted off the bottom are lost.
 */
_hidden void __bitmap_shift_right(unsigned long *dst, const unsigned long *src,
			unsigned shift, unsigned nbits)
{
	unsigned k, lim = CRTL_BITS_TO_LONGS(nbits);
	unsigned off = shift/CRTL_BITS_PER_LONG, rem = shift % CRTL_BITS_PER_LONG;
	unsigned long mask = CRTL_BITMAP_LAST_WORD_MASK(nbits);
	for (k = 0; off + k < lim; ++k) {
		unsigned long upper, lower;

		/*
		 * If shift is not word aligned, take lower rem bits of
		 * word above and make them the top rem bits of result.
		 */
		if (!rem || off + k + 1 >= lim)
			upper = 0;
		else {
			upper = src[off + k + 1];
			if (off + k + 1 == lim - 1)
				upper &= mask;
			upper <<= (CRTL_BITS_PER_LONG - rem);
		}
		lower = src[off + k];
		if (off + k == lim - 1)
			lower &= mask;
		lower >>= rem;
		dst[k] = lower | upper;
	}
	if (off)
		memset(&dst[lim - off], 0, off*sizeof(unsigned long));
}

_api inline void crtl_bitmap_shift_right(unsigned long *dst, const unsigned long *src,
                unsigned int shift, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        *dst = (*src & CRTL_BITMAP_LAST_WORD_MASK(nbits)) >> shift;
    else
        __bitmap_shift_right(dst, src, shift, nbits);
}

/**
 * __bitmap_shift_left - logical left shift of the bits in a bitmap
 *   @dst : destination bitmap
 *   @src : source bitmap
 *   @shift : shift by this many bits
 *   @nbits : bitmap size, in bits
 *
 * Shifting left (multiplying) means moving bits in the LS -> MS
 * direction.  Zeros are fed into the vacated LS bit positions
 * and those MS bits shifted off the top are lost.
 */

_hidden void __bitmap_shift_left(unsigned long *dst, const unsigned long *src,
			unsigned int shift, unsigned int nbits)
{
	int k;
	unsigned int lim = CRTL_BITS_TO_LONGS(nbits);
	unsigned int off = shift/CRTL_BITS_PER_LONG, rem = shift % CRTL_BITS_PER_LONG;
	for (k = lim - off - 1; k >= 0; --k) {
		unsigned long upper, lower;

		/*
		 * If shift is not word aligned, take upper rem bits of
		 * word below and make them the bottom rem bits of result.
		 */
		if (rem && k > 0)
			lower = src[k - 1] >> (CRTL_BITS_PER_LONG - rem);
		else
			lower = 0;
		upper = src[k] << rem;
		dst[k + off] = lower | upper;
	}
	if (off)
		memset(dst, 0, off*sizeof(unsigned long));
}
            
_api inline void crtl_bitmap_shift_left(unsigned long *dst, const unsigned long *src,
                unsigned int shift, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        *dst = (*src << shift) & CRTL_BITMAP_LAST_WORD_MASK(nbits);
    else
        __bitmap_shift_left(dst, src, shift, nbits);
}


/**
 * crtl_bitmap_cut() - remove bit region from bitmap and right shift remaining bits
 * @dst: destination bitmap, might overlap with src
 * @src: source bitmap
 * @first: start bit of region to be removed
 * @cut: number of bits to remove
 * @nbits: bitmap size, in bits
 *
 * Set the n-th bit of @dst iff the n-th bit of @src is set and
 * n is less than @first, or the m-th bit of @src is set for any
 * m such that @first <= n < nbits, and m = n + @cut.
 *
 * In pictures, example for a big-endian 32-bit architecture:
 *
 * @src:
 * 31                                   63
 * |                                    |
 * 10000000 11000001 11110010 00010101  10000000 11000001 01110010 00010101
 *                 |  |              |                                    |
 *                16  14             0                                   32
 *
 * if @cut is 3, and @first is 14, bits 14-16 in @src are cut and @dst is:
 *
 * 31                                   63
 * |                                    |
 * 10110000 00011000 00110010 00010101  00010000 00011000 00101110 01000010
 *                    |              |                                    |
 *                    14 (bit 17     0                                   32
 *                        from @src)
 *
 * Note that @dst and @src might overlap partially or entirely.
 *
 * This is implemented in the obvious way, with a shift and carry
 * step for each moved bit. Optimisation is left as an exercise
 * for the compiler.
 */
_api void crtl_bitmap_cut(unsigned long *dst, const unsigned long *src,
		unsigned int first, unsigned int cut, unsigned int nbits)
{
	unsigned int len = CRTL_BITS_TO_LONGS(nbits);
	unsigned long keep = 0, carry;
	int i;

	memmove(dst, src, len * sizeof(*dst));

	if (first % CRTL_BITS_PER_LONG) {
		keep = src[first / CRTL_BITS_PER_LONG] &
		       (~0UL >> (CRTL_BITS_PER_LONG - first % CRTL_BITS_PER_LONG));
        __crtl_dbg("keep = %d\n", keep);
	}

	while (cut--) {
		for (i = first / CRTL_BITS_PER_LONG; i < len; i++) {
			if (i < len - 1)
				carry = dst[i + 1] & 1UL;
			else
				carry = 0;

			dst[i] = (dst[i] >> 1) | (carry << (CRTL_BITS_PER_LONG - 1));
            __crtl_dbg("dst[%d] = %x\n", i, (unsigned long)dst[i]);
		}
	}
    __crtl_dbg("first / CRTL_BITS_PER_LONG = %d\n", first / CRTL_BITS_PER_LONG);
	dst[first / CRTL_BITS_PER_LONG] &= ~0UL << (first % CRTL_BITS_PER_LONG);
	dst[first / CRTL_BITS_PER_LONG] |= keep;
}

_hidden int __bitmap_and(unsigned long *dst, const unsigned long *bitmap1,
				const unsigned long *bitmap2, unsigned int bits)
{
	unsigned int k;
	unsigned int lim = bits/CRTL_BITS_PER_LONG;
	unsigned long result = 0;

	for (k = 0; k < lim; k++)
		result |= (dst[k] = bitmap1[k] & bitmap2[k]);
	if (bits % CRTL_BITS_PER_LONG)
		result |= (dst[k] = bitmap1[k] & bitmap2[k] &
			   CRTL_BITMAP_LAST_WORD_MASK(bits));
	return result != 0;
}
                
_api inline int crtl_bitmap_and(unsigned long *dst, const unsigned long *src1,
            const unsigned long *src2, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        return (*dst = *src1 & *src2 & CRTL_BITMAP_LAST_WORD_MASK(nbits)) != 0;
    return __bitmap_and(dst, src1, src2, nbits);
}

_hidden void __bitmap_or(unsigned long *dst, const unsigned long *bitmap1,
				const unsigned long *bitmap2, unsigned int bits)
{
	unsigned int k;
	unsigned int nr = CRTL_BITS_TO_LONGS(bits);

	for (k = 0; k < nr; k++)
		dst[k] = bitmap1[k] | bitmap2[k];
}
_api inline void crtl_bitmap_or(unsigned long *dst, const unsigned long *src1,
            const unsigned long *src2, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        *dst = *src1 | *src2;
    else
        __bitmap_or(dst, src1, src2, nbits);
}

_hidden void __bitmap_xor(unsigned long *dst, const unsigned long *bitmap1,
				const unsigned long *bitmap2, unsigned int bits)
{
	unsigned int k;
	unsigned int nr = CRTL_BITS_TO_LONGS(bits);

	for (k = 0; k < nr; k++)
		dst[k] = bitmap1[k] ^ bitmap2[k];
}
_api inline void crtl_bitmap_xor(unsigned long *dst, const unsigned long *src1,
            const unsigned long *src2, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        *dst = *src1 ^ *src2;
    else
        __bitmap_xor(dst, src1, src2, nbits);
}

_hidden int __bitmap_andnot(unsigned long *dst, const unsigned long *bitmap1,
				const unsigned long *bitmap2, unsigned int bits)
{
	unsigned int k;
	unsigned int lim = bits/CRTL_BITS_PER_LONG;
	unsigned long result = 0;

	for (k = 0; k < lim; k++)
		result |= (dst[k] = bitmap1[k] & ~bitmap2[k]);
	if (bits % CRTL_BITS_PER_LONG)
		result |= (dst[k] = bitmap1[k] & ~bitmap2[k] &
			   CRTL_BITMAP_LAST_WORD_MASK(bits));
	return result != 0;
}
_api inline int crtl_bitmap_andnot(unsigned long *dst, const unsigned long *src1,
            const unsigned long *src2, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        return (*dst = *src1 & ~(*src2) & CRTL_BITMAP_LAST_WORD_MASK(nbits)) != 0;
    return __bitmap_andnot(dst, src1, src2, nbits);
}

_hidden void __bitmap_replace(unsigned long *dst,
		      const unsigned long *old, const unsigned long *new,
		      const unsigned long *mask, unsigned int nbits)
{
	unsigned int k;
	unsigned int nr = CRTL_BITS_TO_LONGS(nbits);

	for (k = 0; k < nr; k++)
		dst[k] = (old[k] & ~mask[k]) | (new[k] & mask[k]);
}
_api inline void crtl_bitmap_replace(unsigned long *dst,
                const unsigned long *old,
                const unsigned long *new,
                const unsigned long *mask,
                unsigned int nbits)
{
  if (small_const_nbits(nbits))
      *dst = (*old & ~(*mask)) | (*new & *mask);
  else
      __bitmap_replace(dst, old, new, mask, nbits);
}

_api inline void crtl_bitmap_next_clear_region(unsigned long *bitmap,
                        unsigned int *rs, unsigned int *re,
                        unsigned int end)
{
    *rs = crtl_find_next_zero_bit(bitmap, end, *rs);
    *re = crtl_find_next_bit(bitmap, end, *rs + 1);
}

_api inline void crtl_bitmap_next_set_region(unsigned long *bitmap,
                      unsigned int *rs, unsigned int *re,
                      unsigned int end)
{
    *rs = crtl_find_next_bit(bitmap, end, *rs);
    *re = crtl_find_next_zero_bit(bitmap, end, *rs + 1);
}


/**
* crtl_bitmap_from_u64 - Check and swap words within uint64_t.
*  @mask: source bitmap
*  @dst:  destination bitmap
*
* In 32-bit Big Endian kernel, when using ``(u32 *)(&val)[*]``
* to read uint64_t mask, we will get the wrong word.
* That is ``(u32 *)(&val)[0]`` gets the upper 32 bits,
* but we expect the lower 32-bits of uint64_t.
*/
_api inline void crtl_bitmap_from_u64(unsigned long *dst, uint64_t mask)
{
  dst[0] = mask & ULONG_MAX;

  if (sizeof(mask) > sizeof(unsigned long))
      dst[1] = mask >> 32;
}


/**
* crtl_bitmap_get_value8 - get an 8-bit value within a memory region
* @map: address to the bitmap memory region
* @start: bit offset of the 8-bit value; must be a multiple of 8
*
* Returns the 8-bit value located at the @start bit offset within the @src
* memory region.
*/
_api inline unsigned long crtl_bitmap_get_value8(const unsigned long *map,
                        unsigned long start)
{
  const size_t index = CRTL_BIT_WORD(start);
  const unsigned long offset = start % CRTL_BITS_PER_LONG;

  return (map[index] >> offset) & 0xFF;
}

/**
* crtl_bitmap_set_value8 - set an 8-bit value within a memory region
* @map: address to the bitmap memory region
* @value: the 8-bit value; values wider than 8 bits may clobber bitmap
* @start: bit offset of the 8-bit value; must be a multiple of 8
*/
_api inline void crtl_bitmap_set_value8(unsigned long *map, unsigned long value,
                 unsigned long start)
{
    const size_t index = CRTL_BIT_WORD(start);
    const unsigned long offset = start % CRTL_BITS_PER_LONG;

    map[index] &= ~(0xFFUL << offset);
    map[index] |= value << offset;
}

_hidden int __bitmap_intersects(const unsigned long *bitmap1,
			const unsigned long *bitmap2, unsigned int bits)
{
	unsigned int k, lim = bits/CRTL_BITS_PER_LONG;
	for (k = 0; k < lim; ++k)
		if (bitmap1[k] & bitmap2[k])
			return 1;

	if (bits % CRTL_BITS_PER_LONG)
		if ((bitmap1[k] & bitmap2[k]) & CRTL_BITMAP_LAST_WORD_MASK(bits))
			return 1;
	return 0;
}
_api inline int crtl_bitmap_intersects(const unsigned long *src1,
            const unsigned long *src2, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        return ((*src1 & *src2) & CRTL_BITMAP_LAST_WORD_MASK(nbits)) != 0;
    else
        return __bitmap_intersects(src1, src2, nbits);
}

_hidden int __bitmap_subset(const unsigned long *bitmap1,
		    const unsigned long *bitmap2, unsigned int bits)
{
	unsigned int k, lim = bits/CRTL_BITS_PER_LONG;
	for (k = 0; k < lim; ++k)
		if (bitmap1[k] & ~bitmap2[k])
			return 0;

	if (bits % CRTL_BITS_PER_LONG)
		if ((bitmap1[k] & ~bitmap2[k]) & CRTL_BITMAP_LAST_WORD_MASK(bits))
			return 0;
	return 1;
}
_api inline int crtl_bitmap_subset(const unsigned long *src1,
            const unsigned long *src2, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        return ! ((*src1 & ~(*src2)) & CRTL_BITMAP_LAST_WORD_MASK(nbits));
    else
        return __bitmap_subset(src1, src2, nbits);
}

_hidden int __bitmap_weight(const unsigned long *bitmap, unsigned int bits)
{
	unsigned int k, lim = bits/CRTL_BITS_PER_LONG;
	int w = 0;

	for (k = 0; k < lim; k++)
		w += hweight_long(bitmap[k]);

	if (bits % CRTL_BITS_PER_LONG)
		w += hweight_long(bitmap[k] & CRTL_BITMAP_LAST_WORD_MASK(bits));

	return w;
}
_api inline int crtl_bitmap_empty(const unsigned long *src, unsigned nbits)
{
    if (small_const_nbits(nbits))
        return ! (*src & CRTL_BITMAP_LAST_WORD_MASK(nbits));

    return crtl_find_first_bit(src, nbits) == nbits;
}

_api inline int crtl_bitmap_full(const unsigned long *src, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        return ! (~(*src) & CRTL_BITMAP_LAST_WORD_MASK(nbits));

    return crtl_find_first_zero_bit(src, nbits) == nbits;
}


_api inline int crtl_bitmap_weight(const unsigned long *src, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        return hweight_long(*src & CRTL_BITMAP_LAST_WORD_MASK(nbits));
    return __bitmap_weight(src, nbits);
}

_hidden void __bitmap_set(unsigned long *map, unsigned int start, int len)
{
	unsigned long *p = map + CRTL_BIT_WORD(start);
	const unsigned int size = start + len;
	int bits_to_set = CRTL_BITS_PER_LONG - (start % CRTL_BITS_PER_LONG);
	unsigned long mask_to_set = CRTL_BITMAP_FIRST_WORD_MASK(start);

	while (len - bits_to_set >= 0) {
		*p |= mask_to_set;
		len -= bits_to_set;
		bits_to_set = CRTL_BITS_PER_LONG;
		mask_to_set = ~0UL;
		p++;
	}
	if (len) {
		mask_to_set &= CRTL_BITMAP_LAST_WORD_MASK(size);
		*p |= mask_to_set;
	}
}
_api inline void crtl_bitmap_set(unsigned long *map, unsigned int start,
        unsigned int nbits)
{
    if (__builtin_constant_p(nbits) && nbits == 1)
        __crtl_set_bit(start, map);
    else if (__builtin_constant_p(start & CRTL_BITMAP_MEM_MASK) &&
         IS_ALIGNED(start, CRTL_BITMAP_MEM_ALIGNMENT) &&
         __builtin_constant_p(nbits & CRTL_BITMAP_MEM_MASK) &&
         IS_ALIGNED(nbits, CRTL_BITMAP_MEM_ALIGNMENT))
        memset((char *)map + start / 8, 0xff, nbits / 8);
    else
        __bitmap_set(map, start, nbits);
}

_hidden void __bitmap_clear(unsigned long *map, unsigned int start, int len)
{
	unsigned long *p = map + CRTL_BIT_WORD(start);
	const unsigned int size = start + len;
	int bits_to_clear = CRTL_BITS_PER_LONG - (start % CRTL_BITS_PER_LONG);
	unsigned long mask_to_clear = CRTL_BITMAP_FIRST_WORD_MASK(start);

	while (len - bits_to_clear >= 0) {
		*p &= ~mask_to_clear;
		len -= bits_to_clear;
		bits_to_clear = CRTL_BITS_PER_LONG;
		mask_to_clear = ~0UL;
		p++;
	}
	if (len) {
		mask_to_clear &= CRTL_BITMAP_LAST_WORD_MASK(size);
		*p &= ~mask_to_clear;
	}
}

_api inline void crtl_bitmap_clear(unsigned long *map, unsigned int start,
        unsigned int nbits)
{
    if (__builtin_constant_p(nbits) && nbits == 1)
        __crtl_clear_bit(start, map);
    else if (__builtin_constant_p(start & CRTL_BITMAP_MEM_MASK) &&
         IS_ALIGNED(start, CRTL_BITMAP_MEM_ALIGNMENT) &&
         __builtin_constant_p(nbits & CRTL_BITMAP_MEM_MASK) &&
         IS_ALIGNED(nbits, CRTL_BITMAP_MEM_ALIGNMENT))
        memset((char *)map + start / 8, 0, nbits / 8);
    else
        __bitmap_clear(map, start, nbits);
}


/**
 * crtl_bitmap_find_next_zero_area_off - find a contiguous aligned zero area
 * @map: The address to base the search on
 * @size: The bitmap size in bits
 * @start: The bitnumber to start searching at
 * @nr: The number of zeroed bits we're looking for
 * @align_mask: Alignment mask for zero area
 * @align_offset: Alignment offset for zero area.
 *
 * The @align_mask should be one less than a power of 2; the effect is that
 * the bit offset of all zero areas this function finds plus @align_offset
 * is multiple of that power of 2.
 */
_api unsigned long crtl_bitmap_find_next_zero_area_off(unsigned long *map,
					     unsigned long size,
					     unsigned long start,
					     unsigned int nr,
					     unsigned long align_mask,
					     unsigned long align_offset)
{
	unsigned long index, end, i;
again:
	index = crtl_find_next_zero_bit(map, size, start);

	/* Align allocation */
	index = __ALIGN_MASK(index + align_offset, align_mask) - align_offset;

	end = index + nr;
	if (end > size)
		return end;
	i = crtl_find_next_bit(map, end, index);
	if (i < end) {
		start = i + 1;
		goto again;
	}
	return index;
}

                         
_api inline unsigned long crtl_bitmap_find_next_zero_area(unsigned long *map,
                                unsigned long size,
                                unsigned long start,
                                unsigned int nr,
                                unsigned long align_mask)
{
 return crtl_bitmap_find_next_zero_area_off(map, size, start, nr,
                       align_mask, 0);
}


/*
 * Bitmap printing & parsing functions: first version by Nadia Yvette Chambers,
 * second version by Paul Jackson, third by Joe Korty.
 */

/**
 * crtl_bitmap_parse_user - convert an ASCII hex string in a user buffer into a bitmap
 *
 * @ubuf: pointer to user buffer containing string.
 * @ulen: buffer size in bytes.  If string is smaller than this
 *    then it must be terminated with a \0.
 * @maskp: pointer to bitmap array that will contain result.
 * @nmaskbits: size of bitmap, in bits.
 */
_api int crtl_bitmap_parse_user(const char *ubuf,
			unsigned int ulen, unsigned long *maskp,
			int nmaskbits)
{
	char *buf;
	int ret;
#if 0
	buf = memdup_user_nul(ubuf, ulen);
#else
    buf = malloc(ulen);
    memcpy(buf, ubuf, ulen);
#endif
	if (IS_ERR(buf))
		return PTR_ERR(buf);

	ret = crtl_bitmap_parse(buf, UINT_MAX, maskp, nmaskbits);

	free(buf);
	return ret;
}


/*
 * Region 9-38:4/10 describes the following bitmap structure:
 * 0	   9  12    18			38
 * .........****......****......****......
 *	    ^  ^     ^			 ^
 *      start  off   group_len	       end
 */
struct region {
	unsigned int start;
	unsigned int off;
	unsigned int group_len;
	unsigned int end;
};

static int __bitmap_set_region(const struct region *r,
				unsigned long *bitmap, int nbits)
{
	unsigned int start;

	if (r->end >= nbits)
		return -ERANGE;

	for (start = r->start; start <= r->end; start += r->group_len)
		crtl_bitmap_set(bitmap, start, min(r->end - start + 1, r->off));

	return 0;
}

static int __bitmap_check_region(const struct region *r)
{
	if (r->start > r->end || r->group_len == 0 || r->off > r->group_len)
		return -EINVAL;

	return 0;
}

static inline uint64_t __div_u64_rem(uint64_t dividend, uint32_t divisor, uint32_t *remainder)
{
	union {
		uint64_t v64;
		uint32_t v32[2];
	} d = { dividend };
	uint32_t upper;

	upper = d.v32[1];
	d.v32[1] = 0;
	if (upper >= divisor) {
		d.v32[1] = upper / divisor;
		upper %= divisor;
	}
	asm ("divl %2" : "=a" (d.v32[0]), "=d" (*remainder) :
		"rm" (divisor), "0" (d.v32[0]), "1" (upper));
	return d.v64;
}

static inline uint64_t __div_u64(uint64_t dividend, uint32_t divisor)
{
	uint32_t remainder;

	return __div_u64_rem(dividend, divisor, &remainder);
}

/*
 * Convert non-negative integer string representation in explicitly given radix
 * to an integer.
 * Return number of characters consumed maybe or-ed with overflow bit.
 * If overflow occurs, result integer (incorrect) is still returned.
 *
 * Don't you dare use this function.
 */
static unsigned int _parse_integer(const char *s,
				   unsigned int base,
				   unsigned long long *p)
{
	unsigned long long res;
	unsigned int rv;

	res = 0;
	rv = 0;
	while (1) {
		unsigned int c = *s;
		unsigned int lc = c | 0x20; /* don't tolower() this line */
		unsigned int val;

		if ('0' <= c && c <= '9')
			val = c - '0';
		else if ('a' <= lc && lc <= 'f')
			val = lc - 'a' + 10;
		else
			break;

		if (val >= base)
			break;
		/*
		 * Check for overflow only if we are within range of
		 * it in the max base we support (16)
		 */
		if (unlikely(res & (~0ull << 60))) {
			if (res > __div_u64((~0ULL)/*ULLONG_MAX*/ - val, base))
				rv |= /*KSTRTOX_OVERFLOW*/(1U << 31);
		}
		res = res * base + val;
		rv++;
		s++;
	}
	*p = res;
	return rv;
}
static inline void * __ERR_PTR(long error_)
{
   return (void *) error_;
}

static const char *bitmap_getnum(const char *str, unsigned int *num)
{
	unsigned long long n;
	unsigned int len;

	len = _parse_integer(str, 10, &n);
	if (!len)
		return __ERR_PTR(-EINVAL);
	if (len & /*KSTRTOX_OVERFLOW*/(1U << 31) || n != (unsigned int)n)
		return __ERR_PTR(-EOVERFLOW);

	*num = n;
	return str + len;
}

static inline bool end_of_str(char c)
{
	return c == '\0' || c == '\n';
}


static inline bool __end_of_region(char c)
{
	return isspace(c) || c == ',';
}

static inline bool end_of_region(char c)
{
	return __end_of_region(c) || end_of_str(c);
}

/*
 * The format allows commas and whitespases at the beginning
 * of the region.
 */
static const char *bitmap_find_region(const char *str)
{
	while (__end_of_region(*str))
		str++;

	return end_of_str(*str) ? NULL : str;
}

static const char *bitmap_find_region_reverse(const char *start, const char *end)
{
	while (start <= end && __end_of_region(*end))
		end--;

	return end;
}

static const char *bitmap_parse_region(const char *str, struct region *r)
{
	str = bitmap_getnum(str, &r->start);
	if (IS_ERR(str))
		return str;

	if (end_of_region(*str))
		goto no_end;

	if (*str != '-')
		return __ERR_PTR(-EINVAL);

	str = bitmap_getnum(str + 1, &r->end);
	if (IS_ERR(str))
		return str;

	if (end_of_region(*str))
		goto no_pattern;

	if (*str != ':')
		return __ERR_PTR(-EINVAL);

	str = bitmap_getnum(str + 1, &r->off);
	if (IS_ERR(str))
		return str;

	if (*str != '/')
		return __ERR_PTR(-EINVAL);

	return bitmap_getnum(str + 1, &r->group_len);

no_end:
	r->end = r->start;
no_pattern:
	r->off = r->end + 1;
	r->group_len = r->end + 1;

	return end_of_str(*str) ? NULL : str;
}

/**
 * crtl_bitmap_parselist - convert list format ASCII string to bitmap
 * @buf: read user string from this buffer; must be terminated
 *    with a \0 or \n.
 * @maskp: write resulting mask here
 * @nmaskbits: number of bits in mask to be written
 *
 * Input format is a comma-separated list of decimal numbers and
 * ranges.  Consecutively set bits are shown as two hyphen-separated
 * decimal numbers, the smallest and largest bit numbers set in
 * the range.
 * Optionally each range can be postfixed to denote that only parts of it
 * should be set. The range will divided to groups of specific size.
 * From each group will be used only defined amount of bits.
 * Syntax: range:used_size/group_size
 * Example: 0-1023:2/256 ==> 0,1,256,257,512,513,768,769
 *
 * Returns: 0 on success, -errno on invalid input strings. Error values:
 *
 *   - ``-EINVAL``: wrong region format
 *   - ``-EINVAL``: invalid character in string
 *   - ``-ERANGE``: bit number specified too large for mask
 *   - ``-EOVERFLOW``: integer overflow in the input parameters
 */
_api int crtl_bitmap_parselist(const char *buf, unsigned long *maskp, int nmaskbits)
{
	struct region r;
	long ret;

	crtl_bitmap_zero(maskp, nmaskbits);

	while (buf) {
		buf = bitmap_find_region(buf);
		if (buf == NULL)
			return 0;

		buf = bitmap_parse_region(buf, &r);
		if (IS_ERR(buf))
			return PTR_ERR(buf);

		ret = __bitmap_check_region(&r);
		if (ret)
			return ret;

		ret = __bitmap_set_region(&r, maskp, nmaskbits);
		if (ret)
			return ret;
	}

	return 0;
}


/**
 * crtl_bitmap_parselist_user()
 *
 * @ubuf: pointer to user buffer containing string.
 * @ulen: buffer size in bytes.  If string is smaller than this
 *    then it must be terminated with a \0.
 * @maskp: pointer to bitmap array that will contain result.
 * @nmaskbits: size of bitmap, in bits.
 *
 * Wrapper for crtl_bitmap_parselist(), providing it with user buffer.
 */
_api int crtl_bitmap_parselist_user(const char *ubuf,
			unsigned int ulen, unsigned long *maskp,
			int nmaskbits)
{
	char *buf;
	int ret;
    
#if 0
	buf = memdup_user_nul(ubuf, ulen);
#else
    buf = malloc(ulen);
    memcpy(buf, ubuf, ulen);
#endif

	if (IS_ERR(buf))
		return PTR_ERR(buf);

	ret = crtl_bitmap_parselist(buf, maskp, nmaskbits);

	free(buf);
	return ret;
}
            

static const char *bitmap_get_x32_reverse(const char *start,
					const char *end, uint32_t *num)
{
	uint32_t ret = 0;
	int c, i;

	for (i = 0; i < 32; i += 4) {
		c = crtl_hex_to_bin(*end--); //crtl_hex2bin
		if (c < 0)
			return __ERR_PTR(-EINVAL);

		ret |= c << i;

		if (start > end || __end_of_region(*end))
			goto out;
	}

	if (crtl_hex_to_bin(*end--) >= 0)
		return __ERR_PTR(-EOVERFLOW);
out:
	*num = ret;
	return end;
}

/**
* strnchrnul - Find and return a character in a length limited string,
* or end of string
* @s: The string to be searched
* @count: The number of characters to be searched
* @c: The character to search for
*
* Returns pointer to the first occurrence of 'c' in s. If c is not found,
* then return a pointer to the last character of the string.
*/
static char *strnchrnul(const char *s, size_t count, int c)
{
    while (count-- && *s && *s != (char)c)
        s++;
    return (char *)s;
}


/**
 * crtl_bitmap_parse - convert an ASCII hex string into a bitmap.
 * @start: pointer to buffer containing string.
 * @buflen: buffer size in bytes.  If string is smaller than this
 *    then it must be terminated with a \0 or \n. In that case,
 *    UINT_MAX may be provided instead of string length.
 * @maskp: pointer to bitmap array that will contain result.
 * @nmaskbits: size of bitmap, in bits.
 *
 * Commas group hex digits into chunks.  Each chunk defines exactly 32
 * bits of the resultant bitmask.  No chunk may specify a value larger
 * than 32 bits (%-EOVERFLOW), and if a chunk specifies a smaller value
 * then leading 0-bits are prepended.  %-EINVAL is returned for illegal
 * characters. Grouping such as "1,,5", ",44", "," or "" is allowed.
 * Leading, embedded and trailing whitespace accepted.
 */
_api int crtl_bitmap_parse(const char *start, unsigned int buflen,
		unsigned long *maskp, int nmaskbits)
{
	const char *end = strnchrnul(start, buflen, '\n') - 1;
	int chunks = CRTL_BITS_TO_U32(nmaskbits);
	uint32_t *bitmap = (uint32_t *)maskp;
	int unset_bit;

	while (1) {
		end = bitmap_find_region_reverse(start, end);
		if (start > end)
			break;

		if (!chunks--)
			return -EOVERFLOW;

		end = bitmap_get_x32_reverse(start, end, bitmap++);
		if (IS_ERR(end))
			return PTR_ERR(end);
	}

	unset_bit = (CRTL_BITS_TO_U32(nmaskbits) - chunks) * 32;
	if (unset_bit < nmaskbits) {
		crtl_bitmap_clear(maskp, unset_bit, nmaskbits - unset_bit);
		return 0;
	}

	if (crtl_find_next_bit(maskp, unset_bit, nmaskbits) != unset_bit)
		return -EOVERFLOW;

	return 0;
}

        
//#ifdef CONFIG_NUMA
#if 1

/**
 * bitmap_pos_to_ord - find ordinal of set bit at given position in bitmap
 *	@buf: pointer to a bitmap
 *	@pos: a bit position in @buf (0 <= @pos < @nbits)
 *	@nbits: number of valid bit positions in @buf
 *
 * Map the bit at position @pos in @buf (of length @nbits) to the
 * ordinal of which set bit it is.  If it is not set or if @pos
 * is not a valid bit position, map to -1.
 *
 * If for example, just bits 4 through 7 are set in @buf, then @pos
 * values 4 through 7 will get mapped to 0 through 3, respectively,
 * and other @pos values will get mapped to -1.  When @pos value 7
 * gets mapped to (returns) @ord value 3 in this example, that means
 * that bit 7 is the 3rd (starting with 0th) set bit in @buf.
 *
 * The bit positions 0 through @bits are valid positions in @buf.
 */
static int bitmap_pos_to_ord(const unsigned long *buf, unsigned int pos, unsigned int nbits)
{
	if (pos >= nbits || !crtl_test_bit(pos, buf))
		return -1;

	return __bitmap_weight(buf, pos);
}

/**
 * crtl_bitmap_ord_to_pos - find position of n-th set bit in bitmap
 *	@buf: pointer to bitmap
 *	@ord: ordinal bit position (n-th set bit, n >= 0)
 *	@nbits: number of valid bit positions in @buf
 *
 * Map the ordinal offset of bit @ord in @buf to its position in @buf.
 * Value of @ord should be in range 0 <= @ord < weight(buf). If @ord
 * >= weight(buf), returns @nbits.
 *
 * If for example, just bits 4 through 7 are set in @buf, then @ord
 * values 0 through 3 will get mapped to 4 through 7, respectively,
 * and all other @ord values returns @nbits.  When @ord value 3
 * gets mapped to (returns) @pos value 7 in this example, that means
 * that the 3rd set bit (starting with 0th) is at position 7 in @buf.
 *
 * The bit positions 0 through @nbits-1 are valid positions in @buf.
 */
_api unsigned int crtl_bitmap_ord_to_pos(const unsigned long *buf, unsigned int ord, unsigned int nbits)
{
	unsigned int pos;

	for (pos = crtl_find_first_bit(buf, nbits);
	     pos < nbits && ord;
	     pos = crtl_find_next_bit(buf, nbits, pos + 1))
		ord--;

	return pos;
}


/**
 * crtl_bitmap_print_to_pagebuf - convert bitmap to list or hex format ASCII string
 * @list: indicates whether the bitmap must be list
 * @buf: page aligned buffer into which string is placed
 * @maskp: pointer to bitmap to convert
 * @nmaskbits: size of bitmap, in bits
 *
 * Output format is a comma-separated list of decimal numbers and
 * ranges if list is specified or hex digits grouped into comma-separated
 * sets of 8 digits/set. Returns the number of characters written to buf.
 *
 * It is assumed that @buf is a pointer into a PAGE_SIZE, page-aligned
 * area and that sufficient storage remains at @buf to accommodate the
 * crtl_bitmap_print_to_pagebuf() output. Returns the number of characters
 * actually printed to @buf, excluding terminating '\0'.
 */
#define __PAGE_SIZE       4096
#define __offset_in_page(p) (((unsigned long)p) % __PAGE_SIZE)
_hidden int __scnprintf(char * buf, size_t size, const char * fmt, ...)
{
       ssize_t ssize = size;
       va_list args;
       int i;

       va_start(args, fmt);
       i = vsnprintf(buf, size, fmt, args);
       va_end(args);

       return (i >= ssize) ? (ssize - 1) : i;
}

_api int crtl_bitmap_print_to_pagebuf(bool list, char *buf, const unsigned long *maskp,
			    int nmaskbits)
{
	signed long len = __PAGE_SIZE - __offset_in_page(buf);

	return list ? __scnprintf(buf, len, "%*pbl\n", nmaskbits, maskp) :
		      __scnprintf(buf, len, "%*pb\n", nmaskbits, maskp);
}


/**
 * crtl_bitmap_remap - Apply map defined by a pair of bitmaps to another bitmap
 *	@dst: remapped result
 *	@src: subset to be remapped
 *	@old: defines domain of map
 *	@new: defines range of map
 *	@nbits: number of bits in each of these bitmaps
 *
 * Let @old and @new define a mapping of bit positions, such that
 * whatever position is held by the n-th set bit in @old is mapped
 * to the n-th set bit in @new.  In the more general case, allowing
 * for the possibility that the weight 'w' of @new is less than the
 * weight of @old, map the position of the n-th set bit in @old to
 * the position of the m-th set bit in @new, where m == n % w.
 *
 * If either of the @old and @new bitmaps are empty, or if @src and
 * @dst point to the same location, then this routine copies @src
 * to @dst.
 *
 * The positions of unset bits in @old are mapped to themselves
 * (the identify map).
 *
 * Apply the above specified mapping to @src, placing the result in
 * @dst, clearing any bits previously set in @dst.
 *
 * For example, lets say that @old has bits 4 through 7 set, and
 * @new has bits 12 through 15 set.  This defines the mapping of bit
 * position 4 to 12, 5 to 13, 6 to 14 and 7 to 15, and of all other
 * bit positions unchanged.  So if say @src comes into this routine
 * with bits 1, 5 and 7 set, then @dst should leave with bits 1,
 * 13 and 15 set.
 */
_api void crtl_bitmap_remap(unsigned long *dst, const unsigned long *src,
		const unsigned long *old, const unsigned long *new,
		unsigned int nbits)
{
	unsigned int oldbit, w;

	if (dst == src)		/* following doesn't handle inplace remaps */
		return;
	crtl_bitmap_zero(dst, nbits);

	w = crtl_bitmap_weight(new, nbits);
	crtl_for_each_set_bit(oldbit, src, nbits) {
		int n = bitmap_pos_to_ord(old, oldbit, nbits);

		if (n < 0 || w == 0)
			crtl_set_bit(oldbit, dst);	/* identity map */
		else
			crtl_set_bit(crtl_bitmap_ord_to_pos(new, n % w, nbits), dst);
	}
}

/**
 * crtl_bitmap_bitremap - Apply map defined by a pair of bitmaps to a single bit
 *	@oldbit: bit position to be mapped
 *	@old: defines domain of map
 *	@new: defines range of map
 *	@bits: number of bits in each of these bitmaps
 *
 * Let @old and @new define a mapping of bit positions, such that
 * whatever position is held by the n-th set bit in @old is mapped
 * to the n-th set bit in @new.  In the more general case, allowing
 * for the possibility that the weight 'w' of @new is less than the
 * weight of @old, map the position of the n-th set bit in @old to
 * the position of the m-th set bit in @new, where m == n % w.
 *
 * The positions of unset bits in @old are mapped to themselves
 * (the identify map).
 *
 * Apply the above specified mapping to bit position @oldbit, returning
 * the new bit position.
 *
 * For example, lets say that @old has bits 4 through 7 set, and
 * @new has bits 12 through 15 set.  This defines the mapping of bit
 * position 4 to 12, 5 to 13, 6 to 14 and 7 to 15, and of all other
 * bit positions unchanged.  So if say @oldbit is 5, then this routine
 * returns 13.
 */
_api int crtl_bitmap_bitremap(int oldbit, const unsigned long *old,
				const unsigned long *new, int bits)
{
	int w = crtl_bitmap_weight(new, bits);
	int n = bitmap_pos_to_ord(old, oldbit, bits);
	if (n < 0 || w == 0)
		return oldbit;
	else
		return crtl_bitmap_ord_to_pos(new, n % w, bits);
}

/**
 * crtl_bitmap_onto - translate one bitmap relative to another
 *	@dst: resulting translated bitmap
 * 	@orig: original untranslated bitmap
 * 	@relmap: bitmap relative to which translated
 *	@bits: number of bits in each of these bitmaps
 *
 * Set the n-th bit of @dst iff there exists some m such that the
 * n-th bit of @relmap is set, the m-th bit of @orig is set, and
 * the n-th bit of @relmap is also the m-th _set_ bit of @relmap.
 * (If you understood the previous sentence the first time your
 * read it, you're overqualified for your current job.)
 *
 * In other words, @orig is mapped onto (surjectively) @dst,
 * using the map { <n, m> | the n-th bit of @relmap is the
 * m-th set bit of @relmap }.
 *
 * Any set bits in @orig above bit number W, where W is the
 * weight of (number of set bits in) @relmap are mapped nowhere.
 * In particular, if for all bits m set in @orig, m >= W, then
 * @dst will end up empty.  In situations where the possibility
 * of such an empty result is not desired, one way to avoid it is
 * to use the crtl_bitmap_fold() operator, below, to first fold the
 * @orig bitmap over itself so that all its set bits x are in the
 * range 0 <= x < W.  The crtl_bitmap_fold() operator does this by
 * setting the bit (m % W) in @dst, for each bit (m) set in @orig.
 *
 * Example [1] for crtl_bitmap_onto():
 *  Let's say @relmap has bits 30-39 set, and @orig has bits
 *  1, 3, 5, 7, 9 and 11 set.  Then on return from this routine,
 *  @dst will have bits 31, 33, 35, 37 and 39 set.
 *
 *  When bit 0 is set in @orig, it means turn on the bit in
 *  @dst corresponding to whatever is the first bit (if any)
 *  that is turned on in @relmap.  Since bit 0 was off in the
 *  above example, we leave off that bit (bit 30) in @dst.
 *
 *  When bit 1 is set in @orig (as in the above example), it
 *  means turn on the bit in @dst corresponding to whatever
 *  is the second bit that is turned on in @relmap.  The second
 *  bit in @relmap that was turned on in the above example was
 *  bit 31, so we turned on bit 31 in @dst.
 *
 *  Similarly, we turned on bits 33, 35, 37 and 39 in @dst,
 *  because they were the 4th, 6th, 8th and 10th set bits
 *  set in @relmap, and the 4th, 6th, 8th and 10th bits of
 *  @orig (i.e. bits 3, 5, 7 and 9) were also set.
 *
 *  When bit 11 is set in @orig, it means turn on the bit in
 *  @dst corresponding to whatever is the twelfth bit that is
 *  turned on in @relmap.  In the above example, there were
 *  only ten bits turned on in @relmap (30..39), so that bit
 *  11 was set in @orig had no affect on @dst.
 *
 * Example [2] for crtl_bitmap_fold() + crtl_bitmap_onto():
 *  Let's say @relmap has these ten bits set::
 *
 *		40 41 42 43 45 48 53 61 74 95
 *
 *  (for the curious, that's 40 plus the first ten terms of the
 *  Fibonacci sequence.)
 *
 *  Further lets say we use the following code, invoking
 *  crtl_bitmap_fold() then crtl_bitmap_onto, as suggested above to
 *  avoid the possibility of an empty @dst result::
 *
 *	unsigned long *tmp;	// a temporary bitmap's bits
 *
 *	crtl_bitmap_fold(tmp, orig, crtl_bitmap_weight(relmap, bits), bits);
 *	crtl_bitmap_onto(dst, tmp, relmap, bits);
 *
 *  Then this table shows what various values of @dst would be, for
 *  various @orig's.  I list the zero-based positions of each set bit.
 *  The tmp column shows the intermediate result, as computed by
 *  using crtl_bitmap_fold() to fold the @orig bitmap modulo ten
 *  (the weight of @relmap):
 *
 *      =============== ============== =================
 *      @orig           tmp            @dst
 *      0                0             40
 *      1                1             41
 *      9                9             95
 *      10               0             40 [#f1]_
 *      1 3 5 7          1 3 5 7       41 43 48 61
 *      0 1 2 3 4        0 1 2 3 4     40 41 42 43 45
 *      0 9 18 27        0 9 8 7       40 61 74 95
 *      0 10 20 30       0             40
 *      0 11 22 33       0 1 2 3       40 41 42 43
 *      0 12 24 36       0 2 4 6       40 42 45 53
 *      78 102 211       1 2 8         41 42 74 [#f1]_
 *      =============== ============== =================
 *
 * .. [#f1]
 *
 *     For these marked lines, if we hadn't first done crtl_bitmap_fold()
 *     into tmp, then the @dst result would have been empty.
 *
 * If either of @orig or @relmap is empty (no set bits), then @dst
 * will be returned empty.
 *
 * If (as explained above) the only set bits in @orig are in positions
 * m where m >= W, (where W is the weight of @relmap) then @dst will
 * once again be returned empty.
 *
 * All bits in @dst not set by the above rule are cleared.
 */
_api void crtl_bitmap_onto(unsigned long *dst, const unsigned long *orig,
			const unsigned long *relmap, unsigned int bits)
{
	unsigned int n, m;	/* same meaning as in above comment */

	if (dst == orig)	/* following doesn't handle inplace mappings */
		return;
	crtl_bitmap_zero(dst, bits);

	/*
	 * The following code is a more efficient, but less
	 * obvious, equivalent to the loop:
	 *	for (m = 0; m < crtl_bitmap_weight(relmap, bits); m++) {
	 *		n = crtl_bitmap_ord_to_pos(orig, m, bits);
	 *		if (crtl_test_bit(m, orig))
	 *			crtl_set_bit(n, dst);
	 *	}
	 */

	m = 0;
	crtl_for_each_set_bit(n, relmap, bits) {
		/* m == bitmap_pos_to_ord(relmap, n, bits) */
		if (crtl_test_bit(m, orig))
			crtl_set_bit(n, dst);
		m++;
	}
}

/**
 * crtl_bitmap_fold - fold larger bitmap into smaller, modulo specified size
 *	@dst: resulting smaller bitmap
 *	@orig: original larger bitmap
 *	@sz: specified size
 *	@nbits: number of bits in each of these bitmaps
 *
 * For each bit oldbit in @orig, set bit oldbit mod @sz in @dst.
 * Clear all other bits in @dst.  See further the comment and
 * Example [2] for crtl_bitmap_onto() for why and how to use this.
 */
_api void crtl_bitmap_fold(unsigned long *dst, const unsigned long *orig,
			unsigned int sz, unsigned int nbits)
{
	unsigned int oldbit;

	if (dst == orig)	/* following doesn't handle inplace mappings */
		return;
	crtl_bitmap_zero(dst, nbits);

	crtl_for_each_set_bit(oldbit, orig, nbits)
		crtl_set_bit(oldbit % sz, dst);
}
#endif /* CONFIG_NUMA */

/*
 * Common code for bitmap_*_region() routines.
 *	bitmap: array of unsigned longs corresponding to the bitmap
 *	pos: the beginning of the region
 *	order: region size (log base 2 of number of bits)
 *	reg_op: operation(s) to perform on that region of bitmap
 *
 * Can set, verify and/or release a region of bits in a bitmap,
 * depending on which combination of REG_OP_* flag bits is set.
 *
 * A region of a bitmap is a sequence of bits in the bitmap, of
 * some size '1 << order' (a power of two), aligned to that same
 * '1 << order' power of two.
 *
 * Returns 1 if REG_OP_ISFREE succeeds (region is all zero bits).
 * Returns 0 in all other cases and reg_ops.
 */

enum {
	REG_OP_ISFREE,		/* true if region is all zero bits */
	REG_OP_ALLOC,		/* set all bits in region */
	REG_OP_RELEASE,		/* clear all bits in region */
};

static int __reg_op(unsigned long *bitmap, unsigned int pos, int order, int reg_op)
{
	int nbits_reg;		/* number of bits in region */
	int index;		/* index first long of region in bitmap */
	int offset;		/* bit offset region in bitmap[index] */
	int nlongs_reg;		/* num longs spanned by region in bitmap */
	int nbitsinlong;	/* num bits of region in each spanned long */
	unsigned long mask;	/* bitmask for one long of region */
	int i;			/* scans bitmap by longs */
	int ret = 0;		/* return value */

	/*
	 * Either nlongs_reg == 1 (for small orders that fit in one long)
	 * or (offset == 0 && mask == ~0UL) (for larger multiword orders.)
	 */
	nbits_reg = 1 << order;
	index = pos / CRTL_BITS_PER_LONG;
	offset = pos - (index * CRTL_BITS_PER_LONG);
	nlongs_reg = CRTL_BITS_TO_LONGS(nbits_reg);
	nbitsinlong = min(nbits_reg,  CRTL_BITS_PER_LONG);

	/*
	 * Can't do "mask = (1UL << nbitsinlong) - 1", as that
	 * overflows if nbitsinlong == BITS_PER_LONG.
	 */
	mask = (1UL << (nbitsinlong - 1));
	mask += mask - 1;
	mask <<= offset;

	switch (reg_op) {
	case REG_OP_ISFREE:
		for (i = 0; i < nlongs_reg; i++) {
			if (bitmap[index + i] & mask)
				goto done;
		}
		ret = 1;	/* all bits in region free (zero) */
		break;

	case REG_OP_ALLOC:
		for (i = 0; i < nlongs_reg; i++)
			bitmap[index + i] |= mask;
		break;

	case REG_OP_RELEASE:
		for (i = 0; i < nlongs_reg; i++)
			bitmap[index + i] &= ~mask;
		break;
	}
done:
	return ret;
}

/**
 * crtl_bitmap_find_free_region - find a contiguous aligned mem region
 *	@bitmap: array of unsigned longs corresponding to the bitmap
 *	@bits: number of bits in the bitmap
 *	@order: region size (log base 2 of number of bits) to find
 *
 * Find a region of free (zero) bits in a @bitmap of @bits bits and
 * allocate them (set them to one).  Only consider regions of length
 * a power (@order) of two, aligned to that power of two, which
 * makes the search algorithm much faster.
 *
 * Return the bit offset in bitmap of the allocated region,
 * or -errno on failure.
 */
_api int crtl_bitmap_find_free_region(unsigned long *bitmap, unsigned int bits, int order)
{
	unsigned int pos, end;		/* scans bitmap by regions of size order */

	for (pos = 0 ; (end = pos + (1U << order)) <= bits; pos = end) {
		if (!__reg_op(bitmap, pos, order, REG_OP_ISFREE))
			continue;
		__reg_op(bitmap, pos, order, REG_OP_ALLOC);
		return pos;
	}
	return -ENOMEM;
}

/**
 * crtl_bitmap_release_region - release allocated bitmap region
 *	@bitmap: array of unsigned longs corresponding to the bitmap
 *	@pos: beginning of bit region to release
 *	@order: region size (log base 2 of number of bits) to release
 *
 * This is the complement to __bitmap_find_free_region() and releases
 * the found region (by clearing it in the bitmap).
 *
 * No return value.
 */
_api void crtl_bitmap_release_region(unsigned long *bitmap, unsigned int pos, int order)
{
	__reg_op(bitmap, pos, order, REG_OP_RELEASE);
}

/**
 * crtl_bitmap_allocate_region - allocate bitmap region
 *	@bitmap: array of unsigned longs corresponding to the bitmap
 *	@pos: beginning of bit region to allocate
 *	@order: region size (log base 2 of number of bits) to allocate
 *
 * Allocate (set bits in) a specified region of a bitmap.
 *
 * Return 0 on success, or %-EBUSY if specified region wasn't
 * free (not all bits were zero).
 */
_api int crtl_bitmap_allocate_region(unsigned long *bitmap, unsigned int pos, int order)
{
	if (!__reg_op(bitmap, pos, order, REG_OP_ISFREE))
		return -EBUSY;
	return __reg_op(bitmap, pos, order, REG_OP_ALLOC);
}

/**
 * crtl_bitmap_copy_le - copy a bitmap, putting the bits into little-endian order.
 * @dst:   destination buffer
 * @src:   bitmap to copy
 * @nbits: number of bits in the bitmap
 *
 * Require nbits % BITS_PER_LONG == 0.
 */
#ifdef __BIG_ENDIAN

#include "crtl/easy/byteswap.h"

_api void crtl_bitmap_copy_le(unsigned long *dst, const unsigned long *src, unsigned int nbits)
{
	unsigned int i;

	for (i = 0; i < nbits/CRTL_BITS_PER_LONG; i++) {
		if (CRTL_BITS_PER_LONG == 64)
			dst[i] = cpu_to_le64(src[i]);
		else
			dst[i] = cpu_to_le32(src[i]);
	}
}
#endif

_api unsigned long *crtl_bitmap_alloc(unsigned int nbits)
{
	return malloc(CRTL_BITS_TO_LONGS(nbits) * sizeof(unsigned long));
}

_api void crtl_bitmap_free( unsigned long *bitmap)
{
	free(bitmap);
}


_api inline void crtl_bitmap_zero(unsigned long *dst, unsigned int nbits)
{
	unsigned int len = CRTL_BITS_TO_LONGS(nbits) * sizeof(unsigned long);
	memset(dst, 0, len);
}

_api inline void crtl_bitmap_fill(unsigned long *dst, unsigned int nbits)
{
	unsigned int len = CRTL_BITS_TO_LONGS(nbits) * sizeof(unsigned long);
//    printf("len = %d\n", len);
	memset(dst, 0xff, len);
}

_api inline void crtl_bitmap_copy(unsigned long *dst, const unsigned long *src, unsigned int nbits)
{
    unsigned int len = CRTL_BITS_TO_LONGS(nbits) * sizeof(unsigned long);
    memcpy(dst, src, len);
}


/*
* Copy bitmap and clear tail bits in last word.
*/
_api inline void crtl_bitmap_copy_clear_tail(unsigned long *dst, const unsigned long *src, unsigned int nbits)
{
   crtl_bitmap_copy(dst, src, nbits);
   if (nbits % CRTL_BITS_PER_LONG)
       dst[nbits / CRTL_BITS_PER_LONG] &= CRTL_BITMAP_LAST_WORD_MASK(nbits);
}


#if CRTL_BITS_PER_LONG == 64
/**
 * crtl_bitmap_from_arr32 - copy the contents of uint32_t array of bits to bitmap
 *	@bitmap: array of unsigned longs, the destination bitmap
 *	@buf: array of uint32_t (in host byte order), the source bitmap
 *	@nbits: number of bits in @bitmap
 */
_api void crtl_bitmap_from_arr32(unsigned long *bitmap, const uint32_t *buf, unsigned int nbits)
{
	unsigned int i, halfwords;

	halfwords = DIV_ROUND_UP(nbits, 32);
	for (i = 0; i < halfwords; i++) {
		bitmap[i/2] = (unsigned long) buf[i];
		if (++i < halfwords)
			bitmap[i/2] |= ((unsigned long) buf[i]) << 32;
	}

	/* Clear tail bits in last word beyond nbits. */
	if (nbits % CRTL_BITS_PER_LONG)
		bitmap[(halfwords - 1) / 2] &= CRTL_BITMAP_LAST_WORD_MASK(nbits);
}

/**
 * crtl_bitmap_to_arr32 - copy the contents of bitmap to a uint32_t array of bits
 *	@buf: array of uint32_t (in host byte order), the dest bitmap
 *	@bitmap: array of unsigned longs, the source bitmap
 *	@nbits: number of bits in @bitmap
 */
_api void crtl_bitmap_to_arr32(uint32_t *buf, const unsigned long *bitmap, unsigned int nbits)
{
	unsigned int i, halfwords;

	halfwords = DIV_ROUND_UP(nbits, 32);
	for (i = 0; i < halfwords; i++) {
		buf[i] = (uint32_t) (bitmap[i/2] & UINT_MAX);
		if (++i < halfwords)
			buf[i] = (uint32_t) (bitmap[i/2] >> 32);
	}

	/* Clear tail bits in last element of array beyond nbits. */
	if (nbits % CRTL_BITS_PER_LONG)
		buf[halfwords - 1] &= (uint32_t) (UINT_MAX >> ((-nbits) & 31));
}

#endif



