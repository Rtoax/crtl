#ifndef __CRTL_TYPES_CTYPE_H
#define __CRTL_TYPES_CTYPE_H 1



#define crtl_isdigit(c) ({ int __c = (c); __c >= '0' && __c <= '9'; })
#define	crtl_isascii(c)	(((c) & ~0x7f) == 0)	/* If C is a 7 bit value.  */
#define	crtl_toascii(c)	((c) & 0x7f)		    /* Mask off high bits.  */


static const char __crtl_string_cletters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";


#endif //__CRTL_TYPES_CTYPE_H