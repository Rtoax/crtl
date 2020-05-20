#include "config.h"
#include "crtl/tcp-ip/sctp.h"


#if !HAVE_NETINET_SCTP_H || !HAVE_LIBSCTP
 #warning  Don not have <netinet/sctp.h> file in "/usr/include" or "/usr/local/include"
#endif




