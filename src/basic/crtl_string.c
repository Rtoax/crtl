#include <limits.h>  /* SSIZE_MAX */

#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <errno.h>

#include "crtl/string.h"

#include "crtl/log.h"

#include "crtl/bits/types_basic.h"
#include "crtl/bits/ctype.h"
#include "crtl/bits/regex.h"
#include "crtl/bits/types_basic.h"

#include "crtl/easy/attribute.h"
#include "crtl/easy/macro.h"
#include "crypto/expect.h"



_api char *crtl_skip_spaces(const char *str)
{
	while (isspace(*str))
		++str;
	return (char *)str;
}


/* string api */
_api int crtl_strstripc(char *str, const char ch)
{    
    if((void*)str == NULL)    
    {        
        return CRTL_ERROR;    
    }    

    if(strlen(str) <= 1)
    {
        return CRTL_ERROR; 
    }

    if(!crtl_ischar(ch))
    {
        crtl_print_err("Can not strip CHAR from %s\n", (char*)str);
        return CRTL_ERROR; 
    }
    
    int ilen, len = strlen(str);  
    
    for(ilen=0; ilen < len; ilen++)    
    {        
        if(str[ilen] == ch)        
        {            
            int i;            
            for(i=ilen; i<len-1; i++)            
            {                
                str[i] = str[i+1];            
            }            
            str[len-1] = '\0';            
            len--;
        }    
    }    
    return CRTL_SUCCESS;
}




/* Count characters in a string */
_api int crtl_strccount (char c, char *s)
{
    int i, count = 0;
    for (i = 0, count = 0; s[i] != 0; i++)
        if(s[i] == c) count++;
    return count;
}


_api void crtl_strchop(char *s, char *t)
{
    while ( (*s != ',') && (*s != '\0') ) 
    {
         *t++ = *s++;
    }
    *t='\0';
}



_api ssize_t crtl_strscpy(char* d, const char* s, size_t n) 
{
  size_t i;

  for (i = 0; i < n; i++)
    if ('\0' == (d[i] = s[i]))
      return i > SSIZE_MAX ? CRTL_ERROR : (ssize_t) i;

  if (i == 0)
    return 0;

  d[--i] = '\0';

  return CRTL_ERROR;
}


/* estrtoh - convert string s to short integer {SHRT_MIN:SHRT_MAX} */
_api short crtl_estrtoh(char *s)
{
    long n = strtol(s, NULL, 10);
    
    if ( (n > SHRT_MAX) || (n < SHRT_MIN) || (errno == ERANGE) )
    {   
        crtl_print_err("Estrtoh: overflow\n");
        return 0;
    }

    return (short) n;
}


/* Estrtous - convert string s to unsigned short integer {0:USHRT_MAX} */
_api unsigned short crtl_estrtous(char *s)
{
    unsigned long n = strtoul(s, NULL, 10);

    if ( (n > USHRT_MAX) || (errno == ERANGE) )
    {   
        crtl_print_err("Estrtous: overflow\n");
        return 0;
    }

    return (unsigned short) n;
}


/* Estrtoi - convert string s to integer {INT_MIN:INT_MAX} */
_api int crtl_estrtoi(char *s)
{
    long n = strtol(s, NULL, 10);

    if ( (n > INT_MAX) || (n < INT_MIN) || (errno == ERANGE) )
    {   
        crtl_print_err("Estrtoi: overflow\n");
        return 0;
    }

    return (int) n;
}


/* Estrtoui - convert string s to unsigned integer {0:UINT_MAX} */
_api unsigned int crtl_estrtoui(char *s)
{
    unsigned long n = strtoul(s, NULL, 10);

    if ( (n > UINT_MAX) || (errno == ERANGE) )
    {   
        crtl_print_err("Estrtoui: overflow\n");
        return 0;
    }

    return (unsigned int) n;
}


/* Estrtol - convert string s to long integer {LONG_MIN:LONG_MAX} */
_api long crtl_estrtol(char *s)
{
    long n = strtol(s, NULL, 10);

    if (errno == ERANGE)
    {   
        crtl_print_err("Estrtol: overflow\n");
        return 0;
    }

    return n;
}


_api unsigned long crtl_estrtoul(char *s)
{
    unsigned long n = strtoul(s, NULL, 10);

    if (errno == ERANGE)
    {   
        crtl_print_err("Estrtoul: overflow\n");
        return 0;
    }
    return n;
}



/* Estrtof - convert string s to float {-FLT_MAX:FLT_MAX} */
_api float crtl_estrtof(char *s)
{
    float x = strtod(s, NULL);

    if ( (x > FLT_MAX) || (x < -FLT_MAX) || (errno == ERANGE) )
    {   
        crtl_print_err("Estrtof: overflow\n");
        return 0;
    }
    return (float) x;
}


/* Estrtod - convert string s to double {-DBL_MAX:DBL_MAX} */
_api double crtl_estrtod(char *s)
{
    double x = strtod(s, NULL);

    /* errno == ERANGE suffices if compiler sets errno on overflow */
    if ( (errno == ERANGE) || (x > DBL_MAX) || (x < -DBL_MAX) )
    {   
        crtl_print_err("Estrtod: overflow\n");
        return 0;
    }
    return x;
}



_api int crtl_strtobool(const char *s, bool *res)
{
	if (!s)
		return -EINVAL;

	switch (s[0]) {
	case 'y':
	case 'Y':
	case '1':
		*res = true;
		return 0;
	case 'n':
	case 'N':
	case '0':
		*res = false;
		return 0;
	case 'o':
	case 'O':
		switch (s[1]) {
		case 'n':
		case 'N':
			*res = true;
			return 0;
		case 'f':
		case 'F':
			*res = false;
			return 0;
		default:
			break;
		}
	default:
		break;
	}

	return -EINVAL;
}


/**
 * Create byte array from hexadecimal string. 
 * @param str Pointer to the string.
 * @param buf Pointer to a buffer where the data should be stored. 
 * @param buflen Length for the data buffer.
 * @return -1 if string contained illegal charactes of if the buffer did not
 * contain enough room for data.
 */
_api int crtl_strtobyte(const char *str, uint8_t *buf, int buflen)
{
        int len = strlen(str), blen,hi;
        const char *p;
        uint8_t *b;

        blen = len / 2;
        if (len % 2 != 0)
                blen++;

        if (buflen < blen)
                return -1;

        memset(buf,0,buflen);

        p = str; 
        b = buf;
        hi = 1;
        if (len %2 != 0) {
                /* Odd number of characters on the string, fill with zero.*/
                hi = 0;
        }

        while( *p != '\0') {
                if ( *p >= '0' && *p <= '9')
                        *b |= *p - '0';
                else if (*p >= 'a' && *p <= 'f')
                        *b |= *p - 'a' +10;
                else if ( *p >= 'A' && *p <= 'F')
                        *b |= *p - 'A' + 10;
                else
                        return -1;

                if (hi) {
                        *b = *b << 4;
                        hi = 0;
                } else {
                        b++;
                        hi = 1;
                }

                p++;
        }
        return 0;
}


_api char *crtl_strjoint(char *dst, const char *fmt, ...)
{
    if(dst == NULL || fmt == NULL)
        return NULL;

    va_list args;
    va_start(args, fmt);

    vsprintf(dst, fmt, args);

    va_end(args);

    return (char*)dst;
}

_api char *crtl_strnjoint(char *dst, size_t size, const char *fmt, ...)
{
    if(dst == NULL || fmt == NULL)
        return NULL;

    va_list args;
    va_start(args, fmt);

    vsnprintf(dst, size, fmt, args);

    va_end(args);

    return (char*)dst;
}



_api int crtl_memshow(const void*in, int len)
{
    if(unlikely(!in))
        return -1;
    char _unused const *c = in;
    int i;
    for(i=0;i<len;i++, c++)
        printf("%02x ", (unsigned char)*c);
    printf("\n");
    return len;
}



_api void crtl_memswap(unsigned char *x, unsigned char *y, size_t size)
{
    while(size-- > 0) {
        unsigned char tmp = *x;
        *x++ = *y;
        *y++ = tmp;
    }
}

_api void crtl_memcopy(void *x, void *y, size_t size)
{
    while(size-- > 0) {
        unsigned char *pbTo = (unsigned char *)x;
        unsigned char *pbFrom = (unsigned char *)y;
        *pbTo++ = *pbFrom++;
    }
}

/**
 * strncasecmp - Case insensitive, length-limited string comparison
 * @s1: One string
 * @s2: The other string
 * @len: the maximum number of characters to compare
 */
_api int crtl_strncasecmp(const char *s1, const char *s2, size_t len)
{
	/* Yes, Virginia, it had better be unsigned */
	unsigned char c1, c2;

	if (!len)
		return 0;

	do {
		c1 = *s1++;
		c2 = *s2++;
		if (!c1 || !c2)
			break;
		if (c1 == c2)
			continue;
		c1 = crtl_tolower(c1);
		c2 = crtl_tolower(c2);
		if (c1 != c2)
			break;
	} while (--len);
	return (int)c1 - (int)c2;
}

_api int crtl_strcasecmp(const char *s1, const char *s2)
{
	int c1, c2;

	do {
		c1 = crtl_tolower(*s1++);
		c2 = crtl_tolower(*s2++);
	} while (c1 == c2 && c1 != 0);
	return c1 - c2;
}


#ifndef SUN_A

/* eatoh - convert string s to short integer {SHRT_MIN:SHRT_MAX} */
_api short crtl_eatoh(char *s)
{
	long n = strtol(s, NULL, 10);
	
	if ( (n > SHRT_MAX) || (n < SHRT_MIN) || (errno == ERANGE) )
		crtl_print_err("eatoh: overflow\n");

	return (short) n;
}


/* eatou - convert string s to unsigned short integer {0:USHRT_MAX} */
_api unsigned short crtl_eatou(char *s)
{
	unsigned long n = strtoul(s, NULL, 10);

	if ( (n > USHRT_MAX) || (errno == ERANGE) )
		crtl_print_err("eatou: overflow\n");

	return (unsigned short) n;
}


/* eatoi - convert string s to integer {INT_MIN:INT_MAX} */
_api int crtl_eatoi(char *s)
{
	long n = strtol(s, NULL, 10);

	if ( (n > INT_MAX) || (n < INT_MIN) || (errno == ERANGE) )
		crtl_print_err("eatoi: overflow\n");

	return (int) n;
}


/* eatop - convert string s to unsigned integer {0:UINT_MAX} */
_api unsigned int crtl_eatop(char *s)
{
	unsigned long n = strtoul(s, NULL, 10);

	if ( (n > UINT_MAX) || (errno == ERANGE) )
		crtl_print_err("eatop: overflow\n");

	return (unsigned int) n;
}


/* eatol - convert string s to long integer {LONG_MIN:LONG_MAX} */
_api long crtl_eatol(char *s)
{
	long n = strtol(s, NULL, 10);

	if (errno == ERANGE)
		crtl_print_err("eatol: overflow\n");

	return n;
}


/* eatov - convert string s to unsigned long {0:ULONG_MAX} */
_api unsigned long crtl_eatov(char *s)
{
	unsigned long n = strtoul(s, NULL, 10);

	if (errno == ERANGE)
		crtl_print_err("eatov: overflow\n");

	return n;
}


/* eatof - convert string s to float {-FLT_MAX:FLT_MAX} */
_api float crtl_eatof(char *s)
{
	float x = strtod(s, NULL);

	if ( (x > FLT_MAX) || (x < -FLT_MAX) || (errno == ERANGE) )
		crtl_print_err("eatof: overflow\n");

	return (float) x;
}


/* eatod - convert string s to double {-DBL_MAX:DBL_MAX} */
_api double crtl_eatod(char *s)
{
	double x = strtod(s, NULL);

	/* errno == ERANGE suffices if compiler sets errno on overflow */
	if ( (errno == ERANGE) || (x > DBL_MAX) || (x < -DBL_MAX) )
		crtl_print_err("eatod: overflow\n");

	return x;
}

#else   /* if SUN_A is defined */
/* old one resurrected because SUN 2 didn't have strtoul */

/* atopkge - convert ascii to arithmetic and with error checking
 *
 * eatoh	- ascii to short
 * eatou	- ascii to unsigned short
 * eatoi	- ascii to int
 * eatop	- ascii to unsigned
 * eatol	- ascii to long
 * eatov	- ascii to unsigned long
 * eatof	- ascii to float (dummy sub)
 * eatod	- ascii to double (dummy sub)
 *
 * Returns:
 *	eatoh: short int
 *	eatou: unsigned short int
 *	eatoi: int
 *	eatop: unsigned int
 *	eatol: long int
 *	eatov: unsigned long int
 *	eatof: float
 *	eatod: double
 *
 * Synopsis:
 *	short eatoh(s)
 *	char s[];
 *
 *	unsigned short eatou(s)
 *	char s[];
 *
 *	int eatoi(s)
 *	char s[];
 *
 *	unsigned eatop(s)
 *	char s[];
 *
 *	long eatol(s)
 *	char s[];
 *
 *	unsigned long eatov(s)
 *	char s[];
 *
 *	float eatof(s)
 *	char s[];
 *
 *	double eatod(s)
 *	char s[];
 *
 * Notes:
 *	I haven't a clue as to how to write eatof and eatod, but when
 *	vendors come up to snuff on the ANSI C prescribed error returns
 *	for strtod, it'll be a piece of cake.  And when strtol, strtoul
 *	are likewise properly implemented, the remaining routines in this
 *	package will simplify materially.  For now, eatof and eatod are
 *	just place holders that don't really check for errors.
 *
 *	The overflow error check on a type that fills an unsigned long
 *	is different and a bit slower than the others.  Still, it might
 *      be better to use it in eatou and eatop as well and avoid the
 *	(possible) additional function call.
 *
 *	The code relies on the fact that converting unsigned to signed
 *	has no surprises for numbers in the lower half of the range.
 *
 *	Size limits on the integer data types are machine dependent and
 *      are read from the file limits.h.
 *
 * Credits:
 *	Plum: Reliable Data Structures in C, p. 2-17.
 *	Kernighan and Ritchie: The C Programming Language, p. 58.
 *	CWP: Jack, Brian
 *
 *
 */

/* eatoh - convert string s to short integer {SHRT_MIN:SHRT_MAX}    *
 * We store the absolute value of the converted string in an        *
 * unsigned long so we can test it for overflow.                    */
_api short crtl_eatoh(char *s)
{
	unsigned long n;
	int i;
	short sign = 1;
	long eatol();

	for (i = 0; isspace(s[i]); ++i) ;	/* skip white space */

	if (s[i] == '+' || s[i] == '-') {
		sign = (s[i++] == '+') ? 1 : -1;
	}

	for (n = 0; isdigit(s[i]) && n <= SHRT_MAX/10; ++i) {
		n = 10 * n + (s[i] - '0');
	}

	if ((sign ==  1) && (n > SHRT_MAX) ||
	    (sign == -1) && (n > SHRT_MIN) || isdigit(s[i]))
		crtl_print_err("eatoh: overflow\n");

	return  sign * (short) n;
}


/* eatou - convert string s to unsigned short integer {0:USHRT_MAX} *
 * If USHRT_MAX < ULONG_MAX, we can temporarily fit the converted   *
 * number in an unsigned long with room to check for overflow       *
 * condition.  If not, we forward the string to the unsigned long   *
 * routine.                                                         */
_api unsigned short crtl_eatou(char *s)
{
	unsigned long n;
	int i;
	unsigned long eatov();

	if (USHRT_MAX == ULONG_MAX)  return (unsigned short) eatov(s);

	for (i = 0; isspace(s[i]); ++i) ;  /* skip white space */

	if (s[i] == '-')
		crtl_print_err("eatou: saw negative number\n");

	for (n = 0; isdigit(s[i]) && n <= USHRT_MAX/10; ++i) {
		n = 10 * n + (s[i] - '0');
	}
	if (n > USHRT_MAX || isdigit(s[i]))
		crtl_print_err("eatou: overflow\n");

	return (unsigned short) n;
}


/* eatoi - convert string s to short integer {INT_MIN:INT_MAX}    *
 * The logic is the same as for eatou with INT_MAX replacing      *
 * SHRT_MAX and INT_MIN replacing SHRT_MIN.                       */
_api int crtl_eatoi(char *s)
{
	unsigned long n;
	int i;
	int sign = 1;
	long eatol();

	if (INT_MAX == LONG_MAX) return (int) eatol(s);

	for (i = 0; isspace(s[i]); ++i) ;	/* skip white space */

	if (s[i] == '+' || s[i] == '-') {
		sign = (s[i++] == '+') ? 1 : -1;
	}

	for (n = 0; isdigit(s[i]) && n <= INT_MAX/10; ++i) {
		n = 10 * n + (s[i] - '0');
	}

	if ((sign ==  1) && (n > INT_MAX) ||
	    (sign == -1) && (n > INT_MIN) || isdigit(s[i]))
		crtl_print_err("eatoi: overflow\n");

	return  sign * (int) n;
}


/* eatop - convert string s to unsigned integer {0:UINT_MAX}        *
 * The logic is the same as for eatou with UINT_MAX replacing       *
 * USHRT_MAX.                                                       */
_api unsigned int crtl_eatop(char *s)
{
	unsigned long n;
	int i;
	unsigned long eatov();

	if (UINT_MAX == ULONG_MAX) return((unsigned int) eatov(s));

	for (i = 0; isspace(s[i]); ++i) ;  /* skip white space */

	if (s[i] == '-')
		crtl_print_err("eatop: saw negative number\n");

	for (n = 0; isdigit(s[i]) && n <= UINT_MAX/10; ++i) {
		n = 10 * n + (s[i] - '0');
	}
	if (n > UINT_MAX || isdigit(s[i]))
		crtl_print_err("eatop: overflow\n");

	return (unsigned int) n;
}


/* eatol - convert string s to long integer {LONG_MIN:LONG_MAX}     *
 * We store the absolute value of the converted string in an        *
 * unsigned long so we can test it for overflow.                    */
_api long crtl_eatol(char *s)
{
	unsigned long n;
	int i;
	int sign = 1L;

	for (i = 0; isspace(s[i]); ++i) ;	/* skip white space */

	if (s[i] == '+' || s[i] == '-') {
		sign = (s[i++] == '+') ? 1L : -1L;
	}

	for (n = 0L; isdigit(s[i]) && n <= LONG_MAX/10L; ++i) {
		n = 10L * n + (s[i] - '0');
	}

	if ((sign ==  1L) && (n > LONG_MAX)   ||
	    (sign == -1L) && (n > LONG_MIN) || isdigit(s[i]))
		crtl_print_err("eatol: overflow\n");

	return  sign * (long) n;
}


/* eatov - convert string s to unsigned long {0:ULONG_MAX}          *
 * Here, we check for overflow by seeing whether n decreases.       */
_api unsigned long crtl_eatov(char *s)
{
	unsigned long n;
	unsigned long n_old;
	int i;

	for (i = 0; isspace(s[i]); ++i) ;  /* skip white space */

	if (s[i] == '-')
		crtl_print_err("eatov: saw negative number\n");

	for (n_old = 0L, n = 0L; isdigit(s[i]); ++i) {
		n = 10L * n + (s[i] - '0');
		if (n < n_old)
			crtl_print_err("eatov: overflow\n");
		n_old = n;
	}

	return n;
}

/* Dummy atof, atod routines until the ANSI police get here */
_api float crtl_eatof(char *s)
{
	return (float) atof(s);
}


_api double crtl_eatod(char *s)
{
	return atof(s);
}

#endif /* SUN_A */





/**
 * glob_match - Shell-style pattern matching, like !fnmatch(pat, str, 0)
 * @pat: Shell-style pattern to match, e.g. "*.[ch]".
 * @str: String to match.  The pattern must match the entire string.
 *
 * Perform shell-style glob matching, returning true (1) if the match
 * succeeds, or false (0) if it fails.  Equivalent to !fnmatch(@pat, @str, 0).
 *
 * Pattern metacharacters are ?, *, [ and \.
 * (And, inside character classes, !, - and ].)
 *
 * This is small and simple implementation intended for device blacklists
 * where a string is matched against a number of patterns.  Thus, it
 * does not preprocess the patterns.  It is non-recursive, and run-time
 * is at most quadratic: strlen(@str)*strlen(@pat).
 *
 * An example of the worst case is glob_match("*aaaaa", "aaaaaaaaaa");
 * it takes 6 passes over the pattern before matching the string.
 *
 * Like !fnmatch(@pat, @str, 0) and unlike the shell, this does NOT
 * treat / or leading . specially; it isn't actually used for pathnames.
 *
 * Note that according to glob(7) (and unlike bash), character classes
 * are complemented by a leading !; this does not support the regex-style
 * [^a-z] syntax.
 *
 * An opening bracket without a matching close is matched literally.
 */
_api bool crtl_glob_match(char const *pat, char const *str)
{
	/*
	 * Backtrack to previous * on mismatch and retry starting one
	 * character later in the string.  Because * matches all characters
	 * (no exception for /), it can be easily proved that there's
	 * never a need to backtrack multiple levels.
	 */
	char const *back_pat = NULL, *back_str = back_str;

	/*
	 * Loop over each token (character or class) in pat, matching
	 * it against the remaining unmatched tail of str.  Return false
	 * on mismatch, or true after matching the trailing nul bytes.
	 */
	for (;;) {
		unsigned char c = *str++;
		unsigned char d = *pat++;

		switch (d) {
		case '?':	/* Wildcard: anything but nul */
			if (c == '\0')
				return false;
			break;
		case '*':	/* Any-length wildcard */
			if (*pat == '\0')	/* Optimize trailing * case */
				return true;
			back_pat = pat;
			back_str = --str;	/* Allow zero-length match */
			break;
		case '[': {	/* Character class */
			bool match = false, inverted = (*pat == '!');
			char const *class = pat + inverted;
			unsigned char a = *class++;

			/*
			 * Iterate over each span in the character class.
			 * A span is either a single character a, or a
			 * range a-b.  The first span may begin with ']'.
			 */
			do {
				unsigned char b = a;

				if (a == '\0')	/* Malformed */
					goto literal;

				if (class[0] == '-' && class[1] != ']') {
					b = class[1];

					if (b == '\0')
						goto literal;

					class += 2;
					/* Any special action if a > b? */
				}
				match |= (a <= c && c <= b);
			} while ((a = *class++) != ']');

			if (match == inverted)
				goto backtrack;
			pat = class;
			}
			break;
		case '\\':
			d = *pat++;
			/*FALLTHROUGH*/
		default:	/* Literal character */
literal:
			if (c == d) {
				if (d == '\0')
					return true;
				break;
			}
backtrack:
			if (c == '\0' || !back_pat)
				return false;	/* No point continuing */
			/* Try again from last *, one character later in str. */
			pat = back_pat;
			str = ++back_str;
			break;
		}
	}
}

