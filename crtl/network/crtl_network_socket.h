#ifndef __CRTL_NETWORK_SOCKET_H
#define __CRTL_NETWORK_SOCKET_H 1


#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <poll.h>
#include <limits.h> /*for OPEN_MAX*/
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#include <sys/types.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <stdint.h>
#include <sys/socket.h>

#include "crtl/network/crtl_network_sockaddrinfo.h"
#include "crtl/network/crtl_network_sockios.h"
#include "crtl/network/crtl_network_sockopt.h"



typedef struct in6_addr             in6_addr_t;
typedef struct sockaddr             sockaddr_t;
typedef struct sockaddr_in          sockaddr_in_t;
typedef struct sockaddr_in6         sockaddr_in6_t;
//typedef struct sockaddr_dl          sockaddr_dl_t;    //if_dl: datalink layer
typedef struct sockaddr_ll          sockaddr_ll_t;
typedef struct sockaddr_llc         sockaddr_llc_t;
typedef struct sockaddr_un          sockaddr_un_t;
typedef struct sockaddr_storage     sockaddr_storage_t;
//typedef struct sockaddr_alg         sockaddr_alg_t;     //if_alg: User-space algorithm interface


int crtl_connect_nonblk(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec);
int crtl_connect_timeout(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec);



#endif /*<__CRTL_NETWORK_SOCKET_H>*/

