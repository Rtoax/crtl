#include <crtl/log.h>
#include <crtl/math.h>
#include <crtl/time.h>
#include <crypto/asm/byteswap.h>

void demo_asm_byteswap_test1()
{
    int i= 0x12345678;
    crtl_print_alert("crtl_asm_swab32 %x=%x\n", i, crtl_asm_swab32(i));


    int l= 0x12345678;
    crtl_print_alert("crtl_asm_swab64 %lx=%lx\n", l, crtl_asm_swab64(l));
    
}

int main()
{
    demo_asm_byteswap_test1();
    
    
    
    return 0;
}





