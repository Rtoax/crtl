#ifndef __RT_MATH_INT_H
#define __RT_MATH_INT_H 1

#include <math.h>
#include <stdint.h>


/*div_t div (int numerator, int denominator)*/
#define t_div(numerator, denominator) div(numerator, denominator)

/*ldiv_t ldiv (long int numerator, long int denominator)*/
#define t_ldiv(numerator, denominator) ldiv(numerator, denominator)

/*lldiv_t lldiv (long long int numerator, long long int denominator)*/
#define t_lldiv(numerator, denominator) lldiv(numerator, denominator)

/*imaxdiv_t imaxdiv (intmax t numerator, intmax t denominator)*/
#define t_imaxdiv(numerator, denominator) imaxdiv(numerator, denominator)




#endif /*<__RT_MATH_INT_H>*/
