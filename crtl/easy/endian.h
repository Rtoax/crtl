#ifndef __CRTL_EASY_ENDIAN_H
#define __CRTL_EASY_ENDIAN_H 1


#include <endian.h>
#include <stdint.h>
#include <netinet/in.h>

#include "crtl/easy/attribute.h"


#define IS_BIG_ENDIAN       __IS_BIG_ENDIAN
#define IS_LITTLE_ENDIAN    __IS_LITTLE_ENDIAN
#define IS_NET_ENDIAN       __IS_NET_ENDIAN

/* system endian define */
#define RT_BYTE_ORDER       BYTE_ORDER
#define RT_BIG_ENDIAN       BIG_ENDIAN
#define RT_LITTLE_ENDIAN    LITTLE_ENDIAN

#if BIG_ENDIAN
#define __BIG_ENDIAN 4321
#else
#define __LITTLE_ENDIAN 1234
#endif


/* global union */
const static union 
{
    char _c[4];
    unsigned long _long;
#define __IS_BIG_ENDIAN       (!!(((char )__crtl_endian_test._long)=='b'))
#define __IS_LITTLE_ENDIAN    (!!(((char )__crtl_endian_test._long)=='l'))
#define __IS_NET_ENDIAN       __IS_BIG_ENDIAN
}_unused __crtl_endian_test  = {{'l', '?', '?', 'b'}};





#endif //__CRTL_EASY_ENDIAN_H

