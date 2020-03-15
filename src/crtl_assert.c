#include "crtl/crtl_assert.h"
#include "crtl/crtl_lock.h"



/* crypto API */
#define __assert_prefix     "assert."

void __crtl_assert(FILE *fp, int exp, int switch_on_assert, const char *__file, const char *__func, const int __line)
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


