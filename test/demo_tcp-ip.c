#include <stdarg.h>

#include "crtl/signal.h"
#include <crtl/mask.h>
#include <crtl/log.h>
#include <crtl/string.h>
#include "crtl/tcp-ip/crtl_ether.h"
#include "crtl/tcp-ip/crtl_icmp.h"

void demo_tcp_ip_ether()
{
    unsigned char mac[CRTL_ETH_ALEN] = {0};

    crtl_print_notice(CRTL_MACFMT"\n", crtl_macfmt(mac));

    crtl_ether_mac("eth0", mac);

    
    crtl_print_notice(CRTL_MACFMT"\n", crtl_macfmt(mac));

    char msg[] = "abcdefghijklmnopqrstuvwxyz1234567890";

#define MY_WINDOWS  "28:6E:D4:88:C7:2C"
    while(1)
    {   
        crtl_ether_sendto(MY_WINDOWS, "eth0", 0x1204, msg, strlen(msg));
        sleep(1);
    }
    
}

int display(void *handle, const char *fmt, ...)
{   
    va_list va;
    va_start(va, fmt);
    fprintf((FILE*)handle, fmt, va);

    va_end(va);
}


void demo_tcp_ip_icmp()
{
    crtl_icmp_ping("10.170.6.24", 2, 1, "RONG", sizeof("RONG"), display, stdout);
    crtl_icmp_ping("10.170.6.24", 2, 1, "TAO", sizeof("TAO"), display, stdout);
    crtl_icmp_ping("10.170.6.24", 2, 1, NULL, 0, display, stdout);

    CRTL_ICMPING("10.170.6.24");
    
    sleep(1);
}


void demo_tcp_ip_telnet_test1()
{
    
}


int main()
{
//    demo_tcp_ip_ether();
//    demo_tcp_ip_icmp();
    
    return 0;
}




