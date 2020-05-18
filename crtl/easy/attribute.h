#ifndef __CRTL_EASY_ATTRIBUTE_H
#define __CRTL_EASY_ATTRIBUTE_H 1

#include "crtl/easy/extern.h"

/**
 * some useful amcor
 */


#ifdef __GNUC__
#define _packed             __attribute__((packed))
#define _noreturn           __attribute__((noreturn))
#define _unused             __attribute__((unused))
#define _used               __attribute__((used))
#define _const              __attribute__((const))
#define _api                __attribute__((visibility("default")))
#define _hidden             __attribute__((visibility("hidden")))
#define _aligned(x)         __attribute__((aligned(x)))
#define _format(func, a, b) __attribute__((__format__ (func, a, b)))
#define _bitwise            __attribute__((bitwise))
#define _inline inline      __attribute__((always_inline))
#define _nonnull            __nonnull
#define _alias(symbol)      __attribute__((__alias__(#symbol)))
//#define _aligned(x)         __attribute__((__aligned__(x)))
#define _aligned_largest    __attribute__((__aligned__))
#define _naked			    __attribute__((__naked__))
#define _weak               __attribute__((weak))//变量加上weak时，是弱符号。函数加上weak时，是弱引用，可以不实现。
#define _pure		        __attribute__((pure))

#else

#define _packed             
#define _noreturn           
#define _unused   
#define _used
#define _const              
#define _api                
#define _hidden             
#define _aligned(x)         
#define _format(func, a, b) 
#define _bitwise 
#define _inline
#define _nonnull            
#define _alias(symbol)
//#define _aligned(x)
#define _aligned_largest
#define _naked	
#define _weak
#define _pure

#endif



#endif //__CRTL_EASY_ATTRIBUTE_H

