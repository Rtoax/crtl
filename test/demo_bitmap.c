
#include "crtl/bits/crtl_bitmap.h"
#include <crtl/crtl_string.h>
#include <crtl/crtl_log.h>

static void demo_bitmap_test1()
{
    int nbits= 256;
    int byte=8;
    unsigned long * bm = bitmap_alloc(nbits);
    unsigned long * bm2 = bitmap_alloc(nbits);
    unsigned long * bm3 = bitmap_alloc(nbits);
    unsigned long * bm4 = bitmap_alloc(nbits);
    
    bitmap_zero(bm, nbits);
    crtl_memshow(bm, nbits/byte);

    bitmap_fill(bm, 128);

    bitmap_set(bm, 2, 200);
    bitmap_set(bm2, 150, 30);

    
    crtl_memshow(bm, nbits/byte);
    crtl_memshow(bm2, nbits/byte);

    bitmap_xor(bm3, bm2, bm, nbits);
    crtl_memshow(bm3, nbits/byte);

    bitmap_or(bm4, bm2, bm, nbits);
    crtl_memshow(bm4, nbits/byte);

    bitmap_complement(bm2, bm, nbits);
    crtl_memshow(bm2, nbits/byte);

    bitmap_clear(bm2, 200, 30);
    crtl_memshow(bm2, nbits/byte);

    
    crtl_print_warning("__bitmap_and\n");
    unsigned long * bm5 = bitmap_alloc(nbits);
    __bitmap_and(bm5, bm2, bm, nbits);
    crtl_memshow(bm2, nbits/byte);
    crtl_memshow(bm, nbits/byte);
    crtl_memshow(bm5, nbits/byte);

    crtl_print_warning("__bitmap_or\n");
    unsigned long * bm6 = bitmap_alloc(nbits);
    __bitmap_or(bm6, bm2, bm, nbits);
    crtl_memshow(bm2, nbits/byte);
    crtl_memshow(bm, nbits/byte);
    crtl_memshow(bm6, nbits/byte);
    
    crtl_print_warning("__bitmap_xor\n");
    __bitmap_xor(bm6, bm3, bm, nbits);
    crtl_memshow(bm3, nbits/byte);
    crtl_memshow(bm, nbits/byte);
    crtl_memshow(bm6, nbits/byte);

    
    crtl_print_warning("__bitmap_andnot\n");
    __bitmap_andnot(bm6, bm2, bm, nbits);
    crtl_memshow(bm2, nbits/byte);
    crtl_memshow(bm, nbits/byte);
    crtl_memshow(bm6, nbits/byte);

    
    
    bitmap_free(bm);
}


static void demo_bitmap_test2_equal()
{
    int nbits= 256;
    unsigned long * bm1 = bitmap_alloc(nbits);
    unsigned long * bm2 = bitmap_alloc(nbits);

    bitmap_set(bm1, 0, 200);
    bitmap_set(bm2, 0, 200);

    if(__bitmap_equal(bm1, bm2, 200)) {
        crtl_print_info("__bitmap_equal equal.\n");
    } else {
        crtl_print_info("__bitmap_equal not equal.\n");
    
}


        

    
    unsigned long * bm3 = bitmap_alloc(nbits);
    unsigned long * bm4 = bitmap_alloc(nbits);
    unsigned long * bm5 = bitmap_alloc(nbits);
    
    bitmap_set(bm3, 0, 100);
    bitmap_set(bm4, 100, 200);
    bitmap_set(bm5, 0, 200);

    if(__bitmap_or_equal(bm1, bm2,bm5, 200)) {
        crtl_print_info("__bitmap_or_equal equal.\n");
    } else {
        crtl_print_info("__bitmap_or_equal not equal.\n");
    
}


    
    unsigned long * bm6 = bitmap_alloc(nbits);
    unsigned long * bm7 = bitmap_alloc(nbits);

    bitmap_set(bm6, 0, 100);
    bitmap_set(bm7, 99, 200);
    if(__bitmap_intersects(bm6, bm7, 200)) {
        crtl_print_info("__bitmap_intersects ok.\n");
    } else {
        crtl_print_info("__bitmap_intersects not ok.\n");
    
}

    crtl_memshow(bm6, nbits/8);
    crtl_memshow(bm7, nbits/8);

    
    crtl_print_warning("__bitmap_subset\n");
    unsigned long * bm9 = bitmap_alloc(nbits);
    unsigned long * bm10 = bitmap_alloc(nbits);
    bitmap_set(bm9, 3, 200);
    bitmap_set(bm10, 3, 100);
    crtl_memshow(bm9, nbits/8);
    crtl_memshow(bm10, nbits/8);
    if(__bitmap_subset(bm10, bm9, nbits)) {
        crtl_print_info("__bitmap_subset ok.\n");
    } else {
        crtl_print_info("__bitmap_subset not ok.\n");
    
}

    
    int w = __bitmap_weight(bm9, nbits);
    crtl_print_info("__bitmap_weight %d.\n", w);
    crtl_memshow(bm9, nbits/8);


    
}

static void demo_bitmap_test3_complement()
{
    int nbits= 1024;
    unsigned long * bm1 = bitmap_alloc(nbits);
    unsigned long * bm2 = bitmap_alloc(nbits);

    
    bitmap_set(bm1, 0, 200);
    
    __bitmap_complement(bm2, bm1, nbits);
    
    crtl_memshow(bm1, nbits/32);
    crtl_memshow(bm2, nbits/32);
    
    if(__bitmap_equal(bm1, bm2, 200)) {
        crtl_print_info("__bitmap_equal equal.\n");
    } else {
        crtl_print_info("__bitmap_equal not equal.\n");
    
}
    
}

    
static void demo_bitmap_test_shift()
{
    int nbits= 256;
    unsigned long * bm1 = bitmap_alloc(nbits);
    unsigned long * bm2 = bitmap_alloc(nbits);

    
    bitmap_set(bm1, 0, 200);
    crtl_memshow(bm1, nbits/8);
    
    __bitmap_shift_right(bm2, bm1, 50, nbits);
    crtl_memshow(bm2, nbits/8);
    
    __bitmap_shift_left(bm2, bm1, 50, nbits);
    crtl_memshow(bm2, nbits/8);
    
}
static void demo_bitmap_test_cut()
{
    int nbits= 256;
    unsigned long * bm1 = bitmap_alloc(nbits);
    unsigned long * bm2 = bitmap_alloc(nbits);
    
    bitmap_set(bm1, 0, 200);
    bitmap_cut(bm2, bm1, 10, 40, nbits);
    
    crtl_memshow(bm1, nbits/8);
    crtl_memshow(bm2, nbits/8);
    
}

static void demo_bitmap_test_replace()
{
    int nbits= 256;
    unsigned long * bm1 = bitmap_alloc(nbits);
    unsigned long * bm2 = bitmap_alloc(nbits);
    unsigned long * bm3 = bitmap_alloc(nbits);
    unsigned long * bm4 = bitmap_alloc(nbits);
    
    bitmap_set(bm1, 0, 100);
    bitmap_set(bm2, 90, 100);
    bitmap_set(bm3, 10, 200);

    __bitmap_replace(bm4, bm1, bm2, bm3, nbits);

    //old   ff ff ff ff ff ff ff ff ff ff ff ff 0f 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
    //new   00 00 00 00 00 00 00 00 00 00 00 fc ff ff ff ff ff ff ff ff ff ff ff 3f 00 00 00 00 00 00 00 00 
    //mask  00 fc ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 03 00 00 00 00 00 
    //dst   ff 03 00 00 00 00 00 00 00 00 00 fc ff ff ff ff ff ff ff ff ff ff ff 3f 00 00 00 00 00 00 00 00

    crtl_memshow(bm1, nbits/8);
    crtl_memshow(bm2, nbits/8);
    crtl_memshow(bm3, nbits/8);
    crtl_memshow(bm4, nbits/8);
    
}

static void demo_bitmap_test_parse()
{
    int nbits= 256;
    unsigned long * bm1 = bitmap_alloc(nbits);
    bitmap_set(bm1, 0, 100);

    //01 00 00 00 01 00 00 00 01 00 00 00 01 00 00 00 01 00 00 00 01 00 00 00 ff ff ff ff 01 00 00 00 
    //demo_bitmap.c:240 demo_bitmap_test_parse: Info: 1,ffffffff,1,1,1,1,1,1,
    char buf[1024] = {"1,ffffffff,1,1,1,1,1,1,"};
    bitmap_parse(buf, strlen(buf), bm1, nbits);
    crtl_memshow(bm1, nbits/8);
    crtl_print_info("%s\n", buf);

    unsigned long * bm2 = bitmap_alloc(nbits);
    bitmap_set(bm2, 0, 1);
    crtl_memshow(bm2, nbits/8);
    char buf2[256] = {"0-1023:2/256"};
    if(bitmap_parselist(buf2, bm2, nbits)) {
        crtl_print_info("bitmap_parselist ok, %s\n", buf2);
    } else {
        crtl_print_info("bitmap_parselist not ok, %s\n", buf2);
    }

    
}




int main()
{
//    demo_bitmap_test1();
//    demo_bitmap_test2_equal();
//    demo_bitmap_test3_complement();
//    demo_bitmap_test_shift();
//    demo_bitmap_test_cut();
//    demo_bitmap_test_replace();
    demo_bitmap_test_parse();



    return 0;
}
