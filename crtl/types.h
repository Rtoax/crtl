#ifndef __CRTL_TYPES_H
#define __CRTL_TYPES_H 1

#include <stdbool.h>
#include <stdint.h>
#include <complex.h>
#include <sys/types.h>

#include "crtl/easy/attribute.h"

#include "crtl/bits/types_basic.h"
#include "crtl/bits/bits_set.h"
#include "crtl/bits/ctype.h"
#include "crtl/bits/complex.h"


typedef __crtl_bits_set crtl_bits_set;


#define CRTL_BITS_SETSIZE                    __CRTL_BITS_SETSIZE
#define CRTL_BITS_SET(bit, p_bits_set)       __CRTL_BITS_SET(bit, p_bits_set)
#define CRTL_BITS_CLR(bit, p_bits_set)       __CRTL_BITS_CLR(bit, p_bits_set)
#define CRTL_BITS_ISSET(bit, p_bits_set)     __CRTL_BITS_ISSET(bit, p_bits_set)
#define CRTL_BITS_ZERO(p_bits_set)           __CRTL_BITS_ZERO(p_bits_set)




#define crtl_string_cletters    __crtl_string_cletters


#endif /*<__CRTL_TYPES_H>*/
