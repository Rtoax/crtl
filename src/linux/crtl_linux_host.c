#include <string.h>
#include <unistd.h>

#include "crtl/log.h"
#include "crtl/bits/types_basic.h"
#include "crtl/bits/core.h"
#include "crtl/easy/macro.h"


const char const *crtl_hostname()
{
    static char hostname[256] = {0};

    CALL_ONCE(hostname) {
	    gethostname(hostname, 256);
    }
	return hostname;
}

const char const *crtl_domainname()
{
    static char domainname[256] = {0};

    CALL_ONCE(domainname) {
	    getdomainname(domainname, 256);
    }
	return domainname;
}


long crtl_hostid()
{
    return gethostid();
}

