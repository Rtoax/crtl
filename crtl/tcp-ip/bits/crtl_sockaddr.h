#ifndef __CRTL_TCP_IP_BITS_SOCKADDR_H
#define __CRTL_TCP_IP_BITS_SOCKADDR_H 1

#include <sys/types.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <stdint.h>
#include <sys/socket.h>
//#include <linux/types.h>
//#include <netinet/ether.h>
//#include <netinet/ether.h>

//#include <net/if_dl.h>
//#include <linux/types.h>
//#include <linux/if_alg.h>

#include <netdb.h>
#include <linux/types.h>


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



#endif /*<__CRTL_TCP_IP_BITS_SOCKADDR_H>*/
