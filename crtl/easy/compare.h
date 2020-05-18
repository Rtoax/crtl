#ifndef __CRTL_EASY_COMPARE_H
#define __CRTL_EASY_COMPARE_H 1


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



#endif /*<__CRTL_EASY_COMPARE_H>*/
