#ifndef __CRTL_BITS_ASSERT_H
#define __CRTL_BITS_ASSERT_H 1

#include <errno.h>
#include <string.h>

#include "crtl/easy/attribute.h"

#include "crtl/bits/crtl_assert_backtrace.h"


#define CRTL_SYS_ERROR  strerror(errno)

/**
 *  打印程序断言（调用栈）
 */
/* macro */
/**
 * display assert of rutinue in stderr
 * @param exp:  such as the expression of "void assert(scalar expression);"
 * @return void
 */
#define crtl_assert(exp) crtl_assert_fp(stderr, exp)

/**
 * display assert of rutinue
 * @param fp: FILE* of file (such as stdout, stderr)
 * @param exp:  such as the expression of "void assert(scalar expression);"
 * @return void
 */
#define crtl_assert_fp(fp, exp) __crtl_assert(fp, !!(exp), 1, __FILE__, __func__, __LINE__)

/* backtrace */
/**
 * display backtrace of rutinue in stderr
 * @param void
 * @return void
 */
#define crtl_backtrace() __crtl_assert_backtrace(stderr)

/**
 * display backtrace of rutinue
 * @param fp: FILE* of file (such as stdout, stderr)
 * @return void
 */
#define crtl_backtrace_fp(fp) __crtl_assert_backtrace(fp)


/* crypto API */
/**
 * display assert of rutinue
 * @param fp: FILE* of file (such as stdout, stderr)
 * @param exp:  such as the expression of "void assert(scalar expression);"
 * @param switch_on_assert: weather execute assert(exp)
 * @param __file:   file of rutinue
 * @param __func:   function name of rutinue
 * @param __line:   line number of rutinue
 * @return void
 */
_api inline void __crtl_assert(FILE *fp, int exp, int switch_on_assert, const char *__file, const char *__func, const int __line);


#endif /*<__CRTL_BITS_ASSERT_H>*/

