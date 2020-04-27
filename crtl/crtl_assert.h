#ifndef __CRTL_BITS_ASSERT_H
#define __CRTL_BITS_ASSERT_H 1

#include <errno.h>
#include <string.h>

#include "crtl/bits/crtl_assert_backtrace.h"


#define CRTL_SYS_ERROR  strerror(errno)

/**
 *  打印程序断言（调用栈）
 */
/* macro */
#define crtl_assert(exp) crtl_assert_fp(stderr, exp)
#define crtl_assert_fp(fp, exp) __crtl_assert(fp, !!(exp), 1, __FILE__, __func__, __LINE__)

/* backtrace */
#define crtl_backtrace() __crtl_assert_backtrace(stderr)
#define crtl_backtrace_fp(fp) __crtl_assert_backtrace(fp)


/* crypto API */
inline void __crtl_assert(FILE *fp, int exp, int switch_on_assert, const char *__file, const char *__func, const int __line);


#endif /*<__CRTL_BITS_ASSERT_H>*/

