#ifndef __CRTL_TCP_IP_ETHER_H
#define __CRTL_TCP_IP_ETHER_H 1


#include "crtl/tcp-ip/bits/crtl_ether_pkt.h"
#include "crtl/easy/attribute.h"


#define CRTL_ETH_HRD_SZ	sizeof(crtl_ethhdr_t)
#define CRTL_ETH_ALEN	6	/* ether address len */

#define CRTL_ETH_P_IP     0x0800
#define CRTL_ETH_P_ARP    0x0806
#define CRTL_ETH_P_RARP   0x8035


#define crtl_macfmt(ha)  (ha)[0], (ha)[1], (ha)[2], (ha)[3], (ha)[4], (ha)[5]
#define CRTL_MACFMT      "%02x:%02x:%02x:%02x:%02x:%02x"


typedef struct {
    unsigned char   h_dst[CRTL_ETH_ALEN];   /* destination eth addr	*/
    unsigned char   h_src[CRTL_ETH_ALEN];   /* source ether addr	*/
    unsigned short  h_proto;                /* packet type ID field	*/
}_packed crtl_ethhdr_t;

typedef struct {
    crtl_ethhdr_t  eth_hdr;     /* ether header */
	void       *eth_data;	    /* data field */
}_packed crtl_ethmsg_t;





#endif /*<__CRTL_TCP_IP_ETHER_H>*/

