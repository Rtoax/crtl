#ifndef __CRTL_TCP_IP_SOCKET_H
#define __CRTL_TCP_IP_SOCKET_H 1


#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>



int crtl_socket_server_tcp(struct sockaddr_in *srvaddr, int port, int listen_backlog);
int crtl_socket_client_tcp(struct sockaddr_in *srvaddr, const char *srvIPv4, int port);

int crtl_socket_accept_tcp(int listenfd, struct sockaddr *src_addr, socklen_t *paddrlen);

int crtl_socket_read_tcp(int listenfd, void *msg, int msg_len);
int crtl_socket_write_tcp(int listenfd, const void *msg, int msg_len);

int crtl_socket_connect_nonblk(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec);
int crtl_socket_connect_timeout(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec);

int crtl_socket_server_udp(struct sockaddr_in *srvaddr, int port);
int crtl_socket_client_udp(struct sockaddr_in *srvaddr, const char *srvIPv4, int port);

int crtl_socket_udp_recvfrom(int sockfd, void *msg, int msg_len, int flags, struct sockaddr *src_addr);
int crtl_socket_udp_sendto(int sockfd, const void *msg, int msg_len, int flags, struct sockaddr *src_addr);


int crtl_socket_server_unix(struct sockaddr_un *srvaddr, const char* path, int backlog);
int crtl_socket_client_unix(struct sockaddr_un *srvaddr, const char* path);
int crtl_socket_accept_unix(int listenfd, struct sockaddr *src_addr, socklen_t *paddrlen);
int crtl_socket_read_unix(int listenfd, void *msg, int msg_len);
int crtl_socket_write_unix(int listenfd, const void *msg, int msg_len);


int crtl_socket_accept(int listenfd, struct sockaddr *src_addr, socklen_t *paddrlen);
int crtl_socket_read(int listenfd, void *msg, int msg_len);
int crtl_socket_write(int listenfd, const void *msg, int msg_len);

int crtl_socket_keepalive(int sockfd, int sec, int usec);
int crtl_socket_reuseaddr(int sockfd, int sec, int usec);
int crtl_socket_recvtimeout(int sockfd, int sec, int usec);
int crtl_socket_sendtimeout(int sockfd, int sec, int usec);



int crtl_socket_close(int sockfd);


#endif /*<__CRTL_TCP_IP_SOCKET_H>*/
