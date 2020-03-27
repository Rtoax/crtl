#ifndef __CRTL_TYPES_CTYPE_H
#define __CRTL_TYPES_CTYPE_H 1


 #include <ctype.h>

#define crtl_isalnum(c) isalnum(c)
#define crtl_isalpha(c) isalpha( c)
#define crtl_isascii(c) isascii( c)
#define crtl_isblank(c) isblank( c)
#define crtl_iscntrl(c) iscntrl( c)
#define crtl_isdigit(c) isdigit( c)
#define crtl_isgraph(c) isgraph( c)
#define crtl_islower(c) islower( c)
#define crtl_isprint(c) isprint( c)
#define crtl_ispunct(c) ispunct( c)
#define crtl_isspace(c) isspace( c)
#define crtl_isupper(c) isupper( c)
#define crtl_isxdigit(c) isxdigit( c)


#define crtl_iswhite(c)	((c) == ' ' || (c) == '\t' || (c) == '\n')
//#define crtl_isdigit(c) ({ int __c = (c); __c >= '0' && __c <= '9'; })
//#define	crtl_isascii(c)	(((c) & ~0x7f) == 0)	/* If C is a 7 bit value.  */
#define	crtl_toascii(c)	((c) & 0x7f)		    /* Mask off high bits.  */


#define crtl_ischar(ch) \
   ( crtl_isalnum(ch)|crtl_isalpha(ch)|crtl_isascii(ch)|crtl_isblank(ch)|\
    crtl_iscntrl(ch)|crtl_isdigit(ch)|crtl_isgraph(ch)|crtl_islower(ch)|crtl_isprint(ch)|\
    crtl_ispunct(ch)|crtl_isspace(ch)|crtl_isupper(ch)|crtl_isxdigit(ch) )


static const char __crtl_string_cletters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";


#endif //__CRTL_TYPES_CTYPE_H