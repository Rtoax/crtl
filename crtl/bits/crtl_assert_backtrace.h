#ifndef __CRTL_BITS_ASSERT_BACKTRACE_H
#define __CRTL_BITS_ASSERT_BACKTRACE_H 1


#include <assert.h>
#include <execinfo.h>


#include "crtl/easy/attribute.h"
#include "crtl/easy/macro.h"
#include "crtl/crtl_log.h"


#define __CRTL_BACKTRACE_SIZE 1024

/* backtrace */
inline void _unused __crtl_assert_backtrace(FILE *fp);


#endif /*<__CRTL_BITS_ASSERT_BACKTRACE_H>*/
