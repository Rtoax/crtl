/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __CRTL_BITS_BITS_H
#define __CRTL_BITS_BITS_H

#include "crtl/bits/crtl_const.h"


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


#endif	/* __CRTL_BITS_BITS_H */
