#ifndef __CRTL_TCP_IP_ETHER_H
#define __CRTL_TCP_IP_ETHER_H 1


#include <netpacket/packet.h>


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
}__attribute__((packed)) crtl_ethhdr_t;

typedef struct {
    crtl_ethhdr_t  eth_hdr;     /* ether header */
	void       *eth_data;	    /* data field */
}__attribute__((packed)) crtl_ethmsg_t;



inline char * crtl_ether_proto(unsigned short proto);
inline void crtl_ether_hwacpy(void *dst, void *src);
inline void crtl_ether_hwaset(void *dst, int val);
inline int crtl_ether_hwacmp(void *hwa1, void *hwa2);
inline int crtl_ether_is_eth_multicast(unsigned char *hwa);
inline int crtl_ether_is_eth_broadcast(unsigned char *hwa);

inline int crtl_ether_strtomac(unsigned char mac[CRTL_ETH_ALEN], const char *str);
inline int crtl_ether_mac(const char *ether, unsigned char mac[CRTL_ETH_ALEN]);

/* send to ether: must be root user */
/**
 *  crtl_ether_sendto("28:6E:D4:88:C7:2C", "eth0", 0x1204, msg, strlen(msg));
 */
int crtl_ether_sendto(char *dst_mac, const char *ether, unsigned short proto, void * msg, unsigned int msg_len);



#endif /*<__CRTL_TCP_IP_ETHER_H>*/

