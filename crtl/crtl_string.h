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

#include "crtl/bits/crtl_types_basic.h"

#include "crtl/easy/attribute.h"
#include "crtl/easy/macro.h"
#include "crtl/bits/crtl_types_ctype.h"

#include "crtl/bits/crtl_regex.h"

_api char *crtl_skip_spaces(const char *str);

_api int crtl_strstripc(char *str, const char ch);
_api int crtl_strccount (char c, char *s);
_api void crtl_strchop(char *s, char *t);
_api ssize_t crtl_strscpy(char* d, const char* s, size_t n);
_api short crtl_estrtoh(char *s);
_api unsigned short crtl_estrtous(char *s);
_api int crtl_estrtoi(char *s);
_api unsigned int crtl_estrtoui(char *s);
_api long crtl_estrtol(char *s);
_api unsigned long crtl_estrtoul(char *s);

_api float crtl_estrtof(char *s);
_api double crtl_estrtod(char *s);

_api int crtl_strtobool(const char *s, bool *res);

/**
 * Create byte array from hexadecimal string. 
 * @param str Pointer to the string.
 * @param buf Pointer to a buffer where the data should be stored. 
 * @param buflen Length for the data buffer.
 * @return -1 if string contained illegal charactes of if the buffer did not
 * contain enough room for data.
 */
_api int crtl_strtobyte(const char *str, uint8_t *buf, int buflen);


_api char *crtl_strjoint(char *dst, const char *fmt, ...);
_api int crtl_memshow(const void*in, int len);
_api void crtl_memswap(crtl_byte *x, crtl_byte *y, size_t size);
_api void crtl_memcopy(void *x, void *y, size_t size);
_api int crtl_strncasecmp(const char *s1, const char *s2, size_t len);
_api int crtl_strcasecmp(const char *s1, const char *s2);


/* string to numeric conversion with error checking */
_api short crtl_eatoh(char *s);
_api unsigned short crtl_eatou(char *s);
_api int crtl_eatoi(char *s);
_api unsigned int crtl_eatop(char *s);
_api long crtl_eatol(char *s);
_api unsigned long crtl_eatov(char *s);
_api float crtl_eatof(char *s);
_api double crtl_eatod(char *s);

_api bool crtl_glob_match(char const *pat, char const *str);


#endif /*<__CRTL_STRING_H>*/


