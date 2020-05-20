#ifndef __CRTL_NETWORK_INET_H
#define __CRTL_NETWORK_INET_H 1


#include <stdint.h>


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


/**
 * convert IPv4 addresses from binary to text form
 * @param src: network  address  structure  src in the af address family
 * @param dst: character string
 * @param size: The caller specifies the number of bytes available in this buffer in the argument size.
 * @return error number
 */
int crtl_inet_ntop4(const unsigned char *src, char *dst, size_t size);


/**
 * convert IPv6 addresses from binary to text form
 * @param src: network  address  structure  src in the af address family
 * @param dst: character string
 * @param size: The caller specifies the number of bytes available in this buffer in the argument size.
 * @return error number
 */
int crtl_inet_ntop6(const unsigned char *src, char *dst, size_t size);

/**
 * convert IPv4 addresses from text form to binary
 * @param src: character string
 * @param dst: network  address  structure  src in the af address family
 * @return error number
 */
int crtl_inet_pton4(const char *src, unsigned char *dst);


/**
 * convert IPv6 addresses from text form to binary
 * @param src: character string
 * @param dst: network  address  structure  src in the af address family
 * @return error number
 */
int crtl_inet_pton6(const char *src, unsigned char *dst);

/**
 * convert IPv4 or IPv6 addresses from binary to text form
 * @param src: network  address  structure  src in the af address family
 * @param dst: character string
 * @param size: The caller specifies the number of bytes available in this buffer in the argument size.
 * @return error number
 */
int crtl_inet_ntop(int af, const void* src, char* dst, size_t size);


/**
 * convert IPv4 or IPv6 addresses from text form to binary
 * @param src: character string
 * @param dst: network  address  structure  src in the af address family
 * @return error number
 */
int crtl_inet_pton(int af, const char* src, void* dst);



/**
 * convert values between host and network byte order
 * @param in: num ready to convert
 * @return num after convert
 */
uint16_t crtl_hton16(uint16_t in);

/**
 * convert values between host and network byte order
 * @param in: num ready to convert
 * @return num after convert
 */
uint16_t crtl_ntoh16(uint16_t in);

/**
 * convert values between host and network byte order
 * @param in: num ready to convert
 * @return num after convert
 */
uint32_t crtl_hton32(uint32_t in);

/**
 * convert values between host and network byte order
 * @param in: num ready to convert
 * @return num after convert
 */
uint32_t crtl_ntoh32(uint32_t in);

/**
 * convert values between host and network byte order
 * @param in: num ready to convert
 * @return num after convert
 */
uint64_t crtl_hton64(uint64_t in);

/**
 * convert values between host and network byte order
 * @param in: num ready to convert
 * @return num after convert
 */
uint64_t crtl_ntoh64(uint64_t in);

#endif /*<__CRTL_NETWORK_INET_H>*/
