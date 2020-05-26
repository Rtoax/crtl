// SPDX-License-Identifier: GPL-2.0
#include "crtl/log.h"

#include "crypto/operator/compare.h"
#include "crypto/bit/bit.h"
#include "crypto/limits.h"
#include "crypto/asm/div64.h"
#include "crypto/math/reciprocal_div.h"

/*
 * For a description of the algorithm please have a look at
 * include/linux/reciprocal_div.h
 */
struct reciprocal_value reciprocal_value(uint32_t d)
{
	struct reciprocal_value R;
	uint64_t m;
	int l;

	l = fls(d - 1);
	m = ((1ULL << 32) * ((1ULL << l) - d));
	do_div(m, d);
	++m;
	R.m = (uint32_t)m;
	R.sh1 = min(l, 1);
	R.sh2 = max(l - 1, 0);

	return R;
}

struct reciprocal_value_adv reciprocal_value_adv(uint32_t d, uint8_t prec)
{
	struct reciprocal_value_adv R;
	uint32_t l, post_shift;
	uint64_t mhigh, mlow;

	/* ceil(log2(d)) */
	l = fls(d - 1);
	/* NOTE: mlow/mhigh could overflow uint64_t when l == 32. This case needs to
	 * be handled before calling "reciprocal_value_adv", please see the
	 * comment at include/linux/reciprocal_div.h.
	 */
	crtl_print_warning("ceil(log2(0x%08x)) == 32, %s doesn't support such divisor", d, __func__);
	post_shift = l;
	mlow = 1ULL << (32 + l);
	do_div(mlow, d);
	mhigh = (1ULL << (32 + l)) + (1ULL << (32 + l - prec));
	do_div(mhigh, d);

	for (; post_shift > 0; post_shift--) {
		uint64_t lo = mlow >> 1, hi = mhigh >> 1;

		if (lo >= hi)
			break;

		mlow = lo;
		mhigh = hi;
	}

	R.m = (uint32_t)mhigh;
	R.sh = post_shift;
	R.exp = l;
	R.is_wide_m = mhigh > UINT_MAX;

	return R;
}

