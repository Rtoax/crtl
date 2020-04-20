/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/* const.h: Macros for dealing with constants.  */

#ifndef __CRTL_BITS_CONST_H
#define __CRTL_BITS_CONST_H 1

/* Some constant macros are used in both assembler and
 * C code.  Therefore we cannot annotate them always with
 * 'UL' and other type specifiers unilaterally.  We
 * use the following macros to deal with this.
 *
 * Similarly, _AT() will cast an expression with a type in C, but
 * leave it unchanged in asm.
 */

#ifdef __ASSEMBLY__
#define _CRTL_AC(X,Y)	X
#define _CRTL_AT(T,X)	X
#else
#define __CRTL_AC(X,Y)	(X##Y)
#define _CRTL_AC(X,Y)	__CRTL_AC(X,Y)
#define _CRTL_AT(T,X)	((T)(X))
#endif

#define _CRTL_UL(x)		(_CRTL_AC(x, UL))
#define _CRTL_ULL(x)		(_CRTL_AC(x, ULL))

#define _CRTL_BITUL(x)	(_CRTL_UL(1) << (x))
#define _CRTL_BITULL(x)	(_CRTL_ULL(1) << (x))


#define CRTL_UL(x)		(_CRTL_UL(x))
#define CRTL_ULL(x)		(_CRTL_ULL(x))



#endif /* __CRTL_BITS_CONST_H */
