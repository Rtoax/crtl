
/**
 *  x=8: 0000 0000 0000 1000; location of suffix "1" . 
 *  1000 -> 4 
 */
#ifndef _ffs
#define _ffs(integer) __builtin_ffs(integer)
#endif

/**
 * _ctzl - find first bit in word.
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
#ifndef _ctzl
#define _ctzl(long_word) __builtin_ctzl(long_word)
#endif

/*
 * _ffz - find first zero in word.
 * @word: The word to search
 *
 * Undefined if no zero exists, so code should check against ~0UL first.
 */
#ifndef _ffz
#define _ffz(x)  _ctzl(~(x))
#endif

/**
 * _ffs64 - find first set bit in a 64 bit word
 * @word: The 64 bit word
 *
 * On 64 bit arches this is a synomyn for _ctzl
 * The result is not defined if no bits are set, so check that @word
 * is non-zero before calling this.
 */
#ifndef _ffs64
#define _ffs64(word)  _ctzl((unsigned long)word)
#endif

/**
 * _fls32 - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as _ffs.
 * Note _fls32(0) = 0, _fls32(1) = 1, _fls32(0x80000000) = 32.
 */
#ifndef _fls32
#define _fls32(x) (x ? sizeof(int) * 8 - __builtin_clz(x) : 0)
#endif

/**
 * _fls64 - find last (most-significant) set bit in a long word
 * @word: the word to search
 *
 * Undefined if no set bit exists, so code should check against 0 first.
 */
#ifndef _fls64
#define _fls64(word) ((sizeof(long) * 8) - 1 - __builtin_clzl(word))
#endif

/**
 * fls64 - find last set bit in a 64-bit word
 * @x: the word to search
 *
 * This is defined in a similar way as the libc and compiler builtin
 * ffsll, but returns the position of the most significant set bit.
 *
 * fls64(value) returns 0 if value is 0 or the position of the last
 * set bit if value is nonzero. The last (most significant) bit is
 * at position 64.
 */
#ifndef fls64
#define fls64(x) (x?_fls64(x) + 1:0)
#endif

#ifndef fls_long
#define fls_long(l) fls64(l)
#endif

/**
 *  x=8: 0000 0000 0000 1000; number of "1" 
 *  8 -> 1
 */
#ifndef _popcount
#define _popcount(integer) __builtin_popcount(integer)
#endif

/**
 *  x=8: 0000 0000 0000 1000;  number of low "0" 
 *  8 -> 3
 */
#ifndef _ctz
#define _ctz(x) __builtin_ctz(x)
#endif


/**
 *  x=8: 0000 0000 0000 1000;  number of high "0" 
 *  8 -> 28
 */
#ifndef _clz
#define _clz(x) __builtin_clz(x)
#endif

/**
 *  x=8: 0000 0000 0000 1000;  parity of "1" 
 *  8 -> 0
 *  12 -> 0
 *  13 -> 1
 *  1 -> 1
 */
#ifndef _parity
#define _parity(x) __builtin_parity(x)
#endif


/**
 * fls - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
 */
#ifndef fls
#define fls(x) ((unsigned int )x ? sizeof(x) * 8 - __builtin_clz(x) : 0)
#endif

