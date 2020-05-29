/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/* const.h: Macros for dealing with constants.  */

#ifndef __CRYPTO_BITS_CONST_H
#define __CRYPTO_BITS_CONST_H 1

/* Some constant macros are used in both assembler and
 * C code.  Therefore we cannot annotate them always with
 * 'UL' and other type specifiers unilaterally.  We
 * use the following macros to deal with this.
 *
 * Similarly, _AT() will cast an expression with a type in C, but
 * leave it unchanged in asm.
 */

#ifdef __ASSEMBLY__
#define _AC(X,Y)	X
#define _AT(T,X)	X
#else
#define __AC(X,Y)	(X##Y)
#define _AC(X,Y)	__AC(X,Y)
#define _AT(T,X)	((T)(X))
#endif

#define _UL(x)		(_AC(x, UL))
#define _ULL(x)		(_AC(x, ULL))

#define _BITUL(x)	(_UL(1) << (x))
#define _BITULL(x)	(_ULL(1) << (x))


#define CRTL_UL(x)		(_UL(x))
#define CRTL_ULL(x)		(_ULL(x))


/* is constant during compiled:  yes -> 1, no -> 0 */
#ifndef constant_p
#define constant_p(v) __builtin_constant_p(v)
#endif



#endif /* __CRYPTO_BITS_CONST_H */

