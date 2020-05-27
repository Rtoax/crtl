#include <string.h>
#include <stdlib.h>
#include <math.h>


float crtl_rand_gauss(void) 
{
    float v1,v2,s;

    do {
        v1 = 2.0 * ((float) rand()/RAND_MAX) - 1;
        v2 = 2.0 * ((float) rand()/RAND_MAX) - 1;

        s = v1*v1 + v2*v2;
    } while ( s >= 1.0 || s == 0.0);

    return (v1*sqrt(-2.0 * log(s) / s));
}

