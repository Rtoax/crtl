#ifndef __CRTL_EASY_BYTESWAP_H
#define __CRTL_EASY_BYTESWAP_H 1

#include <stdint.h>


#ifdef __x86_64__
#define CRTL_WORDSIZE	64
#else
#define CRTL_WORDSIZE	32
#endif



static inline uint16_t crtl_swab16(uint16_t x){
	return  ((x & (uint16_t)0x00ffU) << 8) | ((x & (uint16_t)0xff00U) >> 8);
}

static inline uint32_t crtl_swab32(uint32_t x){
	return  ((x & (uint32_t)0x000000ffUL) << 24) | ((x & (uint32_t)0x0000ff00UL) <<  8) |
    		((x & (uint32_t)0x00ff0000UL) >>  8) | ((x & (uint32_t)0xff000000UL) >> 24);
}

static inline uint64_t crtl_swab64(uint64_t x){
	return  (uint64_t)((x & (uint64_t)0x00000000000000ffULL) << 56) |
    		(uint64_t)((x & (uint64_t)0x000000000000ff00ULL) << 40) |
    		(uint64_t)((x & (uint64_t)0x0000000000ff0000ULL) << 24) |
    		(uint64_t)((x & (uint64_t)0x00000000ff000000ULL) <<  8) |
    		(uint64_t)((x & (uint64_t)0x000000ff00000000ULL) >>  8) |
    		(uint64_t)((x & (uint64_t)0x0000ff0000000000ULL) >> 24) |
    		(uint64_t)((x & (uint64_t)0x00ff000000000000ULL) >> 40) |
    		(uint64_t)((x & (uint64_t)0xff00000000000000ULL) >> 56);
}


#define _swapbyte16(s16) (((s16>>8)&0xff) | ((s16&0xff)<<8))

#define _swapbyte32(i32) (((i32>>24)&0xff) | ((i32&0xff)<<24) |\
                          ((i32>>8)&0xff00) | ((i32&0xff00)<<8))

#define _swapbyte64(l64) (((l64>>56)&0xff)       | ((l64&0xff)<<56) |\
		                  ((l64>>40)&0xff00)     | ((l64&0xff00)<<40) |\
		                  ((l64>>24)&0xff0000)   | ((l64&0xff0000)<<24) |\
		                  ((l64>> 8)&0xff000000) | ((l64&0xff000000)<<8))

#define _swapbyte16_2(x) ((unsigned short int) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))

#define _swapbyte32_2(x) ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |\
                          (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

#define _swapbyte64_2(x) (((((x) & 0xff00000000000000ull) >> 56)	  \
            		     | (((x) & 0x00ff000000000000ull) >> 40)	  \
            		     | (((x) & 0x0000ff0000000000ull) >> 24)	  \
            		     | (((x) & 0x000000ff00000000ull) >> 8)		  \
            		     | (((x) & 0x00000000ff000000ull) << 8)		  \
            		     | (((x) & 0x0000000000ff0000ull) << 24)	  \
            		     | (((x) & 0x000000000000ff00ull) << 40)	  \
            		     | (((x) & 0x00000000000000ffull) << 56)))

#define _swapbyte16_3(x)    ((unsigned short int) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))
#define _swapbyte32_3(x)    ((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8) \
                            | (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24))
#define _swapbyte64_3(x)    ((((x) & 0xff00000000000000ull) >> 56)  \
                            | (((x) & 0x00ff000000000000ull) >> 40)  \
                            | (((x) & 0x0000ff0000000000ull) >> 24)  \
                            | (((x) & 0x000000ff00000000ull) >> 8)   \
                            | (((x) & 0x00000000ff000000ull) << 8)   \
                            | (((x) & 0x0000000000ff0000ull) << 24)  \
                            | (((x) & 0x000000000000ff00ull) << 40)  \
                            | (((x) & 0x00000000000000ffull) << 56))


#endif //__CRTL_EASY_BYTESWAP_H
