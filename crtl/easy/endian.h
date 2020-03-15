#ifndef __CRTL_EASY_ENDIAN_H
#define __CRTL_EASY_ENDIAN_H 1


#include <endian.h>
#include <stdint.h>
#include <netinet/in.h>

#include "crtl/easy/attribute.h"

#if PLATFORM_ENV_IS_LITTLE_ENDIAN && PLATFORM_ENV_IS_BIG_ENDIAN
#error "Can't define 1 PLATFORM_ENV_IS_LITTLE_ENDIAN and PLATFORM_ENV_IS_BIG_ENDIAN as the same time."
#endif

#define IS_BIG_ENDIAN       __IS_BIG_ENDIAN
#define IS_LITTLE_ENDIAN    __IS_LITTLE_ENDIAN
#define IS_NET_ENDIAN       __IS_NET_ENDIAN

/* system endian define */
#define RT_BYTE_ORDER       BYTE_ORDER
#define RT_BIG_ENDIAN       BIG_ENDIAN
#define RT_LITTLE_ENDIAN    LITTLE_ENDIAN

/* global union */
const static union 
{
    char _c[4];
    unsigned long _long;
#define __IS_BIG_ENDIAN       (!!(((char )__rt_endian_test._long)=='b'))
#define __IS_LITTLE_ENDIAN    (!!(((char )__rt_endian_test._long)=='l'))
#define __IS_NET_ENDIAN       __IS_BIG_ENDIAN
}_unused __rt_endian_test  = {{'l', '?', '?', 'b'}};





#endif //__CRTL_EASY_ENDIAN_H

