#ifndef __CRTL_TCP_IP_SOCKET_H
#define __CRTL_TCP_IP_SOCKET_H 1

#include <signal.h>
#include <errno.h>
#include <string.h>

#include "crtl/tcp-ip/bits/crtl_socket.h"
#include "crtl/tcp-ip/bits/crtl_sockaddr.h"
#include "crtl/tcp-ip/bits/crtl_byteorder.h"


int crtl_socket_server_tcp(sockaddr_in_t *srvaddr, int port, int listen_backlog);
int crtl_socket_client_tcp(sockaddr_in_t *srvaddr, const char *srvIPv4, int port);

int crtl_socket_accept_tcp(int listenfd, sockaddr_t *src_addr, socklen_t *paddrlen);

int crtl_socket_read_tcp(int listenfd, void *msg, int msg_len);
int crtl_socket_write_tcp(int listenfd, const void *msg, int msg_len);

int crtl_socket_connect_nonblk(int sockfd, const sockaddr_t *saptr, socklen_t salen, int nsec);
int crtl_socket_connect_timeout(int sockfd, const sockaddr_t *saptr, socklen_t salen, int nsec);

int crtl_socket_server_udp(sockaddr_in_t *srvaddr, int port);
int crtl_socket_client_udp(sockaddr_in_t *srvaddr, const char *srvIPv4, int port);

int crtl_socket_udp_recvfrom(int sockfd, void *msg, int msg_len, int flags, sockaddr_t *src_addr);
int crtl_socket_udp_sendto(int sockfd, const void *msg, int msg_len, int flags, sockaddr_t *src_addr);



int crtl_socket_accept(int listenfd, sockaddr_t *src_addr, socklen_t *paddrlen);
int crtl_socket_read(int listenfd, void *msg, int msg_len);
int crtl_socket_write(int listenfd, const void *msg, int msg_len);

int crtl_socket_keepalive(int sockfd, int sec, int usec);
int crtl_socket_reuseaddr(int sockfd, int sec, int usec);
int crtl_socket_recvtimeout(int sockfd, int sec, int usec);
int crtl_socket_sendtimeout(int sockfd, int sec, int usec);



int crtl_socket_close(int sockfd);


#endif /*<__CRTL_TCP_IP_SOCKET_H>*/