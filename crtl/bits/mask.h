#ifndef __CRTL_BITS_MASK_H
#define __CRTL_BITS_MASK_H 1


#include <endian.h>

#include "crtl/easy/byteswap.h"

#if __BYTE_ORDER == __BIG_ENDIAN
 #define __crtl_mask_32bit(bit)	(1ULL << (bit))
#else /* __BYTE_ORDER == __LITTLE_ENDIAN */
// #define __crtl_mask_bit(bit)	((bit) < 8 ? ((1L << (bit)) << 8) : ((1L << (bit)) >> 8))
 #define __crtl_mask_32bit(bit)	_swapbyte32((1ULL << (bit)))
#endif

#define __crtl_mask_32bit_all 0xffffffff


#endif /*<__CRTL_BITS_MASK_H>*/

