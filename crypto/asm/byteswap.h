#ifndef __CRTL_CRYPTO_EASY_BYTESWAP_H
#define __CRTL_CRYPTO_EASY_BYTESWAP_H 1

/**
 * swab 32 bits byteorder with asm
 * @param x: 32 bits in
 * @return the data after swab
 */
static inline __attribute__((const)) uint32_t crtl_asm_swab32(uint32_t val)
{
	asm("bswapl %0" : "=r" (val) : "0" (val));
	return val;
}

/**
 * swab 64 bits byteorder with asm
 * @param x: 64 bits in
 * @return the data after swab
 */
static inline __attribute__((const)) uint64_t crtl_asm_swab64(uint64_t val)
{
#ifdef __i386__
	union {
		struct {
			uint32_t a;
			uint32_t b;
		} s;
		uint64_t u;
	} v;
	v.u = val;
	asm("bswapl %0 ; bswapl %1 ; xchgl %0,%1"
			: "=r" (v.s.a), "=r" (v.s.b)
			: "0" (v.s.a), "1" (v.s.b));
	return v.u;
#else /* __i386__ */
	asm("bswapq %0" : "=r" (val) : "0" (val));
	return val;
#endif
}


#endif /*<__CRTL_CRYPTO_EASY_BYTESWAP_H>*/
