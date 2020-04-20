/* SPDX-License-Identifier: GPL-2.0 */
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

struct crtl_bitmask *crtl_bitmask_alloc(unsigned int n);
void crtl_bitmask_free(struct crtl_bitmask *bmp);

struct crtl_bitmask *crtl_bitmask_setbit(struct crtl_bitmask *bmp, unsigned int i);
struct crtl_bitmask *crtl_bitmask_setall(struct crtl_bitmask *bmp);
struct crtl_bitmask *crtl_bitmask_clearall(struct crtl_bitmask *bmp);

unsigned int crtl_bitmask_first(const struct crtl_bitmask *bmp);
unsigned int crtl_bitmask_next(const struct crtl_bitmask *bmp, unsigned int i);
unsigned int crtl_bitmask_last(const struct crtl_bitmask *bmp);
int crtl_bitmask_isallclear(const struct crtl_bitmask *bmp);
int crtl_bitmask_isbitset(const struct crtl_bitmask *bmp, unsigned int i);

int crtl_bitmask_parselist(const char *buf, struct crtl_bitmask *bmp);
int crtl_bitmask_displaylist(char *buf, int len, const struct crtl_bitmask *bmp);



#endif /*__CRTL_BITS_BITMASK__ */

