#ifndef __CRTL_CRYPTO_ATOMIC_SYNC_FETCH_H
#define __CRTL_CRYPTO_ATOMIC_SYNC_FETCH_H 1


#define _fetch_and_add(ptr, v)  __sync_fetch_and_add(ptr, v)
#define _fetch_and_sub(ptr, v)  __sync_fetch_and_sub(ptr, v)
#define _fetch_and_or(ptr, v)  __sync_fetch_and_or(ptr, v)
#define _fetch_and_and(ptr, v)  __sync_fetch_and_and(ptr, v)
#define _fetch_and_xor(ptr, v)  __sync_fetch_and_xor(ptr, v)
#define _fetch_and_nand(ptr, v)  __sync_fetch_and_nand(ptr, v)


#define _add_and_fetch(ptr, v)  __sync_add_and_fetch(ptr, v)
#define _sub_and_fetch(ptr, v)  __sync_sub_and_fetch(ptr, v)
#define _or_and_fetch(ptr, v)  __sync_or_and_fetch(ptr, v)
#define _and_and_fetch(ptr, v)  __sync_and_and_fetch(ptr, v)
#define _xor_and_fetch(ptr, v)  __sync_xor_and_fetch(ptr, v)
#define _nand_and_fetch(ptr, v)  __sync_nand_and_fetch(ptr, v)


/* put addr to the Cache before use */
#ifndef prefetch
#define prefetch(x) __builtin_prefetch(x)
#endif
 
#ifndef prefetchw
#define prefetchw(x) __builtin_prefetch(x, 1)
#endif


#endif /*<__CRTL_CRYPTO_ATOMIC_SYNC_FETCH_H>*/
