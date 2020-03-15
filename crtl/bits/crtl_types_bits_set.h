#ifndef __CRTL_BITS_TYPES_BITS_SET_H
#define __CRTL_BITS_TYPES_BITS_SET_H 1


#define bits_set                        __crtl_bits_set
#define BITS_SETSIZE                    __CRTL_BITS_SETSIZE
#define BITS_SET(bit, p_bits_set)       __CRTL_BITS_SET(bit, p_bits_set)
#define BITS_CLR(bit, p_bits_set)       __CRTL_BITS_CLR(bit, p_bits_set)
#define BITS_ISSET(bit, p_bits_set)     __CRTL_BITS_ISSET(bit, p_bits_set)
#define BITS_ZERO(p_bits_set)           __CRTL_BITS_ZERO(p_bits_set)


/**
 *  Crypto
 */
#define __CRTL_BITS_SETSIZE  256
#define __CRTL_NBITS     (8 * (int) sizeof (__crtl_bits_mask))
#define __CRTL_BITS_ELT(d)   ((d) / __CRTL_NBITS)
#define __CRTL_BITS_MASK(d)  ((__crtl_bits_mask)(1UL << ((d) % __CRTL_NBITS)))

typedef long int __crtl_bits_mask;
typedef struct {
    __crtl_bits_mask __bits[__CRTL_BITS_SETSIZE/__CRTL_NBITS];
#define __CRTL_BITS(set) ((set)->__bits)
}__crtl_bits_set;


#define __CRTL_BITS_ZERO(s) \
    do {    \
        unsigned int __i;   \
        __crtl_bits_set *__arr = (s);  \
        for (__i = 0; __i < sizeof (__crtl_bits_set) / sizeof (__crtl_bits_mask); ++__i)    \
            __CRTL_BITS (__arr)[__i] = 0;  \
    } while (0)
#define __CRTL_BITS_SET(d, s) \
    ((void) (__CRTL_BITS (s)[__CRTL_BITS_ELT(d)] |= __CRTL_BITS_MASK(d)))
#define __CRTL_BITS_CLR(d, s) \
    ((void) (__CRTL_BITS (s)[__CRTL_BITS_ELT(d)] &= ~ __CRTL_BITS_MASK(d)))
#define __CRTL_BITS_ISSET(d, s) \
    ((__CRTL_BITS (s)[__CRTL_BITS_ELT (d)] & __CRTL_BITS_MASK (d)) != 0)



#endif /*<__CRTL_BITS_TYPES_BITS_SET_H>*/

