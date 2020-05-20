#ifndef __CRTL_EASY_COMPARE_H
#define __CRTL_EASY_COMPARE_H 1



#define and &&
#define or  ||

#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define MIN(a,b) ((a) <= (b) ? (a) : (b))
#define MAX(a,b) ((a) >= (b) ? (a) : (b))



/**
 * max number
 * @param x: x
 * @param y: y
 */
#ifndef max
#define max(x, y) ({				\
	typeof(x) _max1 = (x);			\
	typeof(y) _max2 = (y);			\
	(void) (&_max1 == &_max2);		\
	_max1 > _max2 ? _max1 : _max2; })
#endif

/**
 * min number
 * @param x: x
 * @param y: y
 */
#ifndef min
#define min(x, y) ({				\
	typeof(x) _min1 = (x);			\
	typeof(y) _min2 = (y);			\
	(void) (&_min1 == &_min2);		\
	_min1 < _min2 ? _min1 : _min2; })
#endif




/* compare t1 with t2 is same as each other: yes -> 1, no -> 0 */
#ifndef typecmp
#define typecmp(t1, t2) __builtin_types_compatible_p(t1, t2)
#endif




#endif /*<__CRTL_EASY_COMPARE_H>*/

