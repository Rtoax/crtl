#ifndef __CRTL_TCP_IP_BITS_CHECKSUM_H
#define __CRTL_TCP_IP_BITS_CHECKSUM_H 1

/**
 * Check Sum of package
 * @param pkg: package pointer address
 * @param size: package size
 * @return checksum value
 */
inline unsigned short crtl_tcp_ip_chksum1(void *pkg, int size);

#endif /*<__CRTL_TCP_IP_BITS_CHECKSUM_H>*/
