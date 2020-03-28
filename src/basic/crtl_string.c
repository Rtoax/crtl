

#include "crtl/crtl_string.h"




/* string api */
_api int crtl_strstripc(char *str, const char ch)
{    
    if((void*)str == NULL)    
    {        
        return -1;    
    }    

    if(strlen(str) <= 1)
    {
        return -1; 
    }

    if(!crtl_ischar(ch))
    {
        crtl_print_err("Can not strip CHAR from %s\n", (char*)str);
        return -1; 
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
    return 1;
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



_api int crtl_memshow(const void*in, int len)
{
    if(unlikely(!in))
        return -1;
    char _unused const *c = in;
    int i;
    for(i=0;i<len;i++, c++)
        printf("%02x ", *c);
    printf("\n");
    return len;
}


#ifndef SUN_A

/* eatoh - convert string s to short integer {SHRT_MIN:SHRT_MAX} */
short crtl_eatoh(char *s)
{
	long n = strtol(s, NULL, 10);
	
	if ( (n > SHRT_MAX) || (n < SHRT_MIN) || (errno == ERANGE) )
		crtl_print_err("eatoh: overflow\n");

	return (short) n;
}


/* eatou - convert string s to unsigned short integer {0:USHRT_MAX} */
unsigned short crtl_eatou(char *s)
{
	unsigned long n = strtoul(s, NULL, 10);

	if ( (n > USHRT_MAX) || (errno == ERANGE) )
		crtl_print_err("eatou: overflow\n");

	return (unsigned short) n;
}


/* eatoi - convert string s to integer {INT_MIN:INT_MAX} */
int crtl_eatoi(char *s)
{
	long n = strtol(s, NULL, 10);

	if ( (n > INT_MAX) || (n < INT_MIN) || (errno == ERANGE) )
		crtl_print_err("eatoi: overflow\n");

	return (int) n;
}


/* eatop - convert string s to unsigned integer {0:UINT_MAX} */
unsigned int crtl_eatop(char *s)
{
	unsigned long n = strtoul(s, NULL, 10);

	if ( (n > UINT_MAX) || (errno == ERANGE) )
		crtl_print_err("eatop: overflow\n");

	return (unsigned int) n;
}


/* eatol - convert string s to long integer {LONG_MIN:LONG_MAX} */
long crtl_eatol(char *s)
{
	long n = strtol(s, NULL, 10);

	if (errno == ERANGE)
		crtl_print_err("eatol: overflow\n");

	return n;
}


/* eatov - convert string s to unsigned long {0:ULONG_MAX} */
unsigned long crtl_eatov(char *s)
{
	unsigned long n = strtoul(s, NULL, 10);

	if (errno == ERANGE)
		crtl_print_err("eatov: overflow\n");

	return n;
}


/* eatof - convert string s to float {-FLT_MAX:FLT_MAX} */
float crtl_eatof(char *s)
{
	float x = strtod(s, NULL);

	if ( (x > FLT_MAX) || (x < -FLT_MAX) || (errno == ERANGE) )
		crtl_print_err("eatof: overflow\n");

	return (float) x;
}


/* eatod - convert string s to double {-DBL_MAX:DBL_MAX} */
double crtl_eatod(char *s)
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
short crtl_eatoh(char *s)
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
unsigned short crtl_eatou(char *s)
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
int crtl_eatoi(char *s)
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
unsigned int crtl_eatop(char *s)
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
long crtl_eatol(char *s)
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
unsigned long crtl_eatov(char *s)
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
float crtl_eatof(char *s)
{
	return (float) atof(s);
}


double crtl_eatod(char *s)
{
	return atof(s);
}

#endif /* SUN_A */

