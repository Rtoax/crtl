#ifndef __CRTL_ASSERT_H
# error Do not include <crtl/bits/backtrace.h> directly, include <crtl/assert.h> instead.
#endif

#ifndef __CRTL_BITS_ASSERT_BACKTRACE_H
#define __CRTL_BITS_ASSERT_BACKTRACE_H 1

#include <stdio.h>

#define __CRTL_BACKTRACE_SIZE 1024


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


/**
 * print backtrace into file pointer stream
 * @param fp: file pointer
 * @return void
 */
inline void __crtl_assert_backtrace(FILE *fp);


#endif /*<__CRTL_BITS_ASSERT_BACKTRACE_H>*/

