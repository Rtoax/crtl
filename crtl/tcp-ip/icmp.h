
#ifndef __CRTL_CRTL_ICMP_H
#define __CRTL_CRTL_ICMP_H 1


#define CRTL_ICMPING(ipv4) crtl_icmp_ping(ipv4, 1, 1, NULL, 0, NULL, NULL)


int crtl_icmp_ping(const char *ipv4, unsigned int timeout, int ntry, void *data, int data_len,
                    int (*display)(void *handle, const char *fmt, ...), void *display_handle);




#endif /*<__CRTL_CRTL_ICMP_H>*/
