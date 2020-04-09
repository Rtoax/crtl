#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>

#include "crtl/network/crtl_network_socket.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_types.h"
#include "crtl/easy/attribute.h"

extern int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);


/* non block connect */
int crtl_connect_nonblk(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
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
int crtl_connect_timeout(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
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




