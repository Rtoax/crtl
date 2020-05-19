#include <crtl/log.h>
#include "crtl/linux/crtl_linux_host.h"


void demo_gethostname()
{
    char str[256];
    crtl_print_alert("hostname %s\n", crtl_linux_hostname());
    crtl_print_alert("hostname %s\n", crtl_linux_hostname());
    crtl_print_alert("domainname %s\n", crtl_linux_domainname());
    crtl_print_alert("domainname %s\n", crtl_linux_domainname());
    crtl_print_alert("hostid %ld\n", crtl_linux_hostid());
}

int main()
{
    demo_gethostname();
    
    
    
    return 0;
}

//ENOTNAM

