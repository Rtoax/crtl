#include "crtl/crtl_assert.h"
#include "crtl/crtl_lock.h"



/* crypto API */
#define __assert_prefix     "assert."

inline void __crtl_assert(FILE *fp, int exp, int switch_on_assert, const char *__file, const char *__func, const int __line)
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

        fprintf(fp, __ASSERT_NOTE_FMT"\n", __ASSERT_NOTE_CONTEXT);
        fprintf(fp, "%s"__ASSERT_NOTE_FMT"%s\n",__ASSERT_NOTE_PREFIX, __ASSERT_NOTE_CONTEXT,__ASSERT_NOTE_SUBFIX);
        
        crtl_assert_backtrace(fp);

        fflush(fp);
    }

    crtl_fd_unlock(fileno(fp));

    if(switch_on_assert)
        assert(exp);
    
    return ;
}

/* backtrace */
inline void _unused __crtl_assert_backtrace(FILE *fp)
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
