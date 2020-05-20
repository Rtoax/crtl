#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netpacket/packet.h>


#include "crtl/tcp-ip/ether.h"
#include "crtl/log.h"
#include "crtl/bits/types_basic.h"
#include "crtl/bits/core.h"



inline char * crtl_ether_proto(unsigned short proto)
{
	if (proto == CRTL_ETH_P_IP)
		return "IP";
	else if (proto == CRTL_ETH_P_ARP)
		return "ARP";
	else if (proto == CRTL_ETH_P_RARP)
		return "RARP";
	else
		return "Self defined";
}

inline void crtl_ether_hwacpy(void *dst, void *src)
{
	memcpy(dst, src, CRTL_ETH_ALEN);
}

inline void crtl_ether_hwaset(void *dst, int val)
{
	memset(dst, val, CRTL_ETH_ALEN);
}

inline int crtl_ether_hwacmp(void *hwa1, void *hwa2)
{
	return memcmp(hwa1, hwa2, CRTL_ETH_ALEN);
}


inline int crtl_ether_is_eth_multicast(unsigned char *hwa)
{
	return (hwa[0] & 0x01);
}

inline int crtl_ether_is_eth_broadcast(unsigned char *hwa)
{
	/*
	 * fast compare method
	 * ethernet mac broadcast is FF:FF:FF:FF:FF:FF
	 */
	return (hwa[0] & hwa[1] & hwa[2] & hwa[3] & hwa[4] & hwa[5]) == 0xff;
}

inline int crtl_ether_strtomac(unsigned char mac[CRTL_ETH_ALEN], const char *str)
{
    unsigned int __mac[CRTL_ETH_ALEN];
    int ret = sscanf(str, CRTL_MACFMT, &__mac[0], &__mac[1], &__mac[2], &__mac[3], &__mac[4], &__mac[5]);
    mac[0] = __mac[0];
    mac[1] = __mac[1];
    mac[2] = __mac[2];
    mac[3] = __mac[3];
    mac[4] = __mac[4];
    mac[5] = __mac[5];
    return ret;
}




inline int crtl_ether_mac(const char *ether, unsigned char mac[CRTL_ETH_ALEN])
{
    if(!ether)
    {
        crtl_print_err("null pointer error\n");
        return CRTL_ERROR;
    }
    
    char user[64] = {0};
    crtl_getcurrentusername(user);
    if(strcmp(user, "root") != 0)
    {
        crtl_print_err("must be root user.\n");
		return CRTL_ERROR;
    }
    
    int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sock_raw_fd < 0)
	{
		crtl_print_err("socket created failed\n");
		return CRTL_ERROR;
	}
    
    struct ifreq req; 
	strncpy(req.ifr_name, ether, IFNAMSIZ);
    
	req.ifr_ifru.ifru_flags = IFF_PROMISC;

    if(-1 == ioctl(sock_raw_fd, SIOCGIFINDEX, &req)) //get the port
	{
		crtl_print_err("ioctl: SIOCGIFINDEX error.\n");
		close(sock_raw_fd);
		return CRTL_ERROR;
	}
    strncpy(req.ifr_name, ether, IFNAMSIZ);
    if (-1 == ioctl(sock_raw_fd, SIOCGIFHWADDR, &req))
    {
        crtl_print_err("ioctl: SIOCGIFHWADDR error.\n");
        close(sock_raw_fd);
		return CRTL_ERROR;
    }

    memcpy(mac, req.ifr_hwaddr.sa_data, CRTL_ETH_ALEN);

    close(sock_raw_fd);
    
    return CRTL_SUCCESS;
}

/* send to ether: must be root user */
/**
 *  crtl_ether_sendto("28:6E:D4:88:C7:2C", "eth0", 0x1204, msg, strlen(msg));
 */
int crtl_ether_sendto(char *dst_mac, const char *ether, unsigned short proto, void * _unused msg, unsigned int msg_len)
{
    if(!dst_mac || !msg || !ether)
    {
        crtl_print_err("null pointer error\n");
        return CRTL_ERROR;
    }
    
    char user[64] = {0};
    crtl_getcurrentusername(user);
    if(strcmp(user, "root") != 0)
    {
        crtl_print_err("must be root user.\n");
		return CRTL_ERROR;
    }
    
    crtl_ethmsg_t *ethmsg = (crtl_ethmsg_t *)malloc(sizeof(crtl_ethhdr_t) + msg_len);
    ethmsg->eth_data = ethmsg + sizeof(crtl_ethhdr_t);

    int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sock_raw_fd < 0)
	{
		crtl_print_err("socket created failed\n");
		return CRTL_ERROR;
	}


    struct ifreq req; 
	strncpy(req.ifr_name, ether, IFNAMSIZ);
	req.ifr_ifru.ifru_flags = IFF_PROMISC;
    if(-1 == ioctl(sock_raw_fd, SIOCGIFINDEX, &req)) //get the port
	{
		crtl_print_err("ioctl: SIOCGIFINDEX error.\n");
		close(sock_raw_fd);
		return CRTL_ERROR;
	}
    
    struct sockaddr_ll sll;
	bzero(&sll,sizeof(sll));
    sll.sll_family = AF_PACKET;
	sll.sll_ifindex = req.ifr_ifindex?req.ifr_ifindex:1;
	//sll.sll_pkttype = PACKET_BROADCAST;

    unsigned char src_mac[CRTL_ETH_ALEN] = {0};
    crtl_ether_mac(ether, src_mac);
    
    ethmsg->eth_hdr.h_proto = htons(proto);
    crtl_ether_strtomac(ethmsg->eth_hdr.h_dst, dst_mac);
    //strtomac(ethmsg->eth_hdr.h_src, (char*)src_mac);
    memcpy(ethmsg->eth_hdr.h_src, src_mac, CRTL_ETH_ALEN);
    memcpy(ethmsg->eth_data, msg, msg_len);

    int len = sendto(sock_raw_fd, ethmsg, sizeof(crtl_ethhdr_t)+msg_len, 0, (struct sockaddr *)&sll, sizeof(sll));
    
    crtl_print_debug("%s: "CRTL_MACFMT" > "CRTL_MACFMT", bytes %d\n",ether, 
                        crtl_macfmt(ethmsg->eth_hdr.h_src), crtl_macfmt(ethmsg->eth_hdr.h_dst), len);

    free(ethmsg);
    close(sock_raw_fd);
    
    return len;
}

