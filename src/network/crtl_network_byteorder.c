#include "crtl/log.h"
#include "crtl/network/inet.h"
#include "crtl/easy/attribute.h"
#include "crtl/easy/endian.h"
#include "crtl/easy/byteswap.h"



uint16_t crtl_hton16(uint16_t in)
{
    if(IS_BIG_ENDIAN)
        return in;
    else if(IS_LITTLE_ENDIAN)
        return (uint16_t)_swapbyte16(in);
    return in;
}
uint16_t crtl_ntoh16(uint16_t in)
{
    if(IS_BIG_ENDIAN)
        return in;
    else if(IS_LITTLE_ENDIAN)
        return (uint16_t)_swapbyte16(in);
    return in;
}
uint32_t crtl_hton32(uint32_t in)
{
    if(IS_BIG_ENDIAN)
        return in;
    else if(IS_LITTLE_ENDIAN)
        return (uint32_t)_swapbyte32(in);
    return in;
}
uint32_t crtl_ntoh32(uint32_t in)
{
    if(IS_BIG_ENDIAN)
        return in;
    else if(IS_LITTLE_ENDIAN)
        return (uint32_t)_swapbyte32(in);
    return in;
}
uint64_t crtl_hton64(uint64_t in)
{
    if(IS_BIG_ENDIAN)
        return in;
    else if(IS_LITTLE_ENDIAN)
        return (uint64_t)_swapbyte64(in);
    return in;
}
uint64_t crtl_ntoh64(uint64_t in)
{
    if(IS_BIG_ENDIAN)
        return in;
    else if(IS_LITTLE_ENDIAN)
        return (uint64_t)_swapbyte64(in);
    return in;
}

