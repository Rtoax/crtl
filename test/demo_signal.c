#include "crtl/signal.h"
#include <crtl/mask.h>
#include <crtl/log.h>

void demo_signal_posix_sigset()
{
    crtl_sigset_t sigset1;
    
    CRTL_SIGSET_FILL(&sigset1);
    crtl_memprint(stdout, &sigset1, sizeof(crtl_sigset_t));
    
    CRTL_SIGSET_CLR(&sigset1, SIGINT);
    crtl_memprint(stdout, &sigset1, sizeof(crtl_sigset_t));
    
    CRTL_SIGSET_ZERO(&sigset1);
    crtl_memprint(stdout, &sigset1, sizeof(crtl_sigset_t));

    if(CRTL_SIGSET_ISEMPTY(&sigset1)) {
        crtl_print_notice("Empty SIgset.\n");
    }
    
    CRTL_SIGSET_SET(&sigset1, SIGINT);
    crtl_memprint(stdout, &sigset1, sizeof(crtl_sigset_t));

    if(CRTL_SIGSET_ISEMPTY(&sigset1)) {
        crtl_print_notice("Empty SIgset.\n");
    }
    
}

int main()
{
    demo_signal_posix_sigset();

    
    return 0;
}


