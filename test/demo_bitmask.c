
#include "crtl/bits/crtl_bitmask.h"
#include <crtl/crtl_string.h>
#include <crtl/crtl_log.h>

static void demo_bitmask_test1()
{
    unsigned int nbits = 256;
    
    struct crtl_bitmask *mask1 = crtl_bitmask_alloc(nbits);

    crtl_memshow(mask1->maskp, nbits/8);

    crtl_bitmask_setbit(mask1, 2);
    crtl_memshow(mask1->maskp, nbits/8);
    
    crtl_bitmask_setall(mask1);
    crtl_memshow(mask1->maskp, nbits/8);

    crtl_bitmask_clearall(mask1);
    crtl_memshow(mask1->maskp, nbits/8);
    if(crtl_bitmask_isallclear(mask1)) {
        crtl_print_info("all clear.\n");
    }

    crtl_bitmask_setbit(mask1, 2);
    if(crtl_bitmask_isbitset(mask1, 2)) {
        crtl_print_info(" bit set.\n");
    }
    if(crtl_bitmask_isbitset(mask1, 3)) {
        crtl_print_info(" bit set.\n");
    }

    crtl_bitmask_parselist("0-255:3", mask1);

    char buf[1024] = {0};
    crtl_bitmask_displaylist(buf, 1024, mask1);
    crtl_print_info("%s\n", buf);
    
    crtl_bitmask_free(mask1);
}


int main()
{
    demo_bitmask_test1();
    

    return 0;
}

