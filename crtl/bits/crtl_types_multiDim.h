#ifndef __CRTL_TYPES_MULTIDIM_H
#define __CRTL_TYPES_MULTIDIM_H 1

#include "crtl/easy/attribute.h"


typedef struct {
    int x;
    void *value;
}_packed crtl_1d_t;


typedef struct {
    int x;
    int y;
    void **value;
}_packed crtl_2d_t;


typedef struct {
    int x;
    int y;
    int z;
    void ***value;
}_packed crtl_3d_t;


#endif /*<__CRTL_TYPES_MULTIDIM_H>*/
