#ifndef __CRTL_BITS_CORE_H
#define __CRTL_BITS_CORE_H 1


#include <sys/types.h>
#include <sys/socket.h>

#include "crtl/bits/crtl_time_def.h"

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



int crtl_getpid(void);
int crtl_getppid(void);
int crtl_getpriority(int pid, int* priority);
int crtl_setpriority(int pid, int priority); //CRTL_PRIORITY_XXX

int crtl_uname(crtl_utsname_t* buffer);


int crtl_open_cloexec(const char* path, int flags);
int crtl_dup2_cloexec(int oldfd, int newfd);


int crtl_fcntl_sockfdnonblk(int sockfd);

int crtl_fcntl_nonblk(int fd, int set);
int crtl_fcntl_cloexec(int fd, int set);

int crtl_ioctl_nonblk(int fd, int set); //crtl_ioctl_nonblk(fd, 1);
int crtl_ioctl_cloexec(int fd, int set);


int crtl_cwd(char* buffer, size_t* size);
int crtl_chdir(const char* dir);
int crtl_tmpdir(char* buffer, size_t* size);


int crtl_getrusage(crtl_rusage_t* rusage);

ssize_t crtl_recvmsg(int fd, struct msghdr* msg, int flags);


#endif /*<__CRTL_BITS_CORE_H>*/
