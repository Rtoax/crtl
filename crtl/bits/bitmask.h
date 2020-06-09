#ifndef __CRTL_BITS_BITMASK__
#define __CRTL_BITS_BITMASK__

/* Taken over from libbitmask, a project initiated from sgi:
 * Url:            http://oss.sgi.com/projects/cpusets/
 * Unfortunately it's not very widespread, therefore relevant parts are
 * pasted here.
 */

struct crtl_bitmask {
	unsigned int size;
	unsigned long *maskp;
};

/*
 * Allocate and free `struct bitmask *`
 */
/* Allocate a new `struct bitmask` with a size of n bits */
struct crtl_bitmask *crtl_bitmask_alloc(unsigned int n);

/* Free `struct bitmask` */
void crtl_bitmask_free(struct crtl_bitmask *bmp);

/* Set a single bit i in bitmask */
struct crtl_bitmask *crtl_bitmask_setbit(struct crtl_bitmask *bmp, unsigned int i);

/* Set all bits in bitmask: bmp = ~0 */
struct crtl_bitmask *crtl_bitmask_setall(struct crtl_bitmask *bmp);

/* Clear all bits in bitmask: bmp = 0 */
struct crtl_bitmask *crtl_bitmask_clearall(struct crtl_bitmask *bmp);

/* Number of lowest set bit (min) */
unsigned int crtl_bitmask_first(const struct crtl_bitmask *bmp);

/* Number of next set bit at or above given bit i */
unsigned int crtl_bitmask_next(const struct crtl_bitmask *bmp, unsigned int i);

/* Number of highest set bit (max) */
unsigned int crtl_bitmask_last(const struct crtl_bitmask *bmp);

/* True if all bits are clear */
int crtl_bitmask_isallclear(const struct crtl_bitmask *bmp);

/* True if specified bit i is set */
int crtl_bitmask_isbitset(const struct crtl_bitmask *bmp, unsigned int i);

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
int crtl_bitmask_parselist(const char *buf, struct crtl_bitmask *bmp);

/*
 * Write decimal list representation of bmp to buf.
 *
 * Output format is a comma-separated list of decimal numbers and
 * ranges.  Consecutively set bits are shown as two hyphen-separated
 * decimal numbers, the smallest and largest bit numbers set in
 * the range.  Output format is compatible with the format
 * accepted as input by crtl_bitmap_parselist().
 *
 * The return value is the number of characters which would be
 * generated for the given input, excluding the trailing '\0', as
 * per ISO C99.
 */
int crtl_bitmask_displaylist(char *buf, int len, const struct crtl_bitmask *bmp);



#endif /*__CRTL_BITS_BITMASK__ */

