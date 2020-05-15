#ifndef _CRTL_TCP_IP_BITS_SOCKET_H
#define _CRTL_TCP_IP_BITS_SOCKET_H 1

#include <sys/socket.h>
#include <sys/types.h>          /* See NOTES */
#include <netdb.h>


extern int accept(int socket, struct sockaddr * address, socklen_t * address_len);
extern int listen(int s, int backlog);
extern int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
extern int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
extern int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);





#endif /*<_CRTL_TCP_IP_BITS_SOCKET_H>*/
