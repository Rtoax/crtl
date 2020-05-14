#include "crtl/bits/crtl_getpar.h"
#include "crtl/crtl_log.h"



void demo_crtl_getpar__test(int argc, char *argv[])
{
    crtl_getpar_t argsid = crtl_getpar_init(argc, argv, '=', NULL);

    int i, npar, ipar, nval, vi[12];
    float f;
    double d;
    char *s;
    
    crtl_getpar_int(argsid, "int", &i);
    crtl_getpar_float(argsid, "float", &f);
    crtl_getpar_double(argsid, "double", &d);
    crtl_getpar_string(argsid, "string", &s);

    npar = crtl_getpar_countparname(argsid, "rt");
    for (ipar=1; ipar<=npar; ++ipar) {
        nval = crtl_getpar_countnparval(argsid, ipar,"rt");
		crtl_getnpar_int(argsid, ipar,"rt",vi);
		crtl_print_debug("occurrence nval %d, ipar %d of i=%d\n",nval, ipar,vi[0]);
        int ii;
        for(ii=0;ii<nval;ii++) {
		    crtl_print_debug("occurrence                        %d\n",vi[ii]);
        }
	}
    
    crtl_print_debug("crtl_getpar_: int=%d,  float=%f, double=%lf, string=%s\n",   i,f,d,s);

    crtl_getpar_free(argsid);
}


int main(int argc, char *argv[])
{
    demo_crtl_getpar__test(argc, argv);
    
    return 0;
}
