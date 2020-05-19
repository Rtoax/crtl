#include <crtl/types.h>
#include <crtl/log.h>

void demo1_bits_set()
{
    crtl_bits_set set;

    CRTL_BITS_ZERO(&set);
    CRTL_BITS_SET(1, &set);
    CRTL_BITS_SET(2, &set);
    crtl_memprint(stdout, &set, sizeof(crtl_bits_set));
    CRTL_BITS_SET(10, &set);
    crtl_memprint(stdout, &set, sizeof(crtl_bits_set));
}

int main()
{
    demo1_bits_set();
    
    
    
    return 0;
}
