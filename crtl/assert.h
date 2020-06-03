#ifndef __CRTL_ASSERT_H
#define __CRTL_ASSERT_H 1

#include <errno.h>
#include <string.h>
#include <stdio.h>


#define CRTL_SYS_ERROR  strerror(errno)

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
#define crtl_assert_fp(fp, exp) __crtl_assert(fp, !!(exp), 1, __FILE__, __func__, __LINE__, NULL)


/**
 * display assert of rutinue and print some extra information
 * @param exp:  such as the expression of "void assert(scalar expression);"
 * @param fmt:  information format, like 'int printf(const char *format, ...);'s format
 * @return void
 */
#define crtl_assert_print(exp, fmt...) __crtl_assert(stderr, !!(exp), 1, __FILE__, __func__, __LINE__, fmt)



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
inline void __crtl_assert(FILE *fp, int exp, int switch_on_assert, const char *__file, const char *__func, const int __line, char *fmt, ...);


#endif /*<__CRTL_ASSERT_H>*/

