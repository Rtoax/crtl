#include <crypto/overflow.h>
#include <crtl/log.h>

static void demo_overflow_mul_test1()
{
    size_t a,b,c;
    a = 0xffffffffffff;
    b = 0xffffffffffff;

    crtl_print_info("0x%0lx, %d\n", c, check_mul_overflow(a,b,&c));
}

int main()
{
    demo_overflow_mul_test1();


    return 0;
}
