#include <string.h>

#include "crtl/error.h"
#include "crtl/easy/attribute.h"
#include "crtl/easy/macro.h"

#include "crypto/expect.h"

#define _ERR(err_no, err_str) {err_no, #err_no, err_str}

const struct  {
    long int err_no;
    char *err_name;
    char *err_str;
}_unused __crtl_errno_string[] = {
_ERR(CRTL_ESUCCESS, "success"),
_ERR(CRTL_E2BIG, "argument list too long"),
_ERR(CRTL_EACCES, "permission denied"),
_ERR(CRTL_EADDRINUSE, "address already in use"),
_ERR(CRTL_EADDRNOTAVAIL, "address not available"),
_ERR(CRTL_EAFNOSUPPORT, "address family not supported"),
_ERR(CRTL_EAGAIN, "resource temporarily unavailable"),
_ERR(CRTL_EAI_ADDRFAMILY, "address family not supported"),
_ERR(CRTL_EAI_AGAIN, "temporary failure"),
_ERR(CRTL_EAI_BADFLAGS, "bad ai_flags value"),
_ERR(CRTL_EAI_BADHINTS, "invalid value for hints"),
_ERR(CRTL_EAI_CANCELED, "request canceled"),
_ERR(CRTL_EAI_FAIL, "permanent failure"),
_ERR(CRTL_EAI_FAMILY, "ai_family not supported"),
_ERR(CRTL_EAI_MEMORY, "out of memory"),
_ERR(CRTL_EAI_NODATA, "no address"),
_ERR(CRTL_EAI_NONAME, "unknown node or service"),
_ERR(CRTL_EAI_OVERFLOW, "argument buffer overflow"),
_ERR(CRTL_EAI_PROTOCOL, "resolved protocol is unknown"),
_ERR(CRTL_EAI_SERVICE, "service not available for socket type"),
_ERR(CRTL_EAI_SOCKTYPE, "socket type not supported"),
_ERR(CRTL_EALREADY, "connection already in progress"),
_ERR(CRTL_EBADF, "bad file descriptor"),
_ERR(CRTL_EBUSY, "resource busy or locked"),
_ERR(CRTL_ECANCELED, "operation canceled"),
_ERR(CRTL_ECHARSET, "invalid Unicode character"),
_ERR(CRTL_ECONNABORTED, "software caused connection abort"),
_ERR(CRTL_ECONNREFUSED, "connection refused"),
_ERR(CRTL_ECONNRESET, "connection reset by peer"),
_ERR(CRTL_EDESTADDRREQ, "destination address required"),
_ERR(CRTL_EEXIST, "file already exists"),
_ERR(CRTL_EFAULT, "bad address in system call argument"),
_ERR(CRTL_EFBIG, "file too large"),
_ERR(CRTL_EHOSTUNREACH, "host is unreachable"),
_ERR(CRTL_EINTR, "interrupted system call"),
_ERR(CRTL_EINVAL, "invalid argument"),
_ERR(CRTL_EIO, "i/o error"),
_ERR(CRTL_EISCONN, "socket is already connected"),
_ERR(CRTL_EISDIR, "illegal operation on a directory"),
_ERR(CRTL_ELOOP, "too many symbolic links encountered"),
_ERR(CRTL_EMFILE, "too many open files"),
_ERR(CRTL_EMSGSIZE, "message too long"),
_ERR(CRTL_ENAMETOOLONG, "name too long"),
_ERR(CRTL_ENETDOWN, "network is down"),
_ERR(CRTL_ENETUNREACH, "network is unreachable"),
_ERR(CRTL_ENFILE, "file table overflow"),
_ERR(CRTL_ENOBUFS, "no buffer space available"),
_ERR(CRTL_ENODEV, "no such device"),
_ERR(CRTL_ENOENT, "no such file or directory"),
_ERR(CRTL_ENOMEM, "not enough memory"),
_ERR(CRTL_ENONET, "machine is not on the network"),
_ERR(CRTL_ENOPROTOOPT, "protocol not available"),
_ERR(CRTL_ENOSPC, "no space left on device"),
_ERR(CRTL_ENOSYS, "function not implemented"),
_ERR(CRTL_ENOTCONN, "socket is not connected"),
_ERR(CRTL_ENOTDIR, "not a directory"),
_ERR(CRTL_ENOTEMPTY, "directory not empty"),
_ERR(CRTL_ENOTSOCK, "socket operation on non-socket"),
_ERR(CRTL_ENOTSUP, "operation not supported on socket"),
_ERR(CRTL_EPERM, "operation not permitted"),
_ERR(CRTL_EPIPE, "broken pipe"),
_ERR(CRTL_EPROTO, "protocol error"),
_ERR(CRTL_EPROTONOSUPPORT, "protocol not supported"),
_ERR(CRTL_EPROTOTYPE, "protocol wrong type for socket"),
_ERR(CRTL_ERANGE, "result too large"),
_ERR(CRTL_EROFS, "read-only file system"),
_ERR(CRTL_ESHUTDOWN, "cannot send after transport endpoint shutdown"),
_ERR(CRTL_ESPIPE, "invalid seek"),
_ERR(CRTL_ESRCH, "no such process"),
_ERR(CRTL_ETIMEDOUT, "connection timed out"),
_ERR(CRTL_ETXTBSY, "text file is busy"),
_ERR(CRTL_EXDEV, "cross-device link not permitted"),
_ERR(CRTL_UNKNOWN, "unknown error"),
_ERR(CRTL_EOF, "end of file"),
_ERR(CRTL_ENXIO, "no such device or address"),
_ERR(CRTL_EMLINK, "too many links"),

};

#undef _ERR


const char* crtl_strerror(int err)
{
    if(unlikely(err<0) || unlikely(err>CRTL_EMLINK)) {
        return "unknown error number";
    } 
    return __crtl_errno_string[err].err_str;
}

char* crtl_strerror_r(int err, char* buf, size_t buflen)
{
    if(unlikely(err<0) || unlikely(err>CRTL_EMLINK)) {
        return "unknown error number";
    }
    return strncpy(buf, __crtl_errno_string[err].err_str, buflen);
}

const char* crtl_err_name(int err)
{
    if(unlikely(err<0) || unlikely(err>CRTL_EMLINK)) {
        return NULL;
    } 
    return __crtl_errno_string[err].err_name;
}

char* crtl_err_name_r(int err, char* buf, size_t buflen)
{
    if(unlikely(err<0) || unlikely(err>CRTL_EMLINK)) {
        return NULL;
    } 
    return strncpy(buf, __crtl_errno_string[err].err_name, buflen);
}







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_ERRNO_H

#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO		 5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Argument list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */

#define EDOM	33	/* Math argument out of domain of function.  */
#define EILSEQ	84	/* Illegal byte sequence.  */
#define ERANGE	34	/* Math result not representable.  */

#define	EDEADLK		35	/* Resource deadlock would occur */
#define	ENAMETOOLONG	36	/* File name too long */
#define	ENOLCK		37	/* No record locks available */
#define	ENOSYS		38	/* Function not implemented */
#define	ENOTEMPTY	39	/* Directory not empty */
#define	ELOOP		40	/* Too many symbolic links encountered */
#define	EWOULDBLOCK	EAGAIN	/* Operation would block */
#define	ENOMSG		42	/* No message of desired type */
#define	EIDRM		43	/* Identifier removed */
#define	ECHRNG		44	/* Channel number out of range */
#define	EL2NSYNC	45	/* Level 2 not synchronized */
#define	EL3HLT		46	/* Level 3 halted */
#define	EL3RST		47	/* Level 3 reset */
#define	ELNRNG		48	/* Link number out of range */
#define	EUNATCH		49	/* Protocol driver not attached */
#define	ENOCSI		50	/* No CSI structure available */
#define	EL2HLT		51	/* Level 2 halted */
#define	EBADE		52	/* Invalid exchange */
#define	EBADR		53	/* Invalid request descriptor */
#define	EXFULL		54	/* Exchange full */
#define	ENOANO		55	/* No anode */
#define	EBADRQC		56	/* Invalid request code */
#define	EBADSLT		57	/* Invalid slot */

#define	EDEADLOCK	EDEADLK

#define	EBFONT		59	/* Bad font file format */
#define	ENOSTR		60	/* Device not a stream */
#define	ENODATA		61	/* No data available */
#define	ETIME		62	/* Timer expired */
#define	ENOSR		63	/* Out of streams resources */
#define	ENONET		64	/* Machine is not on the network */
#define	ENOPKG		65	/* Package not installed */
#define	EREMOTE		66	/* Object is remote */
#define	ENOLINK		67	/* Link has been severed */
#define	EADV		68	/* Advertise error */
#define	ESRMNT		69	/* Srmount error */
#define	ECOMM		70	/* Communication error on send */
#define	EPROTO		71	/* Protocol error */
#define	EMULTIHOP	72	/* Multihop attempted */
#define	EDOTDOT		73	/* RFS specific error */
#define	EBADMSG		74	/* Not a data message */
#define	EOVERFLOW	75	/* Value too large for defined data type */
#define	ENOTUNIQ	76	/* Name not unique on network */
#define	EBADFD		77	/* File descriptor in bad state */
#define	EREMCHG		78	/* Remote address changed */
#define	ELIBACC		79	/* Can not access a needed shared library */
#define	ELIBBAD		80	/* Accessing a corrupted shared library */
#define	ELIBSCN		81	/* .lib section in a.out corrupted */
#define	ELIBMAX		82	/* Attempting to link in too many shared libraries */
#define	ELIBEXEC	83	/* Cannot exec a shared library directly */
#define	EILSEQ		84	/* Illegal byte sequence */
#define	ERESTART	85	/* Interrupted system call should be restarted */
#define	ESTRPIPE	86	/* Streams pipe error */
#define	EUSERS		87	/* Too many users */
#define	ENOTSOCK	88	/* Socket operation on non-socket */
#define	EDESTADDRREQ	89	/* Destination address required */
#define	EMSGSIZE	90	/* Message too long */
#define	EPROTOTYPE	91	/* Protocol wrong type for socket */
#define	ENOPROTOOPT	92	/* Protocol not available */
#define	EPROTONOSUPPORT	93	/* Protocol not supported */
#define	ESOCKTNOSUPPORT	94	/* Socket type not supported */
#define	EOPNOTSUPP	95	/* Operation not supported on transport endpoint */
#define	EPFNOSUPPORT	96	/* Protocol family not supported */
#define	EAFNOSUPPORT	97	/* Address family not supported by protocol */
#define	EADDRINUSE	98	/* Address already in use */
#define	EADDRNOTAVAIL	99	/* Cannot assign requested address */
#define	ENETDOWN	100	/* Network is down */
#define	ENETUNREACH	101	/* Network is unreachable */
#define	ENETRESET	102	/* Network dropped connection because of reset */
#define	ECONNABORTED	103	/* Software caused connection abort */
#define	ECONNRESET	104	/* Connection reset by peer */
#define	ENOBUFS		105	/* No buffer space available */
#define	EISCONN		106	/* Transport endpoint is already connected */
#define	ENOTCONN	107	/* Transport endpoint is not connected */
#define	ESHUTDOWN	108	/* Cannot send after transport endpoint shutdown */
#define	ETOOMANYREFS	109	/* Too many references: cannot splice */
#define	ETIMEDOUT	110	/* Connection timed out */
#define	ECONNREFUSED	111	/* Connection refused */
#define	EHOSTDOWN	112	/* Host is down */
#define	EHOSTUNREACH	113	/* No route to host */
#define	EALREADY	114	/* Operation already in progress */
#define	EINPROGRESS	115	/* Operation now in progress */
#define	ESTALE		116	/* Stale file handle */
#define	EUCLEAN		117	/* Structure needs cleaning */
#define	ENOTNAM		118	/* Not a XENIX named type file */
#define	ENAVAIL		119	/* No XENIX semaphores available */
#define	EISNAM		120	/* Is a named type file */
#define	EREMOTEIO	121	/* Remote I/O error */
#define	EDQUOT		122	/* Quota exceeded */

#define	ENOMEDIUM	123	/* No medium found */
#define	EMEDIUMTYPE	124	/* Wrong medium type */
#define	ECANCELED	125	/* Operation Canceled */
#define	ENOKEY		126	/* Required key not available */
#define	EKEYEXPIRED	127	/* Key has expired */
#define	EKEYREVOKED	128	/* Key has been revoked */
#define	EKEYREJECTED	129	/* Key was rejected by service */

/* for robust mutexes */
#define	EOWNERDEAD	130	/* Owner died */
#define	ENOTRECOVERABLE	131	/* State not recoverable */

#define ERFKILL		132	/* Operation not possible due to RF-kill */

#define EHWPOISON	133	/* Memory page has hardware error */


#endif//_ERRNO_H
