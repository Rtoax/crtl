#include <stdio.h>
#include <stdarg.h>
#include <libgen.h> //basename

#include "crtl_log.h"
#include "crtl_lock.h"
#include "crtl_types.h"
#include "crtl/crtl_assert.h"


const static struct {
    crtl_log_level_t level;
    char *level_string;
    char *color_start;
    char *color_end;
}crtl_log_level_to_string[] = {
    {CRTL_LOG_EMERG,    "Emerg",    "\033[1;31m","\033[0m"},
    {CRTL_LOG_ALERT,    "Alert",    "\033[1;31m","\033[0m"},
    {CRTL_LOG_CRIT,     "Critical", "\033[1;31m","\033[0m"},
    {CRTL_LOG_ERR,      "Error",    "\033[1;31m","\033[0m"},
    {CRTL_LOG_WARNING,  "Warning",  "\033[1;35m","\033[0m"},
    {CRTL_LOG_NOTICE,   "Notice",   "\033[1;36m","\033[0m"},
    {CRTL_LOG_INFO,     "Info",     "\033[1;36m","\033[0m"},
    {CRTL_LOG_DEBUG,    "Debug",    "\033[1m",   "\033[0m"},
};


/* debug common function */
inline int 
crtl_printfp(FILE *fp, crtl_log_level_t level, int prefix, int _unused colorful,
                char *_file, const char *_func, const unsigned int _line, const char *fmt, ...)
{
    int nchar = 0;

    if(!fp || level < CRTL_LOG_EMERG || level > CRTL_LOG_DEBUG)
        return nchar;
    
    va_list arg;
    va_start(arg, fmt);
    
	crtl_fd_lock(fileno(fp));
    
    if(prefix)
        nchar += fprintf(fp, "%s:%d %s: %s%s%s: ", basename(_file), _line, _func, 
                         colorful?crtl_log_level_to_string[level].color_start:"", 
                         crtl_log_level_to_string[level].level_string, 
                         colorful?crtl_log_level_to_string[level].color_end:"");
    
    int vf_ret = vfprintf(fp, fmt, arg);
    if(vf_ret < 0) {
        printf("vfprintf error. %s\n", CRTL_SYS_ERROR);
        crtl_assert_fp(stderr, 0);
    }
    nchar += vf_ret;
	crtl_fd_unlock(fileno(fp));

    fflush(fp);
	va_end(arg);

    return nchar;
}


inline int crtl_memprint(FILE* fp, void *const addr, unsigned int size)
{
    if(!addr || size==0)
    {
        crtl_print_err("null pointer error.\n");
        return 0;
    }
    int count = 0;
    
    uint8_t *_mp = addr;
    
    crtl_fd_lock(fileno(fp));
    
    int i;
    for(i=0; i<size; i++)
    {
        count += fprintf(fp, "%02x ", *_mp);
        _mp++;
    }
    count += fprintf(fp, "\n");
    
    crtl_fd_unlock(fileno(fp));
    
    return count;
}


