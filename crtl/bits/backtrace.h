#ifndef __CRTL_BITS_ASSERT_BACKTRACE_H
#define __CRTL_BITS_ASSERT_BACKTRACE_H 1


#include <execinfo.h>


#include "crtl/easy/attribute.h"
#include "crtl/easy/macro.h"
#include "crtl/log.h"


#define __CRTL_BACKTRACE_SIZE 1024

/**
 * print backtrace into file pointer stream
 * @param fp: file pointer
 * @return void
 */
_api inline void _unused __crtl_assert_backtrace(FILE *fp);


#endif /*<__CRTL_BITS_ASSERT_BACKTRACE_H>*/
