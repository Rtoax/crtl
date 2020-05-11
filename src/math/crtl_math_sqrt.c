#include <math.h>
#include <stdio.h>

#include "crtl/crtl_assert.h"
#include "crtl/bits/crtl_math_sqrt.h"


/* 快速平方根取倒数 */
/* compile with: -fno-strict-aliasing/ -Wno-strict-aliasing */
float crtl_qrsqrt(float number)
{
#define _MAGIC_NUM1  0x5f3759df
#define _MAGIC_NUM2  0x5f375a86

    long i;
    float x2, y;
    const float threehalfs = 1.5F;
    x2  = number * 0.5F;
    y   = number;
    i   = * ( long * ) &y; 
    // evil floating point bit level hacking
    i   = _MAGIC_NUM2 - ( i >> 1 ); // what the fuck?
    y   = * ( float * ) &i;
    y   = y * ( threehalfs - ( x2 * y * y ) ); 
    // 1st iteration
    // y   = y * ( threehalfs - ( x2 * y * y ) );
    // 2nd iteration, this can be removed
#ifndef Q3_VM
#ifdef __linux__
    crtl_assert_fp(stderr, !isnan(y) );
    // bk010122 - FPE?
#endif   
#endif    
    //
    //        ▌▒█           ▄▀▒▌     
    //        ▌▒▒▀▄       ▄▀▒▒▒▐     
    //       ▐▄▀▒▒▀▀▀▀▄▄▄▀▒▒▒▒▒▐     
    //     ▄▄▀▒▒▒▒▒▒▒▒▒▒▒█▒▒▄█▒▐     
    //   ▄▀▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▄█▒▐ 
    //  ▐▒▒▒▄▄▄▒▒▒▒▒▒▒▒▒▒▒▒▒▀▄▒▒▌     66666666666
    //  ▌▒▒▐▄█▀▒▒▒▒▄▀█▄▒▒▒▒▒▒▒█▒▐    
    // ▐▒▒▒▒▒▒▒▒▒▒▒▌██▀▒▒▒▒▒▒▒▒▀▄▌   
    // ▌▒▀▄██▄▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒▒▒▌   
    // ▌▀▐▄█▄█▌▄▒▀▒▒▒▒▒▒░░░░░░▒▒▒▐   
    //▐▒▀▐▀▐▀▒▒▄▄▒▄▒▒▒▒▒▒▒▒▒▒▒▒▒▒▌  
    //▐▒▒▒▀▀▄▄▒▒▒▄▒▒▒▒▒▒░░░░░░▒▒▒▐   
    // ▌▒▒▒▒▒▒▀▀▀▒▒▒▒▒▒▒▒░░░░▒▒▒▒▌   
    // ▐▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▐    
    //  ▀▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▄▒▒▒▒▌    
    //    ▀▄▒▒▒▒▒▒▒▒▒▒▄▄▄▀▒▒▒▒▄▀     
    //   ▐▀▒▀▄▄▄▄▄▄▀▀▀▒▒▒▒▒▄▄▀       
    //  ▐▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▀▀         
    //

    return y;
}

