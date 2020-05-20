#include <crtl/log.h>
#include "crtl/bits/core.h"


void demo_gethostname()
{
    char str[256];
    crtl_print_alert("hostname %s\n", crtl_hostname());
    crtl_print_alert("hostname %s\n", crtl_hostname());
    crtl_print_alert("domainname %s\n", crtl_domainname());
    crtl_print_alert("domainname %s\n", crtl_domainname());
    crtl_print_alert("hostid %ld\n", crtl_hostid());
}

int main()
{
    demo_gethostname();
    
    
    
    return 0;
}

//ENOTNAM

