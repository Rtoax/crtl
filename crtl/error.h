#ifndef __CRTL_BITS_ERRNO_H
#define __CRTL_BITS_ERRNO_H 1

#include <errno.h>
#include <sys/types.h>


#define CRTL_ESUCCESS       0//Success
#define CRTL_E2BIG	        1//argument list too long"},
#define CRTL_EACCES	        2//permission denied"},
#define CRTL_EADDRINUSE	    3//address already in use"},
#define CRTL_EADDRNOTAVAIL	4//address not available"},
#define CRTL_EAFNOSUPPORT	5//address family not supported"},
#define CRTL_EAGAIN	        6//resource temporarily unavailable"},
#define CRTL_EAI_ADDRFAMILY	7//address family not supported"},
#define CRTL_EAI_AGAIN	    8//temporary failure"},
#define CRTL_EAI_BADFLAGS	9//bad ai_flags value"},
#define CRTL_EAI_BADHINTS	10//invalid value for hints"},
#define CRTL_EAI_CANCELED	11//request canceled"},
#define CRTL_EAI_FAIL       12//permanent failure"},
#define CRTL_EAI_FAMILY	    13//ai_family not supported"},
#define CRTL_EAI_MEMORY	    14//out of memory"},
#define CRTL_EAI_NODATA	    15//no address"},
#define CRTL_EAI_NONAME	    16//unknown node or service"},
#define CRTL_EAI_OVERFLOW	17//argument buffer overflow"},
#define CRTL_EAI_PROTOCOL	18//resolved protocol is unknown"},
#define CRTL_EAI_SERVICE	19//service not available for socket type"},
#define CRTL_EAI_SOCKTYPE	20//socket type not supported"},
#define CRTL_EALREADY	    21//connection already in progress"},
#define CRTL_EBADF	        22//bad file descriptor"},
#define CRTL_EBUSY	        23//resource busy or locked"},
#define CRTL_ECANCELED	    24//operation canceled"},
#define CRTL_ECHARSET	    25//invalid Unicode character"},
#define CRTL_ECONNABORTED	26//software caused connection abort"},
#define CRTL_ECONNREFUSED	27//connection refused"},
#define CRTL_ECONNRESET	    28//connection reset by peer"},
#define CRTL_EDESTADDRREQ	29//destination address required"},
#define CRTL_EEXIST	        30//file already exists"},
#define CRTL_EFAULT	        31//bad address in system call argument"},
#define CRTL_EFBIG	        32//file too large"},
#define CRTL_EHOSTUNREACH	33//host is unreachable"},
#define CRTL_EINTR	        34//interrupted system call"},
#define CRTL_EINVAL	        35//invalid argument"},
#define CRTL_EIO	        36//i/o error"},
#define CRTL_EISCONN	    37//socket is already connected"},
#define CRTL_EISDIR	        38//illegal operation on a directory"},
#define CRTL_ELOOP	        39//too many symbolic links encountered"},
#define CRTL_EMFILE	        40//too many open files"},
#define CRTL_EMSGSIZE	    41//message too long"},
#define CRTL_ENAMETOOLONG	42//name too long"},
#define CRTL_ENETDOWN	    43//network is down"},
#define CRTL_ENETUNREACH	44//network is unreachable"},
#define CRTL_ENFILE	        45//file table overflow"},
#define CRTL_ENOBUFS	    46//no buffer space available"},
#define CRTL_ENODEV	        47//no such device"},
#define CRTL_ENOENT	        48//no such file or directory"},
#define CRTL_ENOMEM	        49//not enough memory"},
#define CRTL_ENONET	        50//machine is not on the network"},
#define CRTL_ENOPROTOOPT	51//protocol not available"},
#define CRTL_ENOSPC	        52//no space left on device"},
#define CRTL_ENOSYS	        53//function not implemented"},
#define CRTL_ENOTCONN	    54//socket is not connected"},
#define CRTL_ENOTDIR	    55//not a directory"},
#define CRTL_ENOTEMPTY	    56//directory not empty"},
#define CRTL_ENOTSOCK	    57//socket operation on non-socket"},
#define CRTL_ENOTSUP	    58//operation not supported on socket"},
#define CRTL_EPERM	        59//operation not permitted"},
#define CRTL_EPIPE	        60//broken pipe"},
#define CRTL_EPROTO	        61//protocol error"},
#define CRTL_EPROTONOSUPPORT	62//protocol not supported"},
#define CRTL_EPROTOTYPE	    63//protocol wrong type for socket"},
#define CRTL_ERANGE	        64//result too large"},
#define CRTL_EROFS	        65//read-only file system"},
#define CRTL_ESHUTDOWN	    66//cannot send after transport endpoint shutdown"},
#define CRTL_ESPIPE	        67//invalid seek"},
#define CRTL_ESRCH	        68//no such process"},
#define CRTL_ETIMEDOUT	    69//connection timed out"},
#define CRTL_ETXTBSY	    70//text file is busy"},
#define CRTL_EXDEV	        71//cross-device link not permitted"},
#define CRTL_UNKNOWN	    72//unknown error"},
#define CRTL_EOF	        73//end of file"},
#define CRTL_ENXIO	        74//no such device or address"},
#define CRTL_EMLINK	        75//too many links"},



/**
 * return string describing error number
 *
 * @param err: error number, CRTL_EXXXX
 * @return return string describing error number
 */
const char* crtl_strerror(int err);

/**
 * return string describing error number, thread safe
 *
 * @param err: error number, CRTL_EXXXX
 * @param buf: string describing error number
 * @param buflen: string describing length
 * @return return string describing error number
 */
char* crtl_strerror_r(int err, char* buf, size_t buflen);

/**
 * return error number name
 *
 * @param err: error number, CRTL_EXXXX
 * @return return error number name
 */
const char* crtl_err_name(int err);

/**
 * return error number name
 *
 * @param err: error number, CRTL_EXXXX
 * @param buf: error number name
 * @param buflen: error number name length
 * @return return error number name
 */
char* crtl_err_name_r(int err, char* buf, size_t buflen);




#endif /*<__CRTL_BITS_ERRNO_H>*/


