#ifndef __CRTL_STRING_H
#define __CRTL_STRING_H 1


#include <stddef.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <errno.h>

#include "crtl/crtl_log.h"
#include "crtl/crtl_types.h"

#include "crtl/easy/attribute.h"
#include "crtl/easy/macro.h"
#include "crtl/bits/crtl_types_ctype.h"

#include "crtl/bits/crtl_regex.h"


_api int crtl_strstripc(char *str, const char ch);
_api int crtl_strccount (char c, char *s);
_api void crtl_strchop(char *s, char *t);
_api short crtl_estrtoh(char *s);
_api unsigned short crtl_estrtous(char *s);
_api int crtl_estrtoi(char *s);
_api unsigned int crtl_estrtoui(char *s);
_api long crtl_estrtol(char *s);
_api unsigned long crtl_estrtoul(char *s);

_api float crtl_estrtof(char *s);
_api double crtl_estrtod(char *s);
_api char *crtl_strjoint(char *dst, const char *fmt, ...);
_api int crtl_memshow(const void*in, int len);
_api void crtl_memswap(crtl_byte *x, crtl_byte *y, size_t size);
_api void crtl_memcopy(void *x, void *y, size_t size);


/* string to numeric conversion with error checking */
short crtl_eatoh(char *s);
unsigned short crtl_eatou(char *s);
int crtl_eatoi(char *s);
unsigned int crtl_eatop(char *s);
long crtl_eatol(char *s);
unsigned long crtl_eatov(char *s);
float crtl_eatof(char *s);
double crtl_eatod(char *s);



#endif /*<__CRTL_STRING_H>*/


