#include "crtl/crtl_log.h"
#include "crtl/crtl_types.h"
#include "crtl/linux/crtl_linux_host.h"
#include "crtl/easy/macro.h"


const char const *crtl_linux_hostname()
{
    static char hostname[256] = {0};

    CALL_ONCE(hostname) {
	    gethostname(hostname, 256);
    }
	return hostname;
}

const char const *crtl_linux_domainname()
{
    static char domainname[256] = {0};

    CALL_ONCE(domainname) {
	    getdomainname(domainname, 256);
    }
	return domainname;
}


long crtl_linux_hostid()
{
    return gethostid();
}
