#include <crtl/mask.h>
#include <crtl/log.h>



int main()
{
    int i=0;
    for(i=0;i<32;i++)
        crtl_print_debug("mask 0x%x\n", crtl_mask_32bit(i));

    
    crtl_print_debug("mask 0x%x\n", crtl_mask_32bit_all);

    

    return 0;
}

