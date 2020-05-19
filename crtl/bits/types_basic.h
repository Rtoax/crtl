#ifndef __CRTL_BITS_TYPES_BASIC_H
#define __CRTL_BITS_TYPES_BASIC_H 1

#include <stdbool.h>

typedef enum {
    crtl_false = false,
    crtl_true = true,
}crtl_boolean;


enum{
    CRTL_0 = 0,
    CRTL_1,    
};

enum {
    CRTL_ERROR = -2,
    CRTL_NOTSUPPORT,
    CRTL_SUCCESS = crtl_true,
    CRTL_SPECIAL,
};

enum {
    CRTL_LT = -1,
    CRTL_EQ = 0,
    CRTL_GT = 1,
};


#endif /*<__CRTL_BITS_TYPES_BASIC_H>*/
