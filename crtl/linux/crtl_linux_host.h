#ifndef __CRTL_LINUX_HOST_H
#define __CRTL_LINUX_HOST_H 1

#include <string.h>
#include <unistd.h>


const char const *crtl_linux_hostname();
const char const *crtl_linux_domainname();
long crtl_linux_hostid();



#endif /*<__CRTL_LINUX_HOST_H>*/
