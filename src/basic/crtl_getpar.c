#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>

#include <fcntl.h>      /* non-ANSI */
#include <unistd.h>     /* non-ANSI */
#include <sys/types.h>  /* non-ANSI */

#include "crtl/easy/attribute.h"
#include "crtl/bits/types_basic.h"
#include "crtl/file.h"
#include "crtl/log.h"
#include "crtl/alloc.h"
#include "crtl/string.h"
#include "crtl/bits/ctype.h"

struct param_key_value;

struct __crtl_getpar_struct {
    int xargc; 
    char **xargv;
    struct param_key_value *params_tab;
    int nargs;		/* number of args that parse	*/
    
    size_t args_count;		/* total number of args		*/
    char **arg_per_string;		/* pointer to arg strings	*/
    char *args_string;		/* storage for command line	*/

    char Delimiter;

};


/* parameter table */
struct param_key_value{
	char *name;		/* external name of parameter	*/
	char *asciival;		/* ascii value of parameter	*/
};


#define PFNAME "par="
#define crtl_getpar__XARGV_START_INDEX    1
#define PAR_NAMES_MAX 512



/* functions declared and used internally */
static int __crtl_getpar_index(const struct __crtl_getpar_struct *args_id, int n, char *name);
static void __crtl_getpar_init(struct __crtl_getpar_struct *args_id);
static void __crtl_getpar__install_symbol_table(struct __crtl_getpar_struct *args_id);
static char *__crtl_getpar_fname(struct __crtl_getpar_struct *args_id);
static int __crtl_getpar_npar(struct __crtl_getpar_struct *args_id, int n, char *name, char *type, void *ptr);


/* make command line args available to subroutines -- re-entrant version */
struct __crtl_getpar_struct * crtl_getpar_init(int argc, char **argv, char Delimiter, char *fromfilepar)
{
    struct __crtl_getpar_struct *args_id = crtl_malloc1(1, sizeof(struct __crtl_getpar_struct));
    
    args_id->xargc = argc; 
    args_id->xargv = argv;

    args_id->Delimiter = Delimiter;
    
    __crtl_getpar_init(args_id);

	return args_id;
}


int crtl_getpar_free(struct __crtl_getpar_struct *args_id)
{
//    __crtl_dbg("---\n");
	crtl_mfree1(args_id->args_string);
//    __crtl_dbg("---\n");
	crtl_mfree1(args_id->arg_per_string);
//    __crtl_dbg("---\n");
	crtl_mfree1(args_id->params_tab);
//    __crtl_dbg("---\n");
    crtl_mfree1(args_id);
    
    return CRTL_SUCCESS;
}


/* functions to get values for the last occurrence of a parameter name */
int crtl_getpar_int(struct __crtl_getpar_struct *args_id, char *name, int *ptr)
{
	return __crtl_getpar_npar(args_id, 0,name,"i",ptr);
}
int crtl_getpar_uint(struct __crtl_getpar_struct *args_id, char *name, unsigned int *ptr)
{
	return __crtl_getpar_npar(args_id, 0,name,"p",ptr);
}
int crtl_getpar_short(struct __crtl_getpar_struct *args_id, char *name, short *ptr)
{
	return __crtl_getpar_npar(args_id, 0,name,"h",ptr);
}
int crtl_getpar_ushort(struct __crtl_getpar_struct *args_id, char *name, unsigned short *ptr)
{
	return __crtl_getpar_npar(args_id, 0,name,"u",ptr);
}
int crtl_getpar_long(struct __crtl_getpar_struct *args_id, char *name, long *ptr)
{
	return __crtl_getpar_npar(args_id, 0,name,"l",ptr);
}
int crtl_getpar_ulong(struct __crtl_getpar_struct *args_id, char *name, unsigned long *ptr)
{
	return __crtl_getpar_npar(args_id, 0,name,"v",ptr);
}
int crtl_getpar_float(struct __crtl_getpar_struct *args_id, char *name, float *ptr)
{
	return __crtl_getpar_npar(args_id, 0,name,"f",ptr);
}
int crtl_getpar_double(struct __crtl_getpar_struct *args_id, char *name, double *ptr)
{
	return __crtl_getpar_npar(args_id, 0,name,"d",ptr);
}
int crtl_getpar_string(struct __crtl_getpar_struct *args_id, char *name, char **ptr)
{
	return __crtl_getpar_npar(args_id, 0,name,"s",ptr);
}
int crtl_getpar_stringarray(struct __crtl_getpar_struct *args_id, char *name, char **ptr)
{
	return __crtl_getpar_npar(args_id, 0,name,"a",ptr);
}


/* functions to get values for the n'th occurrence of a parameter name */
int crtl_getnpar_int(struct __crtl_getpar_struct *args_id, int n, char *name, int *ptr)
{
	return __crtl_getpar_npar(args_id, n,name,"i",ptr);
}
int crtl_getnpar_uint(struct __crtl_getpar_struct *args_id, int n, char *name, unsigned int *ptr)
{
	return __crtl_getpar_npar(args_id, n,name,"p",ptr);
}
int crtl_getnpar_short(struct __crtl_getpar_struct *args_id, int n, char *name, short *ptr)
{
	return __crtl_getpar_npar(args_id, n,name,"h",ptr);
}
int crtl_getnpar_ushort(struct __crtl_getpar_struct *args_id, int n, char *name, unsigned short *ptr)
{
	return __crtl_getpar_npar(args_id, n,name,"u",ptr);
}
int crtl_getnpar_long(struct __crtl_getpar_struct *args_id, int n, char *name, long *ptr)
{
	return __crtl_getpar_npar(args_id, n,name,"l",ptr);
}
int crtl_getnpar_ulong(struct __crtl_getpar_struct *args_id, int n, char *name, unsigned long *ptr)
{
	return __crtl_getpar_npar(args_id, n,name,"v",ptr);
}
int crtl_getnpar_float(struct __crtl_getpar_struct *args_id, int n, char *name, float *ptr)
{
	return __crtl_getpar_npar(args_id, n,name,"f",ptr);
}
int crtl_getnpar_double(struct __crtl_getpar_struct *args_id, int n, char *name, double *ptr)
{
	return __crtl_getpar_npar(args_id, n,name,"d",ptr);
}
int crtl_getnpar_string(struct __crtl_getpar_struct *args_id, int n, char *name, char **ptr)
{
	return __crtl_getpar_npar(args_id, n,name,"s",ptr);
}
int crtl_getnpar_stringarray(struct __crtl_getpar_struct *args_id, int n, char *name, char **ptr)
{
	return __crtl_getpar_npar(args_id, n,name,"a",ptr);
}
static int __crtl_getpar_npar(struct __crtl_getpar_struct *args_id, int n, char *name, char *type, void *ptr)
{
	int i;			/* index of name in symbol table	*/
	int nval;		/* number of parameter values found	*/
	char *aval;		/* ascii field of symbol		*/
    
	if (args_id->xargc == crtl_getpar__XARGV_START_INDEX) 
        return 0;
    
	i = __crtl_getpar_index(args_id, n,name);/* Get parameter index */
	if (i < 0) 
        return 0;   /* Not there */

	if (0 == ptr) {
	   crtl_print_err("__crtl_getpar_npar called with 0 pointer, type = %s", type);
	}
    
	/*
	 * handle string type as a special case, since a string
	 * may contain commas.
	 */
	if (type[0]=='s') {
		*((char**)ptr) = args_id->params_tab[i].asciival;
		return 1;
	}

	/* convert vector of ascii values to numeric values */
	for (nval=0,aval=args_id->params_tab[i].asciival; *aval; nval++) {
		switch (type[0]) {
			case 'i':
				*(int*)ptr = crtl_eatoi(aval);
				ptr = (int*)ptr+1;
				break;
			case 'p':
				*(unsigned int*)ptr = crtl_eatop(aval);
				ptr = (unsigned int*)ptr+1;
				break;
			case 'h':
				*(short*)ptr = crtl_eatoh(aval);
				ptr = (short*)ptr+1;
				break;
			case 'u':
				*(unsigned short*)ptr = crtl_eatou(aval);
				ptr = (unsigned short*)ptr+1;
				break;
			case 'l':
				*(long*)ptr = crtl_eatol(aval);
				ptr = (long*)ptr+1;
				break;
			case 'v':
				*(unsigned long*)ptr = crtl_eatov(aval);
				ptr = (unsigned long*)ptr+1;
				break;
			case 'f':
				*(float*)ptr = crtl_eatof(aval);
				ptr = (float*)ptr+1;
				break;
			case 'd':
				*(double*)ptr = crtl_eatod(aval);
				ptr = (double*)ptr+1;
				break;
			case 'a':
				{ char *tmpstr="";
				   tmpstr = crtl_malloc1(strlen(aval)+1,1);

				   crtl_strchop(aval,tmpstr);
				   *(char**)ptr = tmpstr;
				   ptr=(char **)ptr + 1;
				}
				   break;
			default:
				crtl_print_err("invalid parameter type = %s\n",	 type);
		}
		while (*aval++ != ',') {
			if (!*aval) break;
		}
	}
	return nval;
}



/* return number of occurrences of parameter name */
int crtl_getpar_countparname(struct __crtl_getpar_struct *args_id, char *name)
{
	int i,nname;

	if (args_id->xargc == crtl_getpar__XARGV_START_INDEX) return 0;
	for (i=0,nname=0; i<args_id->nargs; ++i)
		if (!strcmp(name,args_id->params_tab[i].name)) ++nname;
	return nname;
}

/* return number of values in n'th occurrence of parameter name */
int crtl_getpar_countnparval(const struct __crtl_getpar_struct *args_id, int n, char *name)
{
	int i;

	if (args_id->xargc == crtl_getpar__XARGV_START_INDEX) return 0;
	i = __crtl_getpar_index(args_id, n,name);
	if (i>=0)
		return crtl_strccount(',',args_id->params_tab[i].asciival) + 1;
	else
		return 0;
}

/* return number of values in last occurrence of parameter name */
int crtl_getpar_countparval(const struct __crtl_getpar_struct *args_id, char *name)
{
	return crtl_getpar_countnparval(args_id, 0, name);
}



/*
 * Return the index of the n'th occurrence of a parameter name,
 * except if n==0, return the index of the last occurrence.
 * Return -1 if the specified occurrence does not exist.
 */
static int __crtl_getpar_index(const struct __crtl_getpar_struct *args_id, int n, char *name)
{
	int i;
	if (n==0) {
		for (i=args_id->nargs-1; i>=0; --i)
			if (!strcmp(name,args_id->params_tab[i].name)) break;
		return i;
	} else {
		for (i=0; i<args_id->nargs; ++i)
			if (!strcmp(name,args_id->params_tab[i].name))
				if (--n==0) break;
		if (i<args_id->nargs)
			return i;
		else
			return -1;
	}
}

/* Initialize crtl_getpar_ */
static void __crtl_getpar_init(struct __crtl_getpar_struct *args_id)
{
	static char *pfname;	/* name of parameter file		*/
	FILE *pffd=NULL;	/* file id of parameter file		*/
	size_t pflen;		/* length of parameter file in bytes	*/
	int parfile;		/* parfile existence flag		*/
	int argstrlen=0;
	char *pargstr;		/* storage for parameter file args	*/
	size_t nread=0;		/* bytes fread				*/
	int i, j;		/* counters				*/
	int start = crtl_true;
	int quote = crtl_false;
        
	/* Check if xargc was initiated */

	if(!args_id->xargc)
		crtl_print_err("xargc=%d -- not initiated in main\n", args_id->xargc);

	/* Space needed for command lines */

	for (i = crtl_getpar__XARGV_START_INDEX, argstrlen = 0; i < args_id->xargc; i++) {
		argstrlen += strlen(args_id->xargv[i]) + 1;
	}

	/* Get parfile name if there is one */

	if ((pfname = __crtl_getpar_fname(args_id))) {
		parfile = crtl_true;
	} else {
		parfile = crtl_false;
	}

	if (parfile) {
	 	pffd = crtl_efopen(pfname, "r");

		/* Get the length */
		crtl_efseek(pffd, 0, SEEK_END);

		pflen = (off_t) crtl_eftello(pffd);

		rewind(pffd);
		argstrlen += pflen;
	} else {
		pflen = 0;
	}

	args_id->args_string = (char *) crtl_malloc1(argstrlen+1, 1);
	args_id->arg_per_string = (char **) crtl_malloc1((argstrlen+1)/4,sizeof(char*));

	if (parfile) {
		/* Read the parfile */
		nread = crtl_efread(args_id->args_string, 1, pflen, pffd);
  		if (nread != pflen) {
  	 	    crtl_print_err("fread only %d bytes out of %d from %s\n",
  					        nread, pflen, pfname);
		}
		crtl_efclose(pffd);
	}
	/* force input to valid 7 bit ASCII */
	for( i=0; i<nread; i++ ){
	    args_id->args_string[i] &= 0x7F;
	}
	/* tokenize the input */
	j = 0;
	for( i=0; i<nread; i++ ){
	    /* look for start of token */
	    if( start ){
    		if( crtl_isgraph( (int)args_id->args_string[i] ) ){
    		    args_id->arg_per_string[j] = &(args_id->args_string[i]);
    		    start = !start;
    		    j++;
    		}else{
    		    args_id->args_string[i] = 0;
    		}
	    }else if( !quote && crtl_isspace( (int)args_id->args_string[i] ) ){
    		args_id->args_string[i] = 0;
    		start = !start;
	    }
	    /* toggle quote semaphore */
	    if( args_id->args_string[i] == '\'' || args_id->args_string[i] == '\"' ){
		    quote = !quote;
	    }

	}

	i=0;
	args_id->args_count=0;
	while( i < j && args_id->arg_per_string[i] != 0 ){
	    if( strchr( args_id->arg_per_string[i] , args_id->Delimiter) ){
    		args_id->arg_per_string[args_id->args_count] = args_id->arg_per_string[i];
    		args_id->args_count++;
	    }
	    i++;
	}

	/* Copy command line arguments */

	for (j = crtl_getpar__XARGV_START_INDEX, pargstr = args_id->args_string + pflen + 1; j < args_id->xargc; j++) {
		strcpy(pargstr,args_id->xargv[j]);
		args_id->arg_per_string[args_id->args_count++] = pargstr;
		pargstr += strlen(args_id->xargv[j]) + 1;
	}

	/* Allocate space for the pointer table */
	args_id->params_tab = (struct param_key_value*) crtl_malloc1(args_id->args_count, sizeof(struct param_key_value));

	/* Tabulate arg_per_string */
	__crtl_getpar__install_symbol_table(args_id);

	return;
}


/* Get name of parameter file */
static char *__crtl_getpar_fname (struct __crtl_getpar_struct *args_id)
{
	int i;
	size_t pfnamelen;
    
	pfnamelen = strlen(PFNAME);
	for (i = args_id->xargc-1 ; i >= crtl_getpar__XARGV_START_INDEX ; i--) {
		if(!strncmp(PFNAME, args_id->xargv[i], pfnamelen)
		    && strlen(args_id->xargv[i]) != pfnamelen) {
			return args_id->xargv[i] + pfnamelen;
		}
	}
	return NULL;
}



/* Install symbol table  args_id->args_count, args_id->arg_per_string*/
static void __crtl_getpar__install_symbol_table(struct __crtl_getpar_struct *args_id)
{
	int i;
	char *eqptr;
    
	for (i = 0, args_id->nargs = 0 ; i < args_id->args_count; i++) {
		eqptr = strchr(args_id->arg_per_string[i], args_id->Delimiter);
		if (eqptr) {
			args_id->params_tab[args_id->nargs].name = args_id->arg_per_string[i];
			args_id->params_tab[args_id->nargs].asciival = eqptr + 1;
			*eqptr = (char)0;
			args_id->nargs++;
		}
	}
	return;
}





#ifdef TEST____
#define N 100
main(int argc, char **argv)
{
	char *s;
	short h, vh[N];
	unsigned short u, vu[N];
	long l, vl[N];
	unsigned long v, vv[N];
	int i, vi[N], ipar, npar, nval;
	unsigned int p, vp[N];
	float f, vf[N];
	double d, vd[N];

	crtl_getpar_init(argc, argv);

	/* int parameters */
	npar = crtl_getpar_countparname("i");
	printf("\nnumber of i pars = %d\n",npar);
	for (ipar=1; ipar<=npar; ++ipar) {
		crtl_getnpar_int(ipar,"i",&i);
		printf("occurrence %d of i=%d\n",ipar,i);
	}
	if (crtl_getpar_int("i", &i))
		printf("last occurrence of i=%d\n",i);
	npar = crtl_getpar_countparname("vi");
	printf("number of vi pars = %d\n",npar);
	for (ipar=1; ipar<=npar; ++ipar) {
		nval = crtl_getpar_countnparval(ipar,"vi");
		printf("occurrence %d has %d values\n",ipar,nval);
		nval = crtl_getnpar_int(ipar,"vi",vi);
		printf("vi=");
		for (i=0; i<nval; i++)
			printf("%d%c",vi[i],i==nval-1?'\n':',');
	}
	if (npar>0) {
		nval = crtl_getpar_countparval("vi");
		printf("last occurrence has %d values\n",nval);
		crtl_getpar_int("vi",vi);
		printf("vi=");
		for (i=0; i<nval; i++)
			printf("%d%c",vi[i],i==nval-1?'\n':',');
	}

	/* float parameters */
	npar = crtl_getpar_countparname("f");
	printf("\nnumber of f pars = %d\n",npar);
	for (ipar=1; ipar<=npar; ++ipar) {
		crtl_getnpar_float(ipar,"f",&f);
		printf("occurrence %d of f=%g\n",ipar,f);
	}
	if (crtl_getpar_float("f", &f))
		printf("last occurrence of f=%g\n",f);
	npar = crtl_getpar_countparname("vf");
	printf("number of vf pars = %d\n",npar);
	for (ipar=1; ipar<=npar; ++ipar) {
		nval = crtl_getpar_countnparval(ipar,"vf");
		printf("occurrence %d has %d values\n",ipar,nval);
		nval = crtl_getnpar_float(ipar,"vf",vf);
		printf("vf=");
		for (i=0; i<nval; i++)
			printf("%g%c",vf[i],i==nval-1?'\n':',');
	}
	if (npar>0) {
		nval = crtl_getpar_countparval("vf");
		printf("last occurrence has %d values\n",nval);
		crtl_getpar_float("vf",vf);
		printf("vf=");
		for (i=0; i<nval; i++)
			printf("%g%c",vf[i],i==nval-1?'\n':',');
	}

	/* string parameters */
	npar = crtl_getpar_countparname("s");
	printf("\nnumber of s pars = %d\n",npar);
	for (ipar=1; ipar<=npar; ++ipar) {
		crtl_getnpar_string(ipar,"s",&s);
		printf("occurrence %d of s=%s\n",ipar,s);
	}
	if (crtl_getpar_string("s", &s))
		printf("last occurrence of s=%s\n",s);

	return EXIT_SUCCESS;
}
#endif

