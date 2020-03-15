#ifndef __CRTL_BITS_ASSERT_H
#define __CRTL_BITS_ASSERT_H 1

#include <errno.h>
#include <string.h>

#include "crtl/bits/crtl_assert_backtrace.h"

#define CRTL_SYS_ERROR  strerror(errno)

/* backtrace */
#define crtl_assert_backtrace(fp) __crtl_assert_backtrace(fp)

/* macro */
#define crtl_assert_fp(fp, exp) __crtl_assert(fp, !!(exp), 0, __FILE__, __func__, __LINE__)


/* crypto API */
void __crtl_assert(FILE *fp, int exp, int switch_on_assert, const char *__file, const char *__func, const int __line);


#endif /*<__CRTL_BITS_ASSERT_H>*/

