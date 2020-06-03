#include <crtl/log.h>
#include <crtl/math.h>
#include <crtl/time.h>
#include <crtl/bits/byteswap.h>

void demo_math_sqrt_test1()
{
    crtl_print_alert("Rong %s\n", "Tao");

    float f = 64.0f;
    crtl_print_alert("crtl_qrsqrt %f=%f\n", f, crtl_qrsqrt(f));

    
    crtl_print_alert("crtl_asm_rdtsc %ld\n", crtl_asm_rdtsc());
    
}

int main()
{
    demo_math_sqrt_test1();
    
    
    
    return 0;
}


