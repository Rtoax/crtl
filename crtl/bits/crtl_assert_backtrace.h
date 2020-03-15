#ifndef __CRTL_BITS_ASSERT_BACKTRACE_H
#define __CRTL_BITS_ASSERT_BACKTRACE_H 1


#include <assert.h>
#include <execinfo.h>


#include "crtl/easy/attribute.h"
#include "crtl/easy/macro.h"
#include "crtl/crtl_log.h"


#define __CRTL_BACKTRACE_SIZE 1024

/* backtrace */
static void _unused __crtl_assert_backtrace(FILE *fp)
{
    if(fp == NULL)
    {
        fp = stderr;
    }
 
    void *__buffer[__CRTL_BACKTRACE_SIZE];
    unsigned long size = 0;
    char **__backtrace;
    int _unused i;
    
    size = backtrace (__buffer, __CRTL_BACKTRACE_SIZE);
    __backtrace = backtrace_symbols (__buffer, size);
    
    //FILE *__fp = fopen("core.121212", "w");
    //backtrace_symbols_fd (__buffer, size, fileno(fp));
    
    backtrace_symbols_fd (__buffer, size, fileno(fp));

    free(__backtrace);
    
    return;    
}


#endif /*<__CRTL_BITS_ASSERT_BACKTRACE_H>*/
