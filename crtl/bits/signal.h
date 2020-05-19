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







#endif /*<__CRTL_BITS_SIGNAL_H>*/


