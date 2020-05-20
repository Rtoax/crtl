#include <errno.h>
#include <unistd.h> //close
#include <fcntl.h>
#include <sys/types.h>          /* See NOTES */
#include <unistd.h>

#include <signal.h>
#include <string.h>
#include <stdint.h>
#include <netdb.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <arpa/inet.h> //inet_xxxx

#include "crtl/time.h"
#include "crtl/log.h"
#include "crtl/assert.h"
#include "crtl/bits/types_basic.h"
#include "crtl/network/socket.h"
#include "crtl/network/inet.h"
#include "crtl/file.h"

#include "crtl/easy/macro.h"
#include "crtl/easy/attribute.h"

#include "crypto/expect.h"


int crtl_socket_server_tcp(struct sockaddr_in *srvaddr, int port, int listen_backlog)
{
    if(unlikely(!srvaddr) || unlikely(!port) || !listen_backlog)
    {
        crtl_print_err("Invalid value.\n");
        crtl_assert(0);
        return CRTL_ERROR;
    }
    
    int listenfd = -1;
    
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        crtl_print_err("socket: %s\n", strerror(errno));
		return CRTL_ERROR;
    } 
    
    /**
	 *  Set send/receive message timeout
	 */
	crtl_socket_keepalive(listenfd, 2, 0);
	crtl_socket_reuseaddr(listenfd, 2, 0);

    
    bzero(srvaddr, sizeof(struct sockaddr_in));
    
    srvaddr->sin_family = AF_INET;
    srvaddr->sin_addr.s_addr = crtl_hton32(INADDR_ANY);
    srvaddr->sin_port = crtl_hton16(port);
    
    if(bind(listenfd, (struct sockaddr*)srvaddr, sizeof(struct sockaddr)) < 0)
    {
        crtl_print_err("bind: %s\n", strerror(errno));
        close(listenfd);
		return CRTL_ERROR;
    }
    
    if(listen(listenfd, listen_backlog) < 0)
    {
        crtl_print_err("listen: %s\n", strerror(errno));
        close(listenfd);
		return CRTL_ERROR;
    }  
    
    return listenfd;
}



int crtl_socket_keepalive(int sockfd, int sec, int usec)
{
    struct  timeval timeout={sec,usec};//timeout
    return setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &timeout, sizeof(timeout));
}
int crtl_socket_reuseaddr(int sockfd, int sec, int usec)
{
    struct  timeval timeout={sec,usec};//timeout
    return setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &timeout, sizeof(timeout));
}
int crtl_socket_recvtimeout(int sockfd, int sec, int usec)
{
    struct  timeval timeout={sec,usec};//timeout
    return setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
}
int crtl_socket_sendtimeout(int sockfd, int sec, int usec)
{
    struct  timeval timeout={sec,usec};//timeout
    return setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
}


/* non block connect */
int crtl_socket_connect_nonblk(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
{
	int				flags, n, error;
	socklen_t		len;
	fd_set			rset, wset;
	struct timeval	tval;

	flags = fcntl(sockfd, F_GETFL, 0);
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

	error = 0;
	if ( (n = connect(sockfd, saptr, salen)) < 0)
		if (errno != EINPROGRESS)
			return(CRTL_ERROR);

	/* Do whatever we want while the connect is taking place. */

	if (n == 0)
		goto done;	/* connect completed immediately */

	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	wset = rset;
	tval.tv_sec = nsec;
	tval.tv_usec = 0;

	if ( (n = select(sockfd+1, &rset, &wset, NULL,
					 nsec ? &tval : NULL)) == 0) {
		close(sockfd);		/* timeout */
		errno = ETIMEDOUT;
		return(CRTL_ERROR);
	}

	if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
		len = sizeof(error);
		if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
			return(CRTL_ERROR);			/* Solaris pending error */
	} else {
		crtl_print_err("select error: sockfd not set\n");
        return CRTL_ERROR;
    }

done:
	fcntl(sockfd, F_SETFL, flags);	/* restore file status flags */

	if (error) {
		close(sockfd);		/* just in case */
		errno = error;
		return(CRTL_ERROR);
	}
	return(CRTL_SUCCESS);
}


/* timeout connect */
static void _unused __connect_timeout_alarm(int signo);
int crtl_socket_connect_timeout(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
{
    typedef void    Sigfunc(int);   /* for signal handlers */

    Sigfunc	*sigfunc;
	int		n;
    
	sigfunc = signal(SIGALRM, __connect_timeout_alarm);
	if (alarm(nsec) != 0)
    {
		crtl_print_err("connect_timeo: alarm was already set\n");
    }

	if ( (n = connect(sockfd, saptr, salen)) < 0) {
		close(sockfd);
		if (errno == EINTR)
			errno = ETIMEDOUT;
	}
	alarm(0);					/* turn off the alarm */
	signal(SIGALRM, sigfunc);	/* restore previous signal handler */

	return(n);
}
static void _unused __connect_timeout_alarm(int signo)
{
	return;		/* just interrupt the connect() */
}





/* create socket */
int crtl_socket_client_tcp(struct sockaddr_in *srvaddr, const char *srvIPv4, int port)
{
    if(unlikely(!srvaddr) || unlikely(!srvIPv4) || unlikely(!port))
    {
        crtl_print_err("Invalid value.\n");
        return CRTL_ERROR;
    }
    int sockfd = -1;
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
        crtl_print_err("socket: %s\n", strerror(errno));
		return CRTL_ERROR;
	}
    
    bzero(srvaddr, sizeof(struct sockaddr_in));
	srvaddr->sin_family = AF_INET;
	srvaddr->sin_port = crtl_hton16(port);
	
	if(crtl_inet_pton(AF_INET, srvIPv4, &srvaddr->sin_addr) < 0)
	{
        crtl_print_err("inet_pton: %s\n", strerror(errno));
        close(sockfd);
		return CRTL_ERROR;
	}
	
	if(connect(sockfd, (struct sockaddr*)srvaddr, sizeof(struct sockaddr)) < 0)
	{
        crtl_print_err("connect: %s\n", strerror(errno));
        close(sockfd);
		return CRTL_ERROR;
	}	
    
    return sockfd;
}



int crtl_socket_accept(int listenfd, struct sockaddr *src_addr, socklen_t *paddrlen)
{
    if(unlikely(!listenfd) || unlikely(!src_addr))
    {
        crtl_print_err("Invalid value.\n");
        return CRTL_ERROR;
    }
    *paddrlen = sizeof(struct sockaddr);

    return accept(listenfd, (struct sockaddr*)src_addr, paddrlen);
}


/* accept */
int crtl_socket_accept_tcp(int listenfd, struct sockaddr *src_addr, socklen_t *paddrlen)
{
    return crtl_socket_accept(listenfd, (struct sockaddr*)src_addr, paddrlen);
}




int crtl_socket_read(int listenfd, void *msg, int msg_len)
{
    if(!listenfd || !msg || !msg_len)
    {
        crtl_print_err("Invalid value.\n");
        return CRTL_ERROR;
    }
    return read(listenfd, msg, msg_len);
}


/* read */
int crtl_socket_read_tcp(int listenfd, void *msg, int msg_len)
{
    return crtl_socket_read(listenfd, msg, msg_len);
}


int crtl_socket_write(int listenfd, const void *msg, int msg_len)
{
    if(!listenfd || !msg || !msg_len)
    {
        crtl_print_err("Invalid value.\n");
        return -1;
    }
    return write(listenfd, msg, msg_len);
}


/* write */
int crtl_socket_write_tcp(int listenfd, const void *msg, int msg_len)
{
    return crtl_socket_write(listenfd, msg, msg_len);
}




int crtl_socket_server_udp(struct sockaddr_in *srvaddr, int port)
{
    if(unlikely(!srvaddr) || unlikely(!port))
    {
        crtl_print_err("Invalid value.\n");
        return CRTL_ERROR;
    }

    int sockfd = -1;

    bzero(srvaddr, sizeof(struct sockaddr_in));
    srvaddr->sin_family = AF_INET;    
    srvaddr->sin_addr.s_addr = crtl_hton32(INADDR_ANY);
    srvaddr->sin_port = crtl_hton16(port);
    
    if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        crtl_print_err("socket: %s\n", strerror(errno));
        return CRTL_ERROR;
    }
    
    /**
	 *  Set send/receive message timeout
	 */
	crtl_socket_keepalive(sockfd, 2, 0);
	crtl_socket_reuseaddr(sockfd, 2, 0);
    
    if(bind(sockfd, (struct sockaddr *)srvaddr, sizeof(struct sockaddr)))
    {
        crtl_print_err("bind: %s\n", strerror(errno));
        close(sockfd);
        return CRTL_ERROR;
    }
    
    return sockfd;
}


int crtl_socket_client_udp(struct sockaddr_in *srvaddr, const char *srvIPv4, int port)
{
    if(unlikely(!srvaddr) || unlikely(!srvIPv4) || unlikely(!port))
    {
        crtl_print_err("Invalid value.\n");
        return CRTL_ERROR;
    }
    
    int sockfd = -1, t;
    
    bzero(srvaddr, sizeof(struct sockaddr_in));
    srvaddr->sin_family = AF_INET;
    srvaddr->sin_port = crtl_hton16(port);
    
    if((t = crtl_inet_pton(AF_INET, srvIPv4, &srvaddr->sin_addr)) <= 0)
    {
        crtl_print_err("inet_pton: %s\n", strerror(errno));
        return CRTL_ERROR;
    }
    if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        crtl_print_err("socket: %s\n", strerror(errno));
        return CRTL_ERROR;
    }
    
    return sockfd;
}


/* recvfrom */
int crtl_socket_udp_recvfrom(int sockfd, void *msg, int msg_len, int flags, struct sockaddr *src_addr)
{
    if(unlikely(!msg) || unlikely(!msg_len) || unlikely(!sockfd))
    {
        crtl_print_err("Invalid value.\n");
        return CRTL_ERROR;
    }

    socklen_t addrlen = sizeof(struct sockaddr_in);
    
    return recvfrom(sockfd, msg, msg_len, flags, (struct sockaddr *)src_addr, &addrlen);
}

/* sendto */
int crtl_socket_udp_sendto(int sockfd, const void *msg, int msg_len, int flags, struct sockaddr *src_addr)
{
    if(unlikely(!msg) || unlikely(!msg_len) || unlikely(!sockfd))
    {
        crtl_print_err("Invalid value.\n");
        return CRTL_ERROR;
    }
    socklen_t addrlen = sizeof(struct sockaddr_in);

    __crtl_dbg("msg_len = %d\n", msg_len);

    return sendto(sockfd, msg, msg_len, flags, (struct sockaddr *)src_addr, addrlen);
}



/* Unix Socket API */
int crtl_socket_server_unix(struct sockaddr_un *srvaddr, const char* path, int backlog)
{
    if(!path)
    {
        crtl_print_err("Invalid value.\n");
        return CRTL_ERROR;
    }
    int _unused null_addr = 0;
    if(!srvaddr)
    {
        null_addr = 1;
        srvaddr = (struct sockaddr_un*)malloc(sizeof(struct sockaddr_un));
    }
    
    if(0 == crtl_is_exist(path))
	{
		crtl_print_warning("%s is exist\n", path);
	}
    
    int listen_fd, ret;
    
    listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(listen_fd < 0)
    {
        crtl_print_err("socket: %s\n", strerror(errno));
        return CRTL_ERROR;

    } else {
        
        srvaddr->sun_family = AF_UNIX;
        strncpy(srvaddr->sun_path, path, sizeof(srvaddr->sun_path)-1);
        
        unlink(path);
        
        ret = bind(listen_fd, (struct sockaddr *)srvaddr, sizeof(struct sockaddr));
        if(ret == -1)
        {
            crtl_print_err("bind: %s\n", strerror(errno));
            close(listen_fd);
            unlink(path);
            return CRTL_ERROR;
        }
        
        ret = listen(listen_fd, backlog);
        if(ret == -1)
        {
            crtl_print_err("listen: %s\n", strerror(errno));
            close(listen_fd);
            unlink(path);
            return CRTL_ERROR;
        }
        
        crtl_add_all_permission(path);
    }

    if(null_addr)
        free(srvaddr);
    
    return listen_fd;
    
}


/* client */
int crtl_socket_client_unix(struct sockaddr_un *srvaddr, const char* path)
{
    if(!srvaddr || !path)
    {
        crtl_print_err("Invalid value.\n");
        return CRTL_ERROR;
    }
    if(0 != crtl_is_exist(path))
	{
		crtl_print_err("%s is not exist\n", path);
        return CRTL_ERROR;
	}
    int connect_fd;
    int ret = 0;
    
    connect_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(connect_fd < 0)
    {
        crtl_print_err("socket: %s\n", strerror(errno));
        return CRTL_ERROR;
    }
    else
    {
        srvaddr->sun_family = AF_UNIX;
        strcpy(srvaddr->sun_path, path);
        
        ret = connect(connect_fd, (struct sockaddr *)srvaddr, sizeof(struct sockaddr));
        if(ret == -1)
        {
            crtl_print_err("connect: %s\n", strerror(errno));
            close(connect_fd);
            return CRTL_ERROR;
        }
    }
    
    return connect_fd;
}


int crtl_socket_accept_unix(int listenfd, struct sockaddr *src_addr, socklen_t *paddrlen)
{
    return crtl_socket_accept(listenfd, src_addr, paddrlen);
}
int crtl_socket_read_unix(int listenfd, void *msg, int msg_len)
{
    return crtl_socket_read(listenfd, msg, msg_len);
}
int crtl_socket_write_unix(int listenfd, const void *msg, int msg_len)
{
    return crtl_socket_write(listenfd, msg, msg_len);
}


int crtl_socket_close(int sockfd)
{
    return close(sockfd);
}

