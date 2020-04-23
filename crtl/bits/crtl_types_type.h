#ifndef __CRTL_BITS_TYPES_TYPE_H
#define __CRTL_BITS_TYPES_TYPE_H 1

#include <stddef.h>

#include "crtl/bits/crtl_types_size.h"
#include "crtl/bits/crtl_types_fn.h"



/* type style */
typedef enum 
{
    CRTL_TYPE_INVALID = 0, 
    CRTL_TYPE_C_BUILTIN, 
    CRTL_TYPE_USER_DEFINE, 
    CRTL_TYPE_CRTL_BUILTIN
}crtl_typestyle_t;


typedef struct 
{
    size_t               _t_typesize;                        /* type size */
    char                 _s_typename[CRTL_TYPE_NAME_SIZE + 1];   /* type name */
    crtl_typestyle_t    _t_style;                           /* type style */
    crtl_binary_fn_t    _t_typecopy;                        /* type copy function */
    crtl_binary_fn_t    _t_typeless;                        /* type less function */
    crtl_unary_fn_t     _t_typeinit;                        /* type initialize function */
    crtl_unary_fn_t     _t_typedestroy;                     /* type destroy function */
}crtl_type_t;



#endif /*<__CRTL_BITS_TYPES_TYPE_H>*/

