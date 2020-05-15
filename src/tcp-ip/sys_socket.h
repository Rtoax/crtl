/* Declarations of socket constants, types, and functions.
   Copyright (C) 1991,92,1994-2001,2003,2005,2007,2008,2012
   Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
#ifndef	_SYS_SOCKET_H


/* This operating system-specific header file defines the SOCK_*, PF_*,
   AF_*, MSG_*, SOL_*, and SO_* constants, and the `struct sockaddr',
   `struct msghdr', and `struct linger' types.  */
#include <bits/socket.h>

#ifdef __USE_BSD
/* This is the 4.3 BSD `struct sockaddr' format, which is used as wire
   format in the grotty old 4.3 `talk' protocol.  */
struct osockaddr
  {
    unsigned short int sa_family;
    unsigned char sa_data[14];
  };

/* The following constants should be used for the second parameter of
   `shutdown'.  */
enum
{
  SHUT_RD = 0,		/* No more receptions.  */
#define SHUT_RD		SHUT_RD
  SHUT_WR,		/* No more transmissions.  */
#define SHUT_WR		SHUT_WR
  SHUT_RDWR		/* No more receptions or transmissions.  */
#define SHUT_RDWR	SHUT_RDWR
};

/* For `recvmmsg' and `sendmmsg'.  */
struct mmsghdr
  {
    struct msghdr msg_hdr;	/* Actual message header.  */
    unsigned int msg_len;	/* Number of received or sent bytes for the
				   entry.  */
  };
#endif


/* Create a new socket of type TYPE in domain DOMAIN, using
   protocol PROTOCOL.  If PROTOCOL is zero, one is chosen automatically.
   Returns a file descriptor for the new socket, or -1 for errors.  */
extern int socket (int __domain, int __type, int __protocol);
//__domain
//Name                Purpose                          Man page
// AF_UNIX, AF_LOCAL   Local communication              unix(7)
// AF_INET             IPv4 Internet protocols          ip(7)
// AF_INET6            IPv6 Internet protocols          ipv6(7)
// AF_IPX              IPX - Novell protocols
// AF_NETLINK          Kernel user interface device     netlink(7)
// AF_X25              ITU-T X.25 / ISO-8208 protocol   x25(7)
// AF_AX25             Amateur radio AX.25 protocol
// AF_ATMPVC           Access to raw ATM PVCs
// AF_APPLETALK        Appletalk                        ddp(7)
// AF_PACKET           Low level packet interface       packet(7)

//__type
//SOCK_STREAM     Provides sequenced, reliable, two-way, connection-based  byte  streams.   An  out-of-band
//                data transmission mechanism may be supported.
//
//SOCK_DGRAM      Supports datagrams (connectionless, unreliable messages of a fixed maximum length).
//
//SOCK_SEQPACKET  Provides a sequenced, reliable, two-way connection-based data transmission path for data‐
//                grams of fixed maximum length; a consumer is required to read an entire packet with  each
//                input system call.
//
//SOCK_RAW        Provides raw network protocol access.
//
//SOCK_RDM        Provides a reliable datagram layer that does not guarantee ordering.
//
//SOCK_PACKET     Obsolete and should not be used in new programs; see packet(7).
//Since Linux 2.6.27, the type argument serves a second purpose: in addition to specifying a  socket  type,
//it may include the bitwise OR of any of the following values, to modify the behavior of socket():
//
//SOCK_NONBLOCK   Set  the  O_NONBLOCK  file status flag on the new open file description.  Using this flag
//                saves extra calls to fcntl(2) to achieve the same result.
//
//SOCK_CLOEXEC    Set the close-on-exec (FD_CLOEXEC) flag on the new file descriptor.  See the  description
//                of the O_CLOEXEC flag in open(2) for reasons why this may be useful.


/* Create two new sockets, of type TYPE in domain DOMAIN and using
   protocol PROTOCOL, which are connected to each other, and put file
   descriptors for them in FDS[0] and FDS[1].  If PROTOCOL is zero,
   one will be chosen automatically.  Returns 0 on success, -1 for errors.  */
extern int socketpair (int __domain, int __type, int __protocol, int __fds[2]);

/* Give the socket FD the local address ADDR (which is LEN bytes long).  */
extern int bind (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);

/* Put the local address of FD into *ADDR and its length in *LEN.  */
extern int getsockname (int __fd, __SOCKADDR_ARG __addr, socklen_t *__restrict __len);

/* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
   For connectionless socket types, just set the default address to send to
   and the only address from which to accept transmissions.
   Return 0 on success, -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int connect (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);

/* Put the address of the peer connected to socket FD into *ADDR
   (which is *LEN bytes long), and its actual length into *LEN.  */
extern int getpeername (int __fd, __SOCKADDR_ARG __addr, socklen_t *__restrict __len);


/* Send N bytes of BUF to socket FD.  Returns the number sent or -1.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern ssize_t send (int __fd, const void *__buf, size_t __n, int __flags);

/* Read N bytes into BUF from socket FD.
   Returns the number read or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern ssize_t recv (int __fd, void *__buf, size_t __n, int __flags);

/* Send N bytes of BUF on socket FD to peer at address ADDR (which is
   ADDR_LEN bytes long).  Returns the number sent, or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern ssize_t sendto (int __fd, const void *__buf, size_t __n, int __flags, __CONST_SOCKADDR_ARG __addr, socklen_t __addr_len);

/* Read N bytes into BUF through socket FD.
   If ADDR is not NULL, fill in *ADDR_LEN bytes of it with tha address of
   the sender, and store the actual size of the address in *ADDR_LEN.
   Returns the number of bytes read or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern ssize_t recvfrom (int __fd, void *__restrict __buf, size_t __n, int __flags, __SOCKADDR_ARG __addr, socklen_t *__restrict __addr_len);


/* Send a message described MESSAGE on socket FD.
   Returns the number of bytes sent, or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern ssize_t sendmsg (int __fd, const struct msghdr *__message, int __flags);

#ifdef __USE_GNU
/* Send a VLEN messages as described by VMESSAGES to socket FD.
   Returns the number of datagrams successfully written or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int sendmmsg (int __fd, struct mmsghdr *__vmessages, unsigned int __vlen, int __flags);
#endif

/* Receive a message as described by MESSAGE from socket FD.
   Returns the number of bytes read or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern ssize_t recvmsg (int __fd, struct msghdr *__message, int __flags);

#ifdef __USE_GNU
/* Receive up to VLEN messages as described by VMESSAGES from socket FD.
   Returns the number of bytes read or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int recvmmsg (int __fd, struct mmsghdr *__vmessages,unsigned int __vlen, int __flags,const struct timespec *__tmo);
#endif


/* Put the current value for socket FD's option OPTNAME at protocol level LEVEL
   into OPTVAL (which is *OPTLEN bytes long), and set *OPTLEN to the value's
   actual length.  Returns 0 on success, -1 for errors.  */
extern int getsockopt (int __fd, int __level, int __optname, void *__restrict __optval, socklen_t *__restrict __optlen);

/* Set socket FD's option OPTNAME at protocol level LEVEL
   to *OPTVAL (which is OPTLEN bytes long).
   Returns 0 on success, -1 for errors.  */
extern int setsockopt (int __fd, int __level, int __optname, const void *__optval, socklen_t __optlen);


/* Prepare to accept connections on socket FD.
   N connection requests will be queued before further requests are refused.
   Returns 0 on success, -1 for errors.  */
extern int listen (int __fd, int __n);

/* Await a connection on socket FD.
   When a connection arrives, open a new socket to communicate with it,
   set *ADDR (which is *ADDR_LEN bytes long) to the address of the connecting
   peer and *ADDR_LEN to the address's actual length, and return the
   new socket's descriptor, or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int accept (int __fd, __SOCKADDR_ARG __addr, socklen_t *__restrict __addr_len);

#ifdef __USE_GNU
/* Similar to 'accept' but takes an additional parameter to specify flags.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int accept4 (int __fd, __SOCKADDR_ARG __addr, socklen_t *__restrict __addr_len, int __flags);
#endif

/* Shut down all or part of the connection open on socket FD.
   HOW determines what to shut down:
     SHUT_RD   = No more receptions;
     SHUT_WR   = No more transmissions;
     SHUT_RDWR = No more receptions or transmissions.
   Returns 0 on success, -1 for errors.  */
extern int shutdown (int __fd, int __how);


#ifdef __USE_XOPEN2K
/* Determine wheter socket is at a out-of-band mark.  */
extern int sockatmark (int __fd);
#endif


#ifdef __USE_MISC
/* FDTYPE is S_IFSOCK or another S_IF* macro defined in <sys/stat.h>;
   returns 1 if FD is open on an object of the indicated type, 0 if not,
   or -1 for errors (setting errno).  */
extern int isfdtype (int __fd, int __fdtype);
#endif


/* Supported address families. */
#define AF_UNSPEC	0
#define AF_UNIX		1	/* Unix domain sockets 		*/
#define AF_LOCAL	1	/* POSIX name for AF_UNIX	*/
#define AF_INET		2	/* Internet IP Protocol 	*/
#define AF_AX25		3	/* Amateur Radio AX.25 		*/
#define AF_IPX		4	/* Novell IPX 			*/
#define AF_APPLETALK	5	/* AppleTalk DDP 		*/
#define AF_NETROM	6	/* Amateur Radio NET/ROM 	*/
#define AF_BRIDGE	7	/* Multiprotocol bridge 	*/
#define AF_ATMPVC	8	/* ATM PVCs			*/
#define AF_X25		9	/* Reserved for X.25 project 	*/
#define AF_INET6	10	/* IP version 6			*/
#define AF_ROSE		11	/* Amateur Radio X.25 PLP	*/
#define AF_DECnet	12	/* Reserved for DECnet project	*/
#define AF_NETBEUI	13	/* Reserved for 802.2LLC project*/
#define AF_SECURITY	14	/* Security callback pseudo AF */
#define AF_KEY		15      /* PF_KEY key management API */
#define AF_NETLINK	16
#define AF_ROUTE	AF_NETLINK /* Alias to emulate 4.4BSD */
#define AF_PACKET	17	/* Packet family		*/
#define AF_ASH		18	/* Ash				*/
#define AF_ECONET	19	/* Acorn Econet			*/
#define AF_ATMSVC	20	/* ATM SVCs			*/
#define AF_RDS		21	/* RDS sockets 			*/
#define AF_SNA		22	/* Linux SNA Project (nutters!) */
#define AF_IRDA		23	/* IRDA sockets			*/
#define AF_PPPOX	24	/* PPPoX sockets		*/
#define AF_WANPIPE	25	/* Wanpipe API Sockets */
#define AF_LLC		26	/* Linux LLC			*/
#define AF_IB		27	/* Native InfiniBand address	*/
#define AF_MPLS		28	/* MPLS */
#define AF_CAN		29	/* Controller Area Network      */
#define AF_TIPC		30	/* TIPC sockets			*/
#define AF_BLUETOOTH	31	/* Bluetooth sockets 		*/
#define AF_IUCV		32	/* IUCV sockets			*/
#define AF_RXRPC	33	/* RxRPC sockets 		*/
#define AF_ISDN		34	/* mISDN sockets 		*/
#define AF_PHONET	35	/* Phonet sockets		*/
#define AF_IEEE802154	36	/* IEEE802154 sockets		*/
#define AF_CAIF		37	/* CAIF sockets			*/
#define AF_ALG		38	/* Algorithm sockets		*/
#define AF_NFC		39	/* NFC sockets			*/
#define AF_VSOCK	40	/* vSockets			*/
#define AF_KCM		41	/* Kernel Connection Multiplexor*/
#define AF_QIPCRTR	42	/* Qualcomm IPC Router          */
#define AF_SMC		43	/* smc sockets: reserve number for
				 * PF_SMC protocol family that
				 * reuses AF_INET address family
				 */
#define AF_XDP		44	/* XDP sockets			*/

#define AF_MAX		45	/* For now.. */

/* Protocol families, same as address families. */
#define PF_UNSPEC	AF_UNSPEC
#define PF_UNIX		AF_UNIX
#define PF_LOCAL	AF_LOCAL
#define PF_INET		AF_INET
#define PF_AX25		AF_AX25
#define PF_IPX		AF_IPX
#define PF_APPLETALK	AF_APPLETALK
#define	PF_NETROM	AF_NETROM
#define PF_BRIDGE	AF_BRIDGE
#define PF_ATMPVC	AF_ATMPVC
#define PF_X25		AF_X25
#define PF_INET6	AF_INET6
#define PF_ROSE		AF_ROSE
#define PF_DECnet	AF_DECnet
#define PF_NETBEUI	AF_NETBEUI
#define PF_SECURITY	AF_SECURITY
#define PF_KEY		AF_KEY
#define PF_NETLINK	AF_NETLINK
#define PF_ROUTE	AF_ROUTE
#define PF_PACKET	AF_PACKET
#define PF_ASH		AF_ASH
#define PF_ECONET	AF_ECONET
#define PF_ATMSVC	AF_ATMSVC
#define PF_RDS		AF_RDS
#define PF_SNA		AF_SNA
#define PF_IRDA		AF_IRDA
#define PF_PPPOX	AF_PPPOX
#define PF_WANPIPE	AF_WANPIPE
#define PF_LLC		AF_LLC
#define PF_IB		AF_IB
#define PF_MPLS		AF_MPLS
#define PF_CAN		AF_CAN
#define PF_TIPC		AF_TIPC
#define PF_BLUETOOTH	AF_BLUETOOTH
#define PF_IUCV		AF_IUCV
#define PF_RXRPC	AF_RXRPC
#define PF_ISDN		AF_ISDN
#define PF_PHONET	AF_PHONET
#define PF_IEEE802154	AF_IEEE802154
#define PF_CAIF		AF_CAIF
#define PF_ALG		AF_ALG
#define PF_NFC		AF_NFC
#define PF_VSOCK	AF_VSOCK
#define PF_KCM		AF_KCM
#define PF_QIPCRTR	AF_QIPCRTR
#define PF_SMC		AF_SMC
#define PF_XDP		AF_XDP
#define PF_MAX		AF_MAX

/* Maximum queue length specifiable by listen.  */
#define SOMAXCONN	128

/* Flags we can use with send/ and recv.
   Added those for 1003.1g not all are supported yet
 */

#define MSG_OOB		1
#define MSG_PEEK	2
#define MSG_DONTROUTE	4
#define MSG_TRYHARD     4       /* Synonym for MSG_DONTROUTE for DECnet */
#define MSG_CTRUNC	8
#define MSG_PROBE	0x10	/* Do not send. Only probe path f.e. for MTU */
#define MSG_TRUNC	0x20
#define MSG_DONTWAIT	0x40	/* Nonblocking io		 */
#define MSG_EOR         0x80	/* End of record */
#define MSG_WAITALL	0x100	/* Wait for a full request */
#define MSG_FIN         0x200
#define MSG_SYN		0x400
#define MSG_CONFIRM	0x800	/* Confirm path validity */
#define MSG_RST		0x1000
#define MSG_ERRQUEUE	0x2000	/* Fetch message from error queue */
#define MSG_NOSIGNAL	0x4000	/* Do not generate SIGPIPE */
#define MSG_MORE	0x8000	/* Sender will send more */
#define MSG_WAITFORONE	0x10000	/* recvmmsg(): block until 1+ packets avail */
#define MSG_SENDPAGE_NOPOLICY 0x10000 /* sendpage() internal : do no apply policy */
#define MSG_SENDPAGE_NOTLAST 0x20000 /* sendpage() internal : not the last page */
#define MSG_BATCH	0x40000 /* sendmmsg(): more messages coming */
#define MSG_EOF         MSG_FIN
#define MSG_NO_SHARED_FRAGS 0x80000 /* sendpage() internal : page frags are not shared */
#define MSG_SENDPAGE_DECRYPTED	0x100000 /* sendpage() internal : page may carry
					  * plain text and require encryption
					  */

#define MSG_ZEROCOPY	0x4000000	/* Use user data in kernel path */
#define MSG_FASTOPEN	0x20000000	/* Send data in TCP SYN */
#define MSG_CMSG_CLOEXEC 0x40000000	/* Set close_on_exec for file
					   descriptor received through
					   SCM_RIGHTS */
#if defined(CONFIG_COMPAT)
#define MSG_CMSG_COMPAT	0x80000000	/* This message needs 32 bit fixups */
#else
#define MSG_CMSG_COMPAT	0		/* We never have 32 bit fixups */
#endif


/* Setsockoptions(2) level. Thanks to BSD these must match IPPROTO_xxx */
#define SOL_IP		0
/* #define SOL_ICMP	1	No-no-no! Due to Linux :-) we cannot use SOL_ICMP=1 */
#define SOL_TCP		6
#define SOL_UDP		17
#define SOL_IPV6	41
#define SOL_ICMPV6	58
#define SOL_SCTP	132
#define SOL_UDPLITE	136     /* UDP-Lite (RFC 3828) */
#define SOL_RAW		255
#define SOL_IPX		256
#define SOL_AX25	257
#define SOL_ATALK	258
#define SOL_NETROM	259
#define SOL_ROSE	260
#define SOL_DECNET	261
#define	SOL_X25		262
#define SOL_PACKET	263
#define SOL_ATM		264	/* ATM layer (cell level) */
#define SOL_AAL		265	/* ATM Adaption Layer (packet level) */
#define SOL_IRDA        266
#define SOL_NETBEUI	267
#define SOL_LLC		268
#define SOL_DCCP	269
#define SOL_NETLINK	270
#define SOL_TIPC	271
#define SOL_RXRPC	272
#define SOL_PPPOL2TP	273
#define SOL_BLUETOOTH	274
#define SOL_PNPIPE	275
#define SOL_RDS		276
#define SOL_IUCV	277
#define SOL_CAIF	278
#define SOL_ALG		279
#define SOL_NFC		280
#define SOL_KCM		281
#define SOL_TLS		282
#define SOL_XDP		283

/* IPX options */
#define IPX_TYPE	1

#endif /* sys/socket.h */

