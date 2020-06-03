#ifndef __CRTL_CRYPTO_BIT_BYTESWAP_H
#define __CRTL_CRYPTO_BIT_BYTESWAP_H 1


#include "crtl/bits/byteswap.h"

/**
 *  Some other swap macro
 */
#if BYTE_ORDER == LITTLE_ENDIAN
#define le16_to_cpu(val) (val)
#define le32_to_cpu(val) (val)
#define le64_to_cpu(val) (val)
#define be16_to_cpu(val) _swapbyte16(val)
#define be32_to_cpu(val) _swapbyte32(val)
#define be64_to_cpu(val) _swapbyte64(val)

#define cpu_to_le16(val) (val)
#define cpu_to_le32(val) (val)
#define cpu_to_le64(val) (val)
#define cpu_to_be16(val) _swapbyte16(val)
#define cpu_to_be32(val) _swapbyte32(val)
#define cpu_to_be64(val) _swapbyte64(val)
#endif
#if BYTE_ORDER == BIG_ENDIAN
#define le16_to_cpu(val) _swapbyte16(val)
#define le32_to_cpu(val) _swapbyte32(val)
#define le64_to_cpu(val) _swapbyte64(val)
#define be16_to_cpu(val) (val)
#define be32_to_cpu(val) (val)
#define be64_to_cpu(val) (val)

#define cpu_to_le16(val) _swapbyte16(val)
#define cpu_to_le32(val) _swapbyte32(val)
#define cpu_to_le64(val) _swapbyte64(val)
#define cpu_to_be16(val) (val)
#define cpu_to_be32(val) (val)
#define cpu_to_be64(val) (val)
#endif


#endif //__CRTL_CRYPTO_BIT_BYTESWAP_H
