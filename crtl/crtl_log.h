#ifndef __CRTL_LOG_H
#define __CRTL_LOG_H 1

#include <stdio.h>
#include "crtl/easy/attribute.h"

typedef enum {
    CRTL_LOG_EMERG,     /* system is unusable */
    CRTL_LOG_ALERT,     /* action must be taken immediately */
    CRTL_LOG_CRIT,      /* critical conditions */
    CRTL_LOG_ERR,       /* error conditions */
    CRTL_LOG_WARNING,   /* warning conditions */
    CRTL_LOG_NOTICE,    /* normal but significant condition */
    CRTL_LOG_INFO,      /* informational */
    CRTL_LOG_DEBUG,     /* debug-level messages */
}crtl_log_level_t;


#define crtl_print_emerg(fmt...)    crtl_printfp(stderr, CRTL_LOG_EMERG, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_alert(fmt...)    crtl_printfp(stderr, CRTL_LOG_ALERT, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_crit(fmt...)     crtl_printfp(stderr, CRTL_LOG_CRIT, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_err(fmt...)      crtl_printfp(stderr, CRTL_LOG_ERR, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_warning(fmt...)  crtl_printfp(stdout, CRTL_LOG_WARNING, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_notice(fmt...)   crtl_printfp(stdout, CRTL_LOG_NOTICE, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_info(fmt...)     crtl_printfp(stdout, CRTL_LOG_INFO, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_debug(fmt...)    crtl_printfp(stdout, CRTL_LOG_DEBUG, 1, 1, __FILE__, __func__, __LINE__, fmt)


#define CRTL_DEBUG 1

#if CRTL_DEBUG
 #define __crtl_dbg(fmt...) crtl_print_debug(fmt)
#else
 #define __crtl_dbg(fmt...)
#endif




_api inline int crtl_printfp(FILE *fp, crtl_log_level_t level, int prefix, int _unused colorful,
                          char *_file, const char *_func, const unsigned int _line, 
                          const char *fmt, ...);
_api inline int crtl_memprint(FILE* fp, void *const addr, unsigned int size);

_api inline int crtl_print_failure(const char *file, int line, const char *format, ...);
_api inline void crtl_print_exit_failure(int status, const char *file, int line, const char *format, ...);


#endif /*<__CRTL_LOG_H>*/

