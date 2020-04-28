// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <crtl/bits/crtl_bitmask.h>

/* How many bits in an unsigned long */
#define __BITS_PER_LONG (8 * sizeof(unsigned long))

/* __HOWMANY_LONGS(a,b) : how many elements of size b needed to hold all of a */
#define __HOWMANY_LONGS(x, y) (((x)+((y)-1))/(y))

/* How many longs in mask of n bits */
#define __LONGS_PER_BITS(n) __HOWMANY_LONGS(n, __BITS_PER_LONG)

#define __MAX(a, b) ((a) > (b) ? (a) : (b))

/*
 * Allocate and free `struct bitmask *`
 */

/* Allocate a new `struct bitmask` with a size of n bits */
struct crtl_bitmask *crtl_bitmask_alloc(unsigned int n)
{
	struct crtl_bitmask *bmp;

	bmp = malloc(sizeof(*bmp));
	if (bmp == 0)
		return 0;
	bmp->size = n;
	bmp->maskp = calloc(__LONGS_PER_BITS(n), sizeof(unsigned long));
	if (bmp->maskp == 0) {
		free(bmp);
		return 0;
	}
	return bmp;
}

/* Free `struct bitmask` */
void crtl_bitmask_free(struct crtl_bitmask *bmp)
{
	if (bmp == 0)
		return;
	free(bmp->maskp);
	bmp->maskp = (unsigned long *)0xdeadcdef;  /* double free tripwire */
	free(bmp);
}

/*
 * The routines __crtl_bitmask_getbit() and __crtl_bitmask_setbit() are the only
 * routines that actually understand the layout of bmp->maskp[].
 *
 * On little endian architectures, this could simply be an array of
 * bytes.  But the kernel layout of bitmasks _is_ visible to userspace
 * via the sched_(set/get)affinity calls in Linux 2.6, and on big
 * endian architectures, it is painfully obvious that this is an
 * array of unsigned longs.
 */

/* Return the value (0 or 1) of bit n in bitmask bmp */
static unsigned int __crtl_bitmask_getbit(const struct crtl_bitmask *bmp, unsigned int n)
{
	if (n < bmp->size)
		return (bmp->maskp[n/__BITS_PER_LONG] >> (n % __BITS_PER_LONG)) & 1;
	else
		return 0;
}

/* Set bit n in bitmask bmp to value v (0 or 1) */
static void __crtl_bitmask_setbit(struct crtl_bitmask *bmp, unsigned int n, unsigned int v)
{
	if (n < bmp->size) {
		if (v)
			bmp->maskp[n/__BITS_PER_LONG] |= 1UL << (n % __BITS_PER_LONG);
		else
			bmp->maskp[n/__BITS_PER_LONG] &=
				~(1UL << (n % __BITS_PER_LONG));
	}
}

/*
 * When parsing bitmask lists, only allow numbers, separated by one
 * of the allowed next characters.
 *
 * The parameter 'sret' is the return from a sscanf "%u%c".  It is
 * -1 if the sscanf input string was empty.  It is 0 if the first
 * character in the sscanf input string was not a decimal number.
 * It is 1 if the unsigned number matching the "%u" was the end of the
 * input string.  It is 2 if one or more additional characters followed
 * the matched unsigned number.  If it is 2, then 'nextc' is the first
 * character following the number.  The parameter 'ok_next_chars'
 * is the nul-terminated list of allowed next characters.
 *
 * The mask term just scanned was ok if and only if either the numbers
 * matching the %u were all of the input or if the next character in
 * the input past the numbers was one of the allowed next characters.
 */
static int __crtl_bitmask_scan_was_ok(int sret, char nextc, const char *ok_next_chars)
{
	return sret == 1 ||
		(sret == 2 && strchr(ok_next_chars, nextc) != NULL);
}

static const char *__crtl_bitmask_nexttoken(const char *q,  int sep)
{
	if (q)
		q = strchr(q, sep);
	if (q)
		q++;
	return q;
}

/* Set a single bit i in bitmask */
struct crtl_bitmask *crtl_bitmask_setbit(struct crtl_bitmask *bmp, unsigned int i)
{
	__crtl_bitmask_setbit(bmp, i, 1);
	return bmp;
}

/* Set all bits in bitmask: bmp = ~0 */
struct crtl_bitmask *crtl_bitmask_setall(struct crtl_bitmask *bmp)
{
	unsigned int i;
	for (i = 0; i < bmp->size; i++)
		__crtl_bitmask_setbit(bmp, i, 1);
	return bmp;
}

/* Clear all bits in bitmask: bmp = 0 */
struct crtl_bitmask *crtl_bitmask_clearall(struct crtl_bitmask *bmp)
{
	unsigned int i;
	for (i = 0; i < bmp->size; i++)
		__crtl_bitmask_setbit(bmp, i, 0);
	return bmp;
}

/* True if all bits are clear */
int crtl_bitmask_isallclear(const struct crtl_bitmask *bmp)
{
	unsigned int i;
	for (i = 0; i < bmp->size; i++)
		if (__crtl_bitmask_getbit(bmp, i))
			return 0;
	return 1;
}

/* True if specified bit i is set */
int crtl_bitmask_isbitset(const struct crtl_bitmask *bmp, unsigned int i)
{
	return __crtl_bitmask_getbit(bmp, i);
}

/* Number of lowest set bit (min) */
unsigned int crtl_bitmask_first(const struct crtl_bitmask *bmp)
{
	return crtl_bitmask_next(bmp, 0);
}

/* Number of highest set bit (__MAX) */
unsigned int crtl_bitmask_last(const struct crtl_bitmask *bmp)
{
	unsigned int i;
	unsigned int m = bmp->size;
	for (i = 0; i < bmp->size; i++)
		if (__crtl_bitmask_getbit(bmp, i))
			m = i;
	return m;
}

/* Number of next set bit at or above given bit i */
unsigned int crtl_bitmask_next(const struct crtl_bitmask *bmp, unsigned int i)
{
	unsigned int n;
	for (n = i; n < bmp->size; n++)
		if (__crtl_bitmask_getbit(bmp, n))
			break;
	return n;
}

/*
 * Parses a comma-separated list of numbers and ranges of numbers,
 * with optional ':%u' strides modifying ranges, into provided bitmask.
 * Some examples of input lists and their equivalent simple list:
 *	Input		Equivalent to
 *	0-3		0,1,2,3
 *	0-7:2		0,2,4,6
 *	1,3,5-7		1,3,5,6,7
 *	0-3:2,8-15:4	0,2,8,12
 */
int crtl_bitmask_parselist(const char *buf, struct crtl_bitmask *bmp)
{
	const char *p, *q;

	crtl_bitmask_clearall(bmp);

	q = buf;
	while (p = q, q = __crtl_bitmask_nexttoken(q, ','), p) {
		unsigned int a;		/* begin of range */
		unsigned int b;		/* end of range */
		unsigned int s;		/* stride */
		const char *c1, *c2;	/* next tokens after '-' or ',' */
		char nextc;		/* char after sscanf %u match */
		int sret;		/* sscanf return (number of matches) */

		sret = sscanf(p, "%u%c", &a, &nextc);
		if (!__crtl_bitmask_scan_was_ok(sret, nextc, ",-"))
			goto err;
		b = a;
		s = 1;
		c1 = __crtl_bitmask_nexttoken(p, '-');
		c2 = __crtl_bitmask_nexttoken(p, ',');
		if (c1 != NULL && (c2 == NULL || c1 < c2)) {
			sret = sscanf(c1, "%u%c", &b, &nextc);
			if (!__crtl_bitmask_scan_was_ok(sret, nextc, ",:"))
				goto err;
			c1 = __crtl_bitmask_nexttoken(c1, ':');
			if (c1 != NULL && (c2 == NULL || c1 < c2)) {
				sret = sscanf(c1, "%u%c", &s, &nextc);
				if (!__crtl_bitmask_scan_was_ok(sret, nextc, ","))
					goto err;
			}
		}
		if (!(a <= b))
			goto err;
		if (b >= bmp->size)
			goto err;
		while (a <= b) {
			__crtl_bitmask_setbit(bmp, a, 1);
			a += s;
		}
	}
	return 0;
err:
	crtl_bitmask_clearall(bmp);
	return -1;
}

/*
 * __crtl_bitmask_emit(buf, buflen, rbot, rtop, len)
 *
 * Helper routine for bitmask_displaylist().  Write decimal number
 * or range to buf+len, suppressing output past buf+buflen, with optional
 * comma-prefix.  Return len of what would be written to buf, if it
 * all fit.
 */

static inline int __crtl_bitmask_emit(char *buf, int buflen, int rbot, int rtop, int len)
{
	if (len > 0)
		len += snprintf(buf + len, __MAX(buflen - len, 0), ",");
	if (rbot == rtop)
		len += snprintf(buf + len, __MAX(buflen - len, 0), "%d", rbot);
	else
		len += snprintf(buf + len, __MAX(buflen - len, 0), "%d-%d",
				rbot, rtop);
	return len;
}

/*
 * Write decimal list representation of bmp to buf.
 *
 * Output format is a comma-separated list of decimal numbers and
 * ranges.  Consecutively set bits are shown as two hyphen-separated
 * decimal numbers, the smallest and largest bit numbers set in
 * the range.  Output format is compatible with the format
 * accepted as input by bitmap_parselist().
 *
 * The return value is the number of characters which would be
 * generated for the given input, excluding the trailing '\0', as
 * per ISO C99.
 */

int crtl_bitmask_displaylist(char *buf, int buflen, const struct crtl_bitmask *bmp)
{
	int len = 0;
	/* current bit is 'cur', most recently seen range is [rbot, rtop] */
	unsigned int cur, rbot, rtop;

	if (buflen > 0)
		*buf = 0;
	rbot = cur = crtl_bitmask_first(bmp);
	while (cur < bmp->size) {
		rtop = cur;
		cur = crtl_bitmask_next(bmp, cur+1);
		if (cur >= bmp->size || cur > rtop + 1) {
			len = __crtl_bitmask_emit(buf, buflen, rbot, rtop, len);
			rbot = cur;
		}
	}
	return len;
}

