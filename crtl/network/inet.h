#ifndef __CRTL_NETWORK_INET_H
#define __CRTL_NETWORK_INET_H 1


#include <stdint.h>
#include <netinet/in.h>

#include "crtl/easy/endian.h"
#include "crtl/easy/byteswap.h"
#include "crtl/easy/attribute.h"


/**
 *  There are some old version
 *  Rongtao 2019.07.20
 */
/*	file name: 		if.h
 *	author:			Rong Tao
 *	create time:	2018.11.14
 * 	
 */
#ifndef _T_SYS_NETOWRK_IF_H_
#define _T_SYS_NETOWRK_IF_H_

#include <net/if.h>

/**
	struct if_nameindex{
		unsigned int if_index;
		char *if_name;
	}
*/
#define T_IFNAMSIZ IFNAMSIZ
typedef struct if_nameindex t_if_nameindex;

/**
 *	Marco: all if_nameindex func define/typedef
 *	Author: Rong Tao
 *	Time:	2018.11.14
 */
#define t_if_nametoindex(ifname) if_nametoindex (ifname)
#define t_if_indextoname(ifindex, ifname) if_indextoname(ifindex, ifname)
#define t_if_nameindex() if_nameindex()
#define t_if_freenameindex(pifname) if_freenameindex(pifname)


/**
 *	Show all if_nameindex in terminal
 *	Author: Rong Tao
 *	Time:	2018.11.14
 */
int crtl_if_showallnameindexs(void);

/**
 *	is Index exist
 *	Author: Rong Tao
 *	Time:	2018.11.14
 */
int crtl_if_indexexist(unsigned int if_index);

/**
 *	is if_name exist
 *	Author: Rong Tao
 *	Time:	2018.11.14
 */
int crtl_if_nameexist(const char *if_name);


#endif /*<_T_SYS_NETOWRK_IF_H_>*/



int crtl_inet_ntop4(const unsigned char *src, char *dst, size_t size);
int crtl_inet_ntop6(const unsigned char *src, char *dst, size_t size);
int crtl_inet_pton4(const char *src, unsigned char *dst);
int crtl_inet_pton6(const char *src, unsigned char *dst);


int crtl_inet_ntop(int af, const void* src, char* dst, size_t size);
int crtl_inet_pton(int af, const char* src, void* dst);



#endif /*<__CRTL_NETWORK_INET_H>*/