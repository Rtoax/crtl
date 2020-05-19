#ifndef __CRTL_SIGNAL_H
#define __CRTL_SIGNAL_H 1

#include "crtl/bits/signal.h"


typedef __crtl_sigset_t crtl_sigset_t;


#define CRTL_SIGSET_ZERO(pset) __CRTL_SIGSET_ZERO(pset)  
#define CRTL_SIGSET_FILL(pset) __CRTL_SIGSET_FILL(pset)    
#define CRTL_SIGSET_SET(pset, signum) __CRTL_SIGSET_SET(pset, signum)
#define CRTL_SIGSET_CLR(pset, signum) __CRTL_SIGSET_CLR(pset, signum)
#define CRTL_SIGSET_ISSET(pset, signum)  __CRTL_SIGSET_ISSET(pset, signum) 
#define CRTL_SIGSET_ISEMPTY(pset)   __CRTL_SIGSET_ISEMPTY(pset)   
#define CRTL_SIGSET_OR(pdstset, pset1, pset2) __CRTL_SIGSET_OR(pdstset, pset1, pset2)
#define CRTL_SIGSET_AND(pdstset, pset1, pset2) __CRTL_SIGSET_AND(pdstset, pset1, pset2)


#define CRTL_SIGSUSPEND(pset) __CRTL_SIGSET_WAIT(pset) 







#endif /*<__CRTL_SIGNAL_H>*/
