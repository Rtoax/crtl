#include <malloc.h>
#include <assert.h>
#include <execinfo.h>
#include <stdarg.h>

#include "crtl/assert.h"
#include "crtl/log.h"

#include "crypto/attribute.h"
#include "crypto/once.h"
#include "crypto/type/check.h"
#include "crypto/core/backtrace.h"


extern void crtl_fd_lock(int fd);
extern void crtl_fd_unlock(int fd);

/* crypto API */
#define __assert_prefix     "assert."

_api inline void __crtl_assert(FILE *fp, int exp, int switch_on_assert, const char *__file, const char *__func, const int __line, char *fmt, ...)
{
    if(!fp)
    {
        fp = stdout;
    }
    crtl_fd_lock(fileno(fp));
    if(!exp)
    {        
#define __ASSERT_NOTE_FMT       "Assert [File:%s %s:%d]"    
#define __ASSERT_NOTE_CONTEXT   __file, __func, __line
#define __ASSERT_NOTE_PREFIX    "\033[1;31m"
#define __ASSERT_NOTE_SUBFIX    "\033[0m"

        /* 如果fmt不为NULL， 将打印信息 */
        if(fmt) {
            va_list va;
            va_start(va, fmt);
            fprintf(fp, __ASSERT_NOTE_FMT"\n", __ASSERT_NOTE_CONTEXT);
            vfprintf(fp, fmt, va);
            va_end(va);
        }

        fprintf(fp, "%s"__ASSERT_NOTE_FMT"%s\n",__ASSERT_NOTE_PREFIX, __ASSERT_NOTE_CONTEXT,__ASSERT_NOTE_SUBFIX);
        
        __crtl_assert_backtrace(fp);

        fflush(fp);
    }

    crtl_fd_unlock(fileno(fp));

    if(switch_on_assert)
        assert(exp);
    
    return ;
}

/* backtrace */
/**
 * display backtrace of rutinue
 * @param fp: FILE* of file (such as stdout, stderr)
 * @return void
 */
_api inline void _unused __crtl_assert_backtrace(FILE *fp)
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

