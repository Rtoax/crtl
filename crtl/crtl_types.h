#ifndef __CRTL_TYPES_H
#define __CRTL_TYPES_H 1

#include <stdbool.h>
#include <stdint.h>
#include <complex.h>


#include "crtl/bits/crtl_types_bits_set.h"
#include "crtl/bits/crtl_types_ctype.h"
#include "crtl/bits/crtl_types_string.h"
#include "crtl/bits/crtl_types_multiDim.h"
#include "crtl/bits/crtl_types_complex.h"
#include "crtl/bits/crtl_types_float.h"
#include "crtl/bits/crtl_types_integer.h"
#include "crtl/bits/crtl_types_syserrno.h"
#include "crtl/bits/crtl_types_syssignum.h"


typedef enum {
    crtl_false = false,
    crtl_true = true,
}crtl_boolean;


enum{
    CRTL_0 = 0,
    CRTL_1,    
};

enum {
    CRTL_ERROR = -1,
    CRTL_SUCCESS,
    CRTL_SPECIAL,
};

enum {
    CRTL_LT = -1,
    CRTL_EQ = 0,
    CRTL_GT = 1,
};


typedef void *          crtl_data_t;
typedef unsigned char   crtl_byte;
typedef __crtl_bits_set crtl_bits_set;



#define CRTL_BITS_SETSIZE                    __CRTL_BITS_SETSIZE
#define CRTL_BITS_SET(bit, p_bits_set)       __CRTL_BITS_SET(bit, p_bits_set)
#define CRTL_BITS_CLR(bit, p_bits_set)       __CRTL_BITS_CLR(bit, p_bits_set)
#define CRTL_BITS_ISSET(bit, p_bits_set)     __CRTL_BITS_ISSET(bit, p_bits_set)
#define CRTL_BITS_ZERO(p_bits_set)           __CRTL_BITS_ZERO(p_bits_set)


#define crtl_string_cletters    __crtl_string_cletters


#endif /*<__CRTL_TYPES_H>*/
