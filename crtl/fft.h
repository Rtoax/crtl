#ifndef __RT_MATH_PFAFFT_CWP_H
#define __RT_MATH_PFAFFT_CWP_H 1

#include <math.h>


#include "crtl/bits/complex.h"

/* Prime Factor FFTs */
int crtl_ncrtl_pfa (int nmin);
int crtl_ncrtl_pfao (int nmin, int nmax);
int crtl_ncrtl_pfar (int nmin);
int crtl_ncrtl_pfaro (int nmin, int nmax);
void crtl_pfacc (int isign, int n, crtl_fcomplex z[]);
void crtl_pfarc (int isign, int n, float rz[], crtl_fcomplex cz[]);
void crtl_pfacr (int isign, int n, crtl_fcomplex cz[], float rz[]);
void crtl_pfa2cc (int isign, int idim, int n1, int n2, crtl_fcomplex z[]);
void crtl_pfa2rc (int isign, int idim, int n1, int n2, float rz[], crtl_fcomplex cz[]);
void crtl_pfa2cr (int isign, int idim, int n1, int n2, crtl_fcomplex cz[], float rz[]);
void crtl_pfamcc (int isign, int n, int nt, int k, int kt, crtl_fcomplex z[]);

/* Prime Factor FFTs (double version)*/
int crtl_ncrtl_pfa_d (int nmin);
int crtl_ncrtl_pfao_d (int nmin, int nmax);
int crtl_ncrtl_pfar_d (int nmin);
int crtl_ncrtl_pfaro_d (int nmin, int nmax);
void crtl_pfacc_d (int isign, int n, crtl_dcomplex z[]);
void crtl_pfacr_d (int isign, int n, crtl_dcomplex cz[], double rz[]);
void crtl_pfarc_d (int isign, int n, double rz[], crtl_dcomplex cz[]);
void crtl_pfamcc_d (int isign, int n, int nt, int k, int kt, crtl_dcomplex z[]);
void crtl_pfa2cc_d (int isign, int idim, int n1, int n2, crtl_dcomplex z[]);
void crtl_pfa2cr_d (int isign, int idim, int n1, int n2, crtl_dcomplex cz[], double rz[]);
void crtl_pfa2rc_d (int isign, int idim, int n1, int n2, double rz[], crtl_dcomplex cz[]);





#endif /*<__RT_MATH_PFAFFT_CWP_H>*/


