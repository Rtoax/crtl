#ifndef __CRTL_LOG_H
#define __CRTL_LOG_H 1

#include <stdio.h>

/**
 * CRTL LOG level
 */
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



/**
 * CRTL print
 * @param fmt: format, like printf(fmt, ...)'s format
 * @return charector number of print
 */
#define crtl_print_emerg(fmt...)    crtl_printfp(stderr, CRTL_LOG_EMERG, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_alert(fmt...)    crtl_printfp(stderr, CRTL_LOG_ALERT, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_crit(fmt...)     crtl_printfp(stderr, CRTL_LOG_CRIT, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_err(fmt...)      crtl_printfp(stderr, CRTL_LOG_ERR, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_warning(fmt...)  crtl_printfp(stdout, CRTL_LOG_WARNING, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_notice(fmt...)   crtl_printfp(stdout, CRTL_LOG_NOTICE, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_info(fmt...)     crtl_printfp(stdout, CRTL_LOG_INFO, 1, 1, __FILE__, __func__, __LINE__, fmt)
#define crtl_print_debug(fmt...)    crtl_printfp(stdout, CRTL_LOG_DEBUG, 1, 1, __FILE__, __func__, __LINE__, fmt)

/**
 * CRTL debug switch
 */
#define CRTL_DEBUG 1
#if CRTL_DEBUG
 #define __crtl_dbg(fmt...) crtl_print_debug(fmt)
#else
 #define __crtl_dbg(fmt...)
#endif



/**
 * CRTL original print function
 * @param fp: file pointer to print
 * @param level: log level
 * @param prefix: prefix for log
 * @param colorful: have level color hint
 * @param _file: file name
 * @param _func: function name
 * @param _line: line number
 * @param fmt: format, like printf(fmt, ...)'s format
 * @return charector number of print
 */
inline int crtl_printfp(FILE *fp, crtl_log_level_t level, int prefix, int colorful,
                          char *_file, const char *_func, const unsigned int _line, 
                          const char *fmt, ...);

/**
 * memory print to file pointer
 * @param fp: file pointer to print
 * @param addr: address of memory
 * @param size: size of memory
 * @return charector number of print
 */
inline int crtl_memprint(FILE* fp, void *const addr, unsigned int size);

/**
 * print failure with prefix "error: "
 * @param file: file name
 * @param line: line number
 * @param format: format, like printf(fmt, ...)'s format
 * @return charector number of print
 */
inline int crtl_print_failure(const char *file, int line, const char *format, ...);

/**
 * print failure with prefix "error: ", at the same time exit(status) will be called.
 * @param status: status for exit, exit(status)
 * @param file: file name
 * @param line: line number
 * @param format: format, like printf(fmt, ...)'s format
 * @return charector number of print
 */
inline void crtl_print_exit_failure(int status, const char *file, int line, const char *format, ...);


#endif /*<__CRTL_LOG_H>*/


