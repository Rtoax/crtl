#ifndef __CRTL_TCP_IP_BITS_BYTEORDER_H
#define __CRTL_TCP_IP_BITS_BYTEORDER_H 1

#include "crtl/easy/attribute.h"
#include "crtl/easy/endian.h"
#include "crtl/easy/byteswap.h"



uint16_t crtl_hton16(uint16_t in);
uint16_t crtl_ntoh16(uint16_t in);
uint32_t crtl_hton32(uint32_t in);
uint32_t crtl_ntoh32(uint32_t in);
uint64_t crtl_hton64(uint64_t in);
uint64_t crtl_ntoh64(uint64_t in);



#endif //__CRTL_TCP_IP_BITS_BYTEORDER_H
