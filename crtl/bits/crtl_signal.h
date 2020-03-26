#ifndef __CRTL_BITS_SIGNAL_H
#define __CRTL_BITS_SIGNAL_H 1

#define _GNU_SOURCE
#include <signal.h>
#include "crtl/easy/macro.h"


typedef sigset_t __crtl_sigset_t;


#define __CRTL_SIGSET_ZERO(pset)          typecheck(__crtl_sigset_t*, pset);sigemptyset(pset)
#define __CRTL_SIGSET_FILL(pset)          typecheck(__crtl_sigset_t*, pset);sigfillset(pset)
#define __CRTL_SIGSET_SET(pset, signum)   typecheck(__crtl_sigset_t*, pset);sigaddset(pset, signum)
#define __CRTL_SIGSET_CLR(pset, signum) typecheck(__crtl_sigset_t*, pset);sigdelset(pset, signum)
#define __CRTL_SIGSET_ISSET(pset, signum) ({typecheck(__crtl_sigset_t*, pset);sigismember(pset, signum);})
 
#ifdef _GNU_SOURCE
#define __CRTL_SIGSET_ISEMPTY(pset)          ({typecheck(__crtl_sigset_t*, pset);sigisemptyset(pset);})
#define __CRTL_SIGSET_OR(pdstset, pset1, pset2) ({\
                                                typecheck(__crtl_sigset_t*, pdstset);\
                                                typecheck(__crtl_sigset_t*, pset1);\
                                                typecheck(__crtl_sigset_t*, pset2);\
                                                sigorset(pdstset, pset1, pset2);})
#define __CRTL_SIGSET_AND(pdstset, pset1, pset2) ({\
                                                typecheck(__crtl_sigset_t*, pdstset);\
                                                typecheck(__crtl_sigset_t*, pset1);\
                                                typecheck(__crtl_sigset_t*, pset2);\
                                                sigandset(pdstset, pset1, pset2);})
#else
#define __CRTL_SIGSET_ISEMPTY(pset)          1
#define __CRTL_SIGSET_OR(pdstset, pset1, pset2) 
#define __CRTL_SIGSET_AND(pdstset, pset1, pset2) 
#endif//_GNU_SOURCE

#define __CRTL_SIGSET_WAIT(pset) typecheck(__crtl_sigset_t*, pset);sigsuspend(pset)



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////     ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// ///  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// ///  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////     ///// //////////////////////////////////////////////////////////////////////////////////////////////////////
///////  //////     ////////////////////////////////////////////////////////////////////////////////////////////////////
/////// /  ////// //////////////////////////////////////////////////////////////////////////////////////////////////////
/////// //  /////  /////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef	_SIGNAL_H
typedef void (*sighandler_t) (int);
typedef void (*__sighandler_t) (int);

/*  */
sighandler_t signal(int signum, sighandler_t handler);
int kill(pid_t pid, int sig);

#if _BSD_SOURCE || _XOPEN_SOURCE >= 500 || _XOPEN_SOURCE && _XOPEN_SOURCE_EXTENDED
/* 向组发送信号，pgrp==0，If  pgrp is 0, killpg() sends the signal to the calling process's process group. */
int killpg(int pgrp, int sig);
#endif//_BSD_SOURCE || _XOPEN_SOURCE >= 500 || _XOPEN_SOURCE && _XOPEN_SOURCE_EXTENDED

int raise(int sig);/* 发送信号，kill(getpid(), sig);， pthread_kill(pthread_self(), sig);*/

#ifdef __USE_XOPEN2K8
void psignal(int sig, const char *s);
void psiginfo(const siginfo_t *pinfo, const char *s);
extern const char *const sys_siglist[];
#endif /* POSIX 2008.  */

#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE
#define _SIGSET_NWORDS (1024 / (8 * sizeof (unsigned long int)))
typedef struct
{
  unsigned long int __val[_SIGSET_NWORDS];
} __sigset_t;

int sigsuspend(const sigset_t *mask); //等待一个信号
typedef __sigset_t sigset_t;

#endif //_POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE

#if _BSD_SOURCE
struct sigvec {
   void (*sv_handler)(int); /* Signal disposition */
   int    sv_mask;          /* Signals to be blocked in handler */
   int    sv_flags;         /* Flags */
};
int sigvec(int sig, struct sigvec *vec, struct sigvec *ovec);
int sigmask(int signum);
int sigblock(int mask);
int sigsetmask(int mask);
int siggetmask(void);
#endif //_BSD_SOURCE

#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigdelset(sigset_t *set, int signum);
int sigismember(const sigset_t *set, int signum);
#endif//_POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE

#ifdef _GNU_SOURCE
int sigisemptyset(sigset_t *set);
//       returns 1 if set contains no signals, and 0 otherwise.
int sigorset(sigset_t *dest, sigset_t *left, sigset_t *right);
//       places the union of the sets left and right in dest.
int sigandset(sigset_t *dest, sigset_t *left, sigset_t *right);
//       places the intersection of the sets left and right in dest.
//sigorset() and sigandset() return 0 on success, and -1 on failure.
#endif//_GNU_SOURCE

#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
#define SIG_BLOCK
//The set of blocked signals is the union of the current set and the set argument.
#define SIG_UNBLOCK
//The  signals  in  set  are  removed from the current set of blocked signals.  It is permissible to
//attempt to unblock a signal which is not blocked.
#define SIG_SETMASK
//The set of blocked signals is set to the argument set.
#endif//_POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE

#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE
struct sigaction {
   void     (*sa_handler)(int);
   void     (*sa_sigaction)(int, siginfo_t *, void *);
   sigset_t   sa_mask;
   int        sa_flags;
   void     (*sa_restorer)(void);
};

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int sigpending(sigset_t *set);
int sigwait(const sigset_t *set, int *sig);
int sigwaitinfo(const sigset_t *set, siginfo_t *info);
int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout);
int sigqueue(pid_t pid, int sig, const union sigval value);



#endif //_POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE

extern const char *const _sys_siglist[_NSIG];
extern const char *const sys_siglist[_NSIG];
struct sigcontext
{
    int sc_onstack;
    __sigset_t sc_mask;
    /* Registers and such.  */
};

int sigreturn(unsigned long __unused);

int siginterrupt(int sig, int flag);
/* Structure describing a signal stack (obsolete).  */
struct sigstack
{
    void *ss_sp;		/* Signal stack pointer.  */
    int ss_onstack;		/* Nonzero if executing on this stack.  */
};
typedef struct sigstack stack_t;
int sigaltstack(const stack_t *ss, stack_t *oss);

#if _XOPEN_SOURCE >= 500 || _XOPEN_SOURCE && _XOPEN_SOURCE_EXTENDED
sighandler_t sigset(int sig, sighandler_t disp);
int sighold(int sig);
int sigrelse(int sig);
int sigignore(int sig);
#endif //_XOPEN_SOURCE >= 500 || _XOPEN_SOURCE && _XOPEN_SOURCE_EXTENDED


#ifndef	_BITS_SIGNUM_GENERIC_H
/* We define here all the signal names listed in POSIX (1003.1-2008);
   as of 1003.1-2013, no additional signals have been added by POSIX.
   We also define here signal names that historically exist in every
   real-world POSIX variant (e.g. SIGWINCH).

   Signals in the 1-15 range are defined with their historical numbers.
   For other signals, we use the BSD numbers.
   There are two unallocated signal numbers in the 1-31 range: 7 and 29.
   Signal number 0 is reserved for use as kill(pid, 0), to test whether
   a process exists without sending it a signal.  */

/* ISO C99 signals.  */
#define	SIGINT		2	/* Interactive attention signal.  */
#define	SIGILL		4	/* Illegal instruction.  */
#define	SIGABRT		6	/* Abnormal termination.  */
#define	SIGFPE		8	/* Erroneous arithmetic operation.  */
#define	SIGSEGV		11	/* Invalid access to storage.  */
#define	SIGTERM		15	/* Termination request.  */

/* Historical signals specified by POSIX. */
#define	SIGHUP		1	/* Hangup.  */
#define	SIGQUIT		3	/* Quit.  */
#define	SIGTRAP		5	/* Trace/breakpoint trap.  */
#define	SIGKILL		9	/* Killed.  */
#define SIGBUS		10	/* Bus error.  */
#define	SIGSYS		12	/* Bad system call.  */
#define	SIGPIPE		13	/* Broken pipe.  */
#define	SIGALRM		14	/* Alarm clock.  */

/* New(er) POSIX signals (1003.1-2008, 1003.1-2013).  */
#define	SIGURG		16	/* Urgent data is available at a socket.  */
#define	SIGSTOP		17	/* Stop, unblockable.  */
#define	SIGTSTP		18	/* Keyboard stop.  */
#define	SIGCONT		19	/* Continue.  */
#define	SIGCHLD		20	/* Child terminated or stopped.  */
#define	SIGTTIN		21	/* Background read from control terminal.  */
#define	SIGTTOU		22	/* Background write to control terminal.  */
#define	SIGPOLL		23	/* Pollable event occurred (System V).  */
#define	SIGXCPU		24	/* CPU time limit exceeded.  */
#define	SIGXFSZ		25	/* File size limit exceeded.  */
#define	SIGVTALRM	26	/* Virtual timer expired.  */
#define	SIGPROF		27	/* Profiling timer expired.  */
#define	SIGUSR1		30	/* User-defined signal 1.  */
#define	SIGUSR2		31	/* User-defined signal 2.  */

/* Nonstandard signals found in all modern POSIX systems
   (including both BSD and Linux).  */
#define	SIGWINCH	28	/* Window size change (4.3 BSD, Sun).  */

/* Archaic names for compatibility.  */
#define	SIGIO		SIGPOLL	/* I/O now possible (4.2 BSD).  */
#define	SIGIOT		SIGABRT	/* IOT instruction, abort() on a PDP-11.  */
#define	SIGCLD		SIGCHLD	/* Old System V name */

/* Not all systems support real-time signals.  bits/signum.h indicates
   that they are supported by overriding __SIGRTMAX to a value greater
   than __SIGRTMIN.  These constants give the kernel-level hard limits,
   but some real-time signals may be used internally by glibc.  Do not
   use these constants in application code; use SIGRTMIN and SIGRTMAX
   (defined in signal.h) instead.  */
#define __SIGRTMIN	32
#define __SIGRTMAX	__SIGRTMIN

/* Biggest signal number + 1 (including real-time signals).  */
#define _NSIG		(__SIGRTMAX + 1)
#endif//_BITS_SIGNUM_GENERIC_H

#ifndef __siginfo_t_defined
union sigval
{
  int sival_int;
  void *sival_ptr;
};
typedef union sigval sigval_t;
typedef union sigval __sigval_t;
typedef struct
{
    int si_signo;		/* Signal number.  */
    int si_errno;		/* If non-zero, an errno value associated with
    	   this signal, as defined in <errno.h>.  */
    int si_code;		/* Signal code.  */
    __pid_t si_pid;		/* Sending process ID.  */
    __uid_t si_uid;		/* Real user ID of sending process.  */
    void *si_addr;		/* Address of faulting instruction.  */
    int si_status;		/* Exit value or signal.  */
    long int si_band;		/* Band event for SIGPOLL.  */
    __sigval_t si_value;	/* Signal value.  */
} siginfo_t;
typedef struct {
   int      si_signo;    /* Signal number */
   int      si_errno;    /* An errno value */
   int      si_code;     /* Signal code */
   int      si_trapno;   /* Trap number that caused
                            hardware-generated signal
                            (unused on most architectures) */
   pid_t    si_pid;      /* Sending process ID */
   uid_t    si_uid;      /* Real user ID of sending process */
   int      si_status;   /* Exit value or signal */
   clock_t  si_utime;    /* User time consumed */
   clock_t  si_stime;    /* System time consumed */
   sigval_t si_value;    /* Signal value */
   int      si_int;      /* POSIX.1b signal */
   void    *si_ptr;      /* POSIX.1b signal */
   int      si_overrun;  /* Timer overrun count; POSIX.1b timers */
   int      si_timerid;  /* Timer ID; POSIX.1b timers */
   void    *si_addr;     /* Memory location which caused fault */
   long     si_band;     /* Band event (was int in
                            glibc 2.3.2 and earlier) */
   int      si_fd;       /* File descriptor */
   short    si_addr_lsb; /* Least significant bit of address
                            (since Linux 2.6.32) */
}siginfo_t;

#endif


#endif//_SIGNAL_H





#endif /*<__CRTL_BITS_SIGNAL_H>*/


