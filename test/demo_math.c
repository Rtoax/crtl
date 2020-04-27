#include <crtl/crtl_log.h>
#include <crtl/crtl_math.h>
#include <crtl/crtl_time.h>
#include <crtl/easy/byteswap.h>

void demo_math_sqrt_test1()
{
    crtl_print_alert("Rong %s\n", "Tao");

    float f = 64.0f;
    crtl_print_alert("crtl_qrsqrt %f=%f\n", f, crtl_qrsqrt(f));

    int i= 0x12345678;
    crtl_print_alert("crtl_asm_swab32 %x=%x\n", i, crtl_asm_swab32(i));

    
    int l= 0x12345678;
    crtl_print_alert("crtl_asm_swab64 %lx=%lx\n", l, crtl_asm_swab64(l));

    
    crtl_print_alert("crtl_asm_rdtsc %ld\n", crtl_asm_rdtsc());
    
}

int main()
{
    demo_math_sqrt_test1();
    
    
    
    return 0;
}


