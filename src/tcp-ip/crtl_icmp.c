
#ifndef __USE_BSD
#define __USE_BSD
#endif

#include <sys/time.h>

#include <netdb.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/timerfd.h>
#include <sys/stat.h>
#include <sys/file.h>

#include <unistd.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h> //__USE_BSD


#include "crtl/bits/types_basic.h"

#include "crtl/log.h"
#include "crtl/easy/attribute.h"
#include "crtl/tcp-ip/icmp.h"
#include "crtl/network/checksum.h"
#include "crtl/time.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/
#define CRTL_ICMP_BUF_SIZE           256
#define CRTL_ICMP_RECVBUF_SIZE       (50 * 1024)
#define CRTL_ICMP_MAGIC_ID           0xf1f2
#define CRTL_ICMP_PROTO_NAME         "icmp"
#define CRTL_ICMP_DATA               "CRTL's ICMP Ping"
    

/* type of icmp ping */
typedef struct __crtl_icmp_ping_s {
    int sockfd;
    struct sockaddr_in dst_addr, from_addr;
    int ntx, nrx, ntry;   //number of send pkg and recv pkg
    int tx, rx;
    int timeout_s;  //time out second
    void *data;
    int data_len;
    void *display_handle;
    int (*display)(void *handle, const char *fmt, ...);
    int rslt; //__CRTL_ICMP_PING_XXXX
}crtl_icmp_ping_t;


/* functions */


/* create a socke to icmp */
static _unused int crtl_icmp_socket()
{
    int sockfd;
    int size = CRTL_ICMP_RECVBUF_SIZE;
    
    struct protoent * protocol  = NULL;
    
    if ((protocol = getprotobyname(CRTL_ICMP_PROTO_NAME)) == NULL)
    {
        crtl_print_err("getprotobyname error.\n\r");
        return -1;
    }
    if ((sockfd = socket(AF_INET, SOCK_RAW, protocol->p_proto)) < 0)
    {
        crtl_print_err("socket error.\n\r");
        return -1;
    }
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size))!=0)
    {
        crtl_print_err("setsockopt SO_RCVBUF error.\n\r");
        close(sockfd);
        return -1;
    }
#if 1    
    struct timeval timeout = {1,0}; //sec
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval))!=0)
    {
        crtl_print_err("setsockopt SO_RCVTIMEO error.\n\r");
        close(sockfd);
        return -1;
    }
#endif    
    return sockfd;
}

/* get dst address */
static _unused int crtl_icmp_dst_addr(const char *addrHost, struct sockaddr_in * dst_addr)
{
    struct hostent  * host      = NULL;
    unsigned long   inaddr      = 0;

    bzero(dst_addr, sizeof(struct sockaddr_in));
    dst_addr->sin_family = AF_INET;

    if ((inaddr = inet_addr(addrHost)) == INADDR_NONE)
    {
        if ((host = gethostbyname(addrHost)) == NULL)
        {
            crtl_print_err("gethostbyname error.\n\r");
            return (-1);
        }
        memcpy((char *) &dst_addr->sin_addr, host->h_addr, host->h_length);
    }
    else 
    {
        memcpy((char *) &dst_addr->sin_addr, (char *) &inaddr, sizeof(dst_addr->sin_addr));
    }
    
    return 0;
}


/* set icmp hdr */
static _unused int crtl_icmp_pkg_pack(void *buffer, int pack_no, const void *data, int data_size)
{
    int packsize = 0;

    struct icmp * icmp  = malloc(sizeof(struct icmp));
    icmp->icmp_type     = ICMP_ECHO;
    icmp->icmp_code     = 0;
    icmp->icmp_cksum    = 0;
    icmp->icmp_seq      = htons(pack_no);
    icmp->icmp_id       = htons(CRTL_ICMP_MAGIC_ID);

    gettimeofday((struct timeval *) &icmp->icmp_data, NULL);

    memcpy(buffer, icmp, sizeof(struct icmp));
    packsize += sizeof(struct icmp);

    if(data && data_size)
    {
        memcpy(buffer+packsize, data, data_size);
        packsize += data_size;
        
    }

    return packsize;
}



/* send icmp package */
static _unused int crtl_icmp_send_pkg(crtl_icmp_ping_t *ping, const void *data, int size)
{
    int             packetsize;
    unsigned short  checksum = 0;
    int             n = 0;
    
    char pkg_buffer[CRTL_ICMP_BUF_SIZE];
    
    packetsize  = crtl_icmp_pkg_pack(pkg_buffer, ping->ntx, data, size);
    checksum    = crtl_tcp_ip_chksum1((unsigned short *)pkg_buffer, packetsize);
    
#define CRTL_ICMP_PKG_CHKSUM_OFFSET  offsetof(struct icmp, icmp_cksum)
#define CRTL_ICMP_PKG_CHKSUM_SIZE    2

    memcpy(pkg_buffer + CRTL_ICMP_PKG_CHKSUM_OFFSET, &checksum, CRTL_ICMP_PKG_CHKSUM_SIZE);

    if ((n = sendto(ping->sockfd, pkg_buffer, packetsize, 0, 
                     (struct sockaddr *) &ping->dst_addr, sizeof(struct sockaddr_in)))< 0)
    {
        crtl_print_err("sendto error. n = %d\n\r", n);
        return 0;
    }
    ping->ntx++;
                     
    return n;
}


/* send icmp package */
static _unused int crtl_icmp_recv_pkg(crtl_icmp_ping_t *ping, void *recvbuf, int size)
{
    int n;
    socklen_t fromlen;
    
    fromlen = sizeof(struct sockaddr_in);
    
    if ((n = recvfrom(ping->sockfd, recvbuf, size, 0, 
                      (struct sockaddr *) &ping->from_addr, &fromlen)) < 0)
    {
        crtl_print_err("recvfrom error.n = %d\n\r", n);
        return 0;
    }

    ping->nrx++;
    
    return n;
}


/* unpack icmp pkg */
static _unused int crtl_icmp_pkg_unpack(crtl_icmp_ping_t *ping, char * buf, int len)
{
    int     iphdrlen;

    struct  ip * ip = NULL;
    struct  icmp * icmp = NULL;
    struct  timeval * tvsend = NULL;
    double  rtt;

    ip          = (struct ip *)buf;
    iphdrlen    = ip->ip_hl << 2;
    icmp        = (struct icmp *) (buf + iphdrlen);
    len        -= iphdrlen;

    if (len < 8)
    {
        crtl_print_err("ICMP packet\'s length is less than 8\n\r");
        return - 1;
    }

    /* ensure icmp reply is mine response msg */
    if ((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id = ntohs(CRTL_ICMP_MAGIC_ID)))
    {
        tvsend  = (struct timeval *)icmp->icmp_data;

        struct timeval tvrecv;
        gettimeofday(&tvrecv, NULL);
        crtl_timeval_subabs(&tvrecv, tvsend, &tvrecv);
        rtt = tvrecv.tv_sec * 1000 + tvrecv.tv_usec / 1000;
        if(ping->display)
            ping->display(ping->display_handle, "%d byte from %s: crtl_icmp_req=%u ttl=%d rtt=%.3f ms\n", 
                        len, inet_ntoa(ping->from_addr.sin_addr), ntohs(icmp->icmp_seq), ip->ip_ttl, rtt);
        else
            printf("%d byte from %s: crtl_icmp_req=%u ttl=%d rtt=%.3f ms\n", 
                         len, inet_ntoa(ping->from_addr.sin_addr), ntohs(icmp->icmp_seq), ip->ip_ttl, rtt);
    }
    else 
    {
        return - 1;
    }
    return 0;
}



/**********************************************************************************************************************/
/* icmp ping with timeout */
static _unused void crtl_icmp_ping_finish (crtl_icmp_ping_t *ping)
{
    if(ping->display)
    {
        ping->display(ping->display_handle, "\n--- ping statistics ---\n");
        ping->display(ping->display_handle, "%d packets transmitted, %d received, %d%% lost\n", 
                        ping->ntx, ping->nrx, 
                        (ping->ntx - ping->nrx) / ping->ntx * 100);
    }
    else
    {
        printf("\n--- ping statistics ---\n");
        printf("%d packets transmitted, %d received, %d%% lost\n", 
                    ping->ntx, ping->nrx, 
                    (ping->ntx - ping->nrx) / ping->ntx * 100);
    }
    
    if(ping->ntx == ping->ntry)
        ping->rslt = CRTL_SUCCESS;
    
    return;
}

static _unused crtl_icmp_ping_t* crtl_icmp_ping_init(const char *ipv4, int timeout_sec, int ntry, void *data, int data_len, int (*display)(void *handle, const char *fmt, ...), void *display_handle)
{
    crtl_icmp_ping_t *ping = (crtl_icmp_ping_t *)malloc(sizeof(crtl_icmp_ping_t));
    if(!ping) return NULL;

    memset(ping, 0x00, sizeof(crtl_icmp_ping_t));
    
    ping->sockfd    = crtl_icmp_socket();
    ping->timeout_s = timeout_sec;
    ping->ntry      = ntry;
    ping->display       = display;
    ping->display_handle = display_handle;
    ping->data = data;
    ping->data_len = data_len;
    
    crtl_icmp_dst_addr(ipv4, &ping->dst_addr);
    
    if(ping->display)
        ping->display(ping->display_handle, "PING %s(%s): %ld bytes data in ICMP packets.\n", 
                       ipv4, inet_ntoa(ping->dst_addr.sin_addr), sizeof(CRTL_ICMP_DATA));
    else
        printf("PING %s(%s): %ld bytes data in ICMP packets.\n", 
                       ipv4, inet_ntoa(ping->dst_addr.sin_addr), sizeof(CRTL_ICMP_DATA));
    return ping;
}

static _unused int crtl_icmp_ping_destroy(crtl_icmp_ping_t* ping)
{
    if(ping)
    {
        close(ping->sockfd);
        free(ping);
    }
//    free(ping);
    ping = NULL;
    return 0;
}

/* PING */
_api int crtl_icmp_ping(const char *ipv4, unsigned int timeout, int ntry, void *data, int data_len,
                            int (*display)(void *handle, const char *fmt, ...), void *display_handle)
{
    crtl_icmp_ping_t *ping = crtl_icmp_ping_init(ipv4, timeout, ntry, data, data_len, display, display_handle);

    char pkg_buffer[CRTL_ICMP_BUF_SIZE];

    ping->rslt = CRTL_ERROR;

    while(ping->ntx < ping->ntry)
    {
        ping->tx = crtl_icmp_send_pkg(ping, ping->data?ping->data:CRTL_ICMP_DATA, ping->data?ping->data_len:sizeof(CRTL_ICMP_DATA));
        if(ping->tx < 8) 
        {
            ping->rslt = CRTL_ERROR;
            break;
        }
        
        ping->rx = crtl_icmp_recv_pkg(ping, pkg_buffer, ping->tx);
        if(ping->rx<8) 
        {
            ping->rslt = CRTL_ERROR;
            break;
        }
        if (crtl_icmp_pkg_unpack(ping, pkg_buffer, ping->rx) == -1)
            continue;
        //sleep(1);
    }
    
    crtl_icmp_ping_finish(ping);
    crtl_icmp_ping_destroy(ping);
    
    return ping->rslt;
}

