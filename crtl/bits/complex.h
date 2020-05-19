#ifndef __CRTL_BITS_TYPES_COMPLEX_H
#define __CRTL_BITS_TYPES_COMPLEX_H 1

#include <complex.h>




typedef struct { /* complex number */
	float r,i;
} crtl_fcomplex;


typedef struct  { /* double-precision complex number */
	double r,i;
} crtl_dcomplex;



#ifndef _COMPLEX_H
/* system complex.h hdr */
#define CMPLX(x, y) __builtin_complex ((double) (x), (double) (y))
#define CMPLXF(x, y) __builtin_complex ((float) (x), (float) (y))
#define CMPLXL(x, y) __builtin_complex ((long double) (x), (long double) (y))
#endif //_COMPLEX_H


#endif /*<__CRTL_BITS_TYPES_COMPLEX_H>*/