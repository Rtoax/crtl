#include "crtl/bits/crtl_getpar.h"
#include "crtl/crtl_log.h"



void demo_getpar_test(int argc, char *argv[])
{
    crtl_par_t argsid = getpar_init(argc, argv, '=', NULL);

    int i, npar, ipar, nval;
    float f;
    double d;
    char *s;
    
    getparint(argsid, "int", &i);
    getparfloat(argsid, "float", &f);
    getpardouble(argsid, "double", &d);
    getparstring(argsid, "string", &s);

    npar = countparname(argsid, "rt");
    for (ipar=1; ipar<=npar; ++ipar) {
        nval = countnparval(argsid, ipar,"rt");
		getnparint(argsid, ipar,"rt",&i);
		crtl_print_debug("occurrence nval %d, ipar %d of i=%d\n",nval, ipar,i);
	}
    
    crtl_print_debug("getpar: int=%d,  float=%f, double=%lf, string=%s\n",   i,f,d,s);

    getpar_free(argsid);
}


int main(int argc, char *argv[])
{
    demo_getpar_test(argc, argv);
    
    return 0;
}


