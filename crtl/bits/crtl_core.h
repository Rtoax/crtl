#ifndef __CRTL_BITS_CORE_H
#define __CRTL_BITS_CORE_H 1


#include <sys/types.h>
#include <sys/socket.h>

#include "crtl/bits/time.h"

#if defined(__PASE__)
/* On IBM i PASE, the highest process priority is -10 */
# define CRTL_PRIORITY_LOW 39            // RUNPTY(99)
# define CRTL_PRIORITY_BELOW_NORMAL 15   // RUNPTY(50)
# define CRTL_PRIORITY_NORMAL 0          // RUNPTY(20)
# define CRTL_PRIORITY_ABOVE_NORMAL -4   // RUNPTY(12)
# define CRTL_PRIORITY_HIGH -7           // RUNPTY(6)
# define CRTL_PRIORITY_HIGHEST -10       // RUNPTY(1)
#else
# define CRTL_PRIORITY_LOW 19
# define CRTL_PRIORITY_BELOW_NORMAL 10
# define CRTL_PRIORITY_NORMAL 0
# define CRTL_PRIORITY_ABOVE_NORMAL -7
# define CRTL_PRIORITY_HIGH -14
# define CRTL_PRIORITY_HIGHEST -20
#endif


typedef struct crtl_utsname_struct {
  char sysname[256];
  char release[256];
  char version[256];
  char machine[256];
  /* This struct does not contain the nodename and domainname fields present in
     the utsname type. domainname is a GNU extension. Both fields are referred
     to as meaningless in the docs. */
}crtl_utsname_t;
  


typedef struct {
   t_timeval ru_utime; /* user CPU time used */
   t_timeval ru_stime; /* system CPU time used */
   uint64_t ru_maxrss;    /* maximum resident set size */
   uint64_t ru_ixrss;     /* integral shared memory size */
   uint64_t ru_idrss;     /* integral unshared data size */
   uint64_t ru_isrss;     /* integral unshared stack size */
   uint64_t ru_minflt;    /* page reclaims (soft page faults) */
   uint64_t ru_majflt;    /* page faults (hard page faults) */
   uint64_t ru_nswap;     /* swaps */
   uint64_t ru_inblock;   /* block input operations */
   uint64_t ru_oublock;   /* block output operations */
   uint64_t ru_msgsnd;    /* IPC messages sent */
   uint64_t ru_msgrcv;    /* IPC messages received */
   uint64_t ru_nsignals;  /* signals received */
   uint64_t ru_nvcsw;     /* voluntary context switches */
   uint64_t ru_nivcsw;    /* involuntary context switches */
} crtl_rusage_t;



#if defined(_AIX) || \
    defined(__APPLE__) || \
    defined(__DragonFly__) || \
    defined(__FreeBSD__) || \
    defined(__FreeBSD_kernel__) || \
    defined(__linux__) || \
    defined(__OpenBSD__) || \
    defined(__NetBSD__)
#define crtl_cloexec crtl_ioctl_cloexec
#define crtl_nonblock crtl_ioctl_nonblk
#else
#define crtl_cloexec crtl_fcntl_cloexec
#define crtl_nonblock crtl_fcntl_nonblk
#endif




/**
 * get process id
 * @param void
 * @return the  process ID of the calling process
 */
int crtl_getpid(void);

/**
 * get parent process id
 * @param void
 * @return returns the process ID of the parent of the calling process
 */
int crtl_getppid(void);


    
/**
 * get program scheduling priority
 * @param void
 * @return program scheduling priority
 */
int crtl_getpriority(int pid, int* priority);  

/**
 * set program scheduling priority
 * @param void
 * @return error number
 */
int crtl_setpriority(int pid, int priority); //CRTL_PRIORITY_XXX


/**
 * get the name of the current system
 * @param buffer: buffer
 * @return error number
 */
int crtl_uname(crtl_utsname_t* buffer);


/**
 * open file with The caller can force the close-on-exec 
 * flag to be set  for  the  new  file  descriptor
 *
 * @param path: file path
 * @param flags: int open(const char *pathname, int flags);
 *                  O_CREAT|O_EXCL|O_NOCTTY|O_TRUNC|O_APPEND|O_NONBLOCK|
 *                  O_SYNC|O_NOFOLLOW|O_DIRECTORY|O_LARGEFILE
 * @return new  file  descriptor
 */
int crtl_open_cloexec(const char* path, int flags);

/**
 * duplicate a file descriptor with The caller can force the close-on-exec 
 * flag to be set  for  the  new  file  descriptor
 *
 * @param oldfd: old  file  descriptor
 * @param newfd: new  file  descriptor
 * @return error number if failed, new  file  descriptor if success
 */
int crtl_dup2_cloexec(int oldfd, int newfd);


/**
 * set file descriptor to non block
 *
 * @param sockfd: file  descriptor
 * @return error number
 */
int crtl_fcntl_sockfdnonblk(int sockfd);


/**
 * set file descriptor to non block
 *
 * @param fd: file  descriptor
 * @param set: if(set) -> nonblock; else block
 * @return error number
 */
int crtl_fcntl_nonblk(int fd, int set);

/**
 * set file descriptor to close-on-exec 
 *
 * @param fd: file  descriptor
 * @param set: if(set) -> nonclose-on-exec ; else close-on-exec 
 * @return error number
 */
int crtl_fcntl_cloexec(int fd, int set);

/**
 * set file descriptor to non block
 *
 * @param fd: file  descriptor
 * @param set: if(set) -> nonblock; else block
 * @return error number
 */
int crtl_ioctl_nonblk(int fd, int set); //crtl_ioctl_nonblk(fd, 1);

/**
 * set file descriptor to close-on-exec 
 *
 * @param fd: file  descriptor
 * @param set: if(set) -> nonclose-on-exec ; else close-on-exec 
 * @return error number
 */
int crtl_ioctl_cloexec(int fd, int set);



/**
 * get current work directory
 * @param dir: directory
 * @param size: size of string of directory buffer
 * @return error number
 */
int crtl_cwd(char* buffer, size_t* size);

/**
 * change current work directory
 * @param dir: directory
 * @return error number
 */
int crtl_chdir(const char* dir);

/**
 * get temperaory directory
 * @param dir: directory
 * @param size: size of string of directory buffer
 * @return error number
 */
int crtl_tmpdir(char* buffer, size_t* size);

/**
 * get resource usage
 * @param rusage: resource usage buffer
 * @return error number
 */
int crtl_getrusage(crtl_rusage_t* rusage);

    
/**
 * receive a message from a socket
 * The recvfrom() and recvmsg() calls are used to receive messages from a socket, 
 * and may be used to receive data on a socket whether or not it is connection-oriented.
 *
 * @param fd: file  descriptor
 * @param msg: The recvmsg() call uses a msghdr structure to minimize the number of 
 *              directly supplied  arguments.   This structure is defined as follows in <sys/socket.h>.
 *              struct iovec {                    // Scatter/gather array items 
 *                 void  *iov_base;              // Starting address 
 *                 size_t iov_len;               // Number of bytes to transfer 
 *              };
 *              
 *              struct msghdr {
 *                 void         *msg_name;       // optional address 
 *                 socklen_t     msg_namelen;    // size of address 
 *                 struct iovec *msg_iov;        // scatter/gather array 
 *                 size_t        msg_iovlen;     // # elements in msg_iov 
 *                 void         *msg_control;    // ancillary data, see below 
 *                 size_t        msg_controllen; // ancillary data buffer len 
 *                 int           msg_flags;      // flags on received message 
 *              };
 *              
 * @param flags: ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags)'s flags
 * @return receive message size
 */
ssize_t crtl_recvmsg(int fd, struct msghdr* msg, int flags);

/* close() on macos has the "interesting" quirk that it fails with EINTR
 * without closing the file descriptor when a thread is in the cancel state.
 * That's why libuv calls close$NOCANCEL() instead.
 *
 * glibc on linux has a similar issue: close() is a cancellation point and
 * will unwind the thread when it's in the cancel state. Work around that
 * by making the system call directly. Musl libc is unaffected.
 */
int crtl_close_nocancel(int fd);





#endif /*<__CRTL_BITS_CORE_H>*/
