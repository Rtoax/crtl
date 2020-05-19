#ifndef __CRTL_RANDOM_H
#define __CRTL_RANDOM_H 1



/**
 * get random integer number
 * @param min: min limit
 * @param max: max limit
 * @return random number
 */
int crtl_random_int(int min, int max);



/**
 * get random gauss float number
 * @param void
 * @return random  gauss float number
 */
float crtl_rand_gauss(void);


#endif /*<__CRTL_RANDOM_H>*/
