#ifndef __CRTL_BITS_crtl_getpar__H
#define __CRTL_BITS_crtl_getpar__H 1


#ifdef __cplusplus  /* if C++, specify external C linkage */
extern "C" {
#endif

typedef void* crtl_getpar_t;


/**
 * Create argument getpar handle
 *
 * @param argc: argument number
 * @param argv: arguments
 * @param Delimiter: Delimiter of argument, if is '=', key=value will be used
 * @param fromfilepar: not use
 * @return getpar handle
 */
crtl_getpar_t crtl_getpar_init(int argc, char **argv, char Delimiter, char *fromfilepar);

/**
 * free argument getpar handle
 *
 * @param args_id: getpar handle
 * @return error number
 */
int crtl_getpar_free(crtl_getpar_t args_id);

int crtl_getpar_int(crtl_getpar_t args_id, char *name, int *p);
int crtl_getpar_uint(crtl_getpar_t args_id, char *name, unsigned int *p);
int crtl_getpar_short(crtl_getpar_t args_id, char *name, short *p);
int crtl_getpar_ushort(crtl_getpar_t args_id, char *name, unsigned short *p);
int crtl_getpar_long(crtl_getpar_t args_id, char *name, long *p);
int crtl_getpar_ulong(crtl_getpar_t args_id, char *name, unsigned long *p);
int crtl_getpar_float(crtl_getpar_t args_id, char *name, float *p);
int crtl_getpar_double(crtl_getpar_t args_id, char *name, double *p);
int crtl_getpar_string(crtl_getpar_t args_id, char *name, char **p);
int crtl_getpar_stringarray(crtl_getpar_t args_id, char *name, char **p);
int crtl_getnpar_int(crtl_getpar_t args_id, int n, char *name, int *p);
int crtl_getnpar_uint(crtl_getpar_t args_id, int n, char *name, unsigned int *p);
int crtl_getnpar_short(crtl_getpar_t args_id, int n, char *name, short *p);
int crtl_getnpar_ushort(crtl_getpar_t args_id, int n, char *name, unsigned short *p);
int crtl_getnpar_long(crtl_getpar_t args_id, int n, char *name, long *p);
int crtl_getnpar_ulong(crtl_getpar_t args_id, int n, char *name, unsigned long *p);
int crtl_getnpar_float(crtl_getpar_t args_id, int n, char *name, float *p);
int crtl_getnpar_double(crtl_getpar_t args_id, int n, char *name, double *p);
int crtl_getnpar_string(crtl_getpar_t args_id, int n, char *name, char **p);
int crtl_getnpar_stringarray(crtl_getpar_t args_id, int n, char *name, char **p);
int crtl_getpar_countparname(crtl_getpar_t args_id, char *name);
int crtl_getpar_countparval(const crtl_getpar_t args_id, char *name);
int crtl_getpar_countnparval(const crtl_getpar_t args_id, int n, char *name);




#ifdef __cplusplus  /* if C++ (external C linkage is being specified) */
}
#endif

#endif /* __CRTL_BITS_crtl_getpar__H */


