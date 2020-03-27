/* Copyright (c) Colorado School of Mines, 2011.*/
/* All rights reserved.                       */

/* par.h - include file for getpar, selfdoc, and error handling functions */

#ifndef __CRTL_BITS_GETPAR_H
#define __CRTL_BITS_GETPAR_H 1


/* INCLUDES */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>

#include <fcntl.h>      /* non-ANSI */
#include <unistd.h>     /* non-ANSI */
#include <sys/types.h>  /* non-ANSI */

/* GLOBAL DECLARATIONS */

#ifdef __cplusplus  /* if C++, specify external C linkage */
extern "C" {
#endif

struct __crtl_getpar_struct;

typedef struct __crtl_getpar_struct * crtl_par_t;

/* getpar parameter parsing */
struct __crtl_getpar_struct* getpar_init(int argc, char **argv, char Delimiter, char *fromfilepar);
int getpar_free(struct __crtl_getpar_struct *args_id);

int getparint(struct __crtl_getpar_struct *args_id, char *name, int *p);
int getparuint(struct __crtl_getpar_struct *args_id, char *name, unsigned int *p);
int getparshort(struct __crtl_getpar_struct *args_id, char *name, short *p);
int getparushort(struct __crtl_getpar_struct *args_id, char *name, unsigned short *p);
int getparlong(struct __crtl_getpar_struct *args_id, char *name, long *p);
int getparulong(struct __crtl_getpar_struct *args_id, char *name, unsigned long *p);
int getparfloat(struct __crtl_getpar_struct *args_id, char *name, float *p);
int getpardouble(struct __crtl_getpar_struct *args_id, char *name, double *p);
int getparstring(struct __crtl_getpar_struct *args_id, char *name, char **p);
int getparstringarray(struct __crtl_getpar_struct *args_id, char *name, char **p);
int getnparint(struct __crtl_getpar_struct *args_id, int n, char *name, int *p);
int getnparuint(struct __crtl_getpar_struct *args_id, int n, char *name, unsigned int *p);
int getnparshort(struct __crtl_getpar_struct *args_id, int n, char *name, short *p);
int getnparushort(struct __crtl_getpar_struct *args_id, int n, char *name, unsigned short *p);
int getnparlong(struct __crtl_getpar_struct *args_id, int n, char *name, long *p);
int getnparulong(struct __crtl_getpar_struct *args_id, int n, char *name, unsigned long *p);
int getnparfloat(struct __crtl_getpar_struct *args_id, int n, char *name, float *p);
int getnpardouble(struct __crtl_getpar_struct *args_id, int n, char *name, double *p);
int getnparstring(struct __crtl_getpar_struct *args_id, int n, char *name, char **p);
int getnparstringarray(struct __crtl_getpar_struct *args_id, int n, char *name, char **p);
int countparname(struct __crtl_getpar_struct *args_id, char *name);
int countparval(struct __crtl_getpar_struct *args_id, char *name);
int countnparval(struct __crtl_getpar_struct *args_id, int n, char *name);




#ifdef __cplusplus  /* if C++ (external C linkage is being specified) */
}
#endif

#endif /* __CRTL_BITS_GETPAR_H */

