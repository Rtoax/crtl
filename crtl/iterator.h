#ifndef __CRTL_ITERATOR_H
#define __CRTL_ITERATOR_H 1

#include "crtl/bits/types_basic.h"

typedef struct crtl_iterator_struct {

    crtl_data_t root;
    crtl_data_t current_data;
    
    crtl_data_t (*begin)();
    crtl_data_t (*next)();
    crtl_data_t (*prev)();
    crtl_data_t (*end)();
    crtl_data_t (*reset)();
}crtl_iterator_t;





#endif /*<__CRTL_ITERATOR_H>*/

