
#include "crtl/bits/crtl_bitmap.h"
#include <crtl/string.h>
#include <crtl/log.h>

static void demo_bitmap_test1()
{
    int nbits= 256;
    int byte=8;
    unsigned long * bm = crtl_bitmap_alloc(nbits);
    unsigned long * bm2 = crtl_bitmap_alloc(nbits);
    unsigned long * bm3 = crtl_bitmap_alloc(nbits);
    unsigned long * bm4 = crtl_bitmap_alloc(nbits);
    
    crtl_bitmap_zero(bm, nbits);
    crtl_memshow(bm, nbits/byte);

    crtl_bitmap_fill(bm, 128);

    crtl_bitmap_set(bm, 2, 200);
    crtl_bitmap_set(bm2, 150, 30);

    
    crtl_memshow(bm, nbits/byte);
    crtl_memshow(bm2, nbits/byte);

    crtl_bitmap_xor(bm3, bm2, bm, nbits);
    crtl_memshow(bm3, nbits/byte);

    crtl_bitmap_or(bm4, bm2, bm, nbits);
    crtl_memshow(bm4, nbits/byte);

    crtl_bitmap_complement(bm2, bm, nbits);
    crtl_memshow(bm2, nbits/byte);

    crtl_bitmap_clear(bm2, 200, 30);
    crtl_memshow(bm2, nbits/byte);

    
    crtl_print_warning("crtl_bitmap_and\n");
    unsigned long * bm5 = crtl_bitmap_alloc(nbits);
    crtl_bitmap_and(bm5, bm2, bm, nbits);
    crtl_memshow(bm2, nbits/byte);
    crtl_memshow(bm, nbits/byte);
    crtl_memshow(bm5, nbits/byte);

    crtl_print_warning("crtl_bitmap_or\n");
    unsigned long * bm6 = crtl_bitmap_alloc(nbits);
    crtl_bitmap_or(bm6, bm2, bm, nbits);
    crtl_memshow(bm2, nbits/byte);
    crtl_memshow(bm, nbits/byte);
    crtl_memshow(bm6, nbits/byte);
    
    crtl_print_warning("crtl_bitmap_xor\n");
    crtl_bitmap_xor(bm6, bm3, bm, nbits);
    crtl_memshow(bm3, nbits/byte);
    crtl_memshow(bm, nbits/byte);
    crtl_memshow(bm6, nbits/byte);

    
    crtl_print_warning("crtl_bitmap_andnot\n");
    crtl_bitmap_andnot(bm6, bm2, bm, nbits);
    crtl_memshow(bm2, nbits/byte);
    crtl_memshow(bm, nbits/byte);
    crtl_memshow(bm6, nbits/byte);

    
    
    crtl_bitmap_free(bm);
}


static void demo_bitmap_test2_equal()
{
    int nbits= 256;
    unsigned long * bm1 = crtl_bitmap_alloc(nbits);
    unsigned long * bm2 = crtl_bitmap_alloc(nbits);

    crtl_bitmap_set(bm1, 0, 200);
    crtl_bitmap_set(bm2, 0, 200);

    if(crtl_bitmap_equal(bm1, bm2, 200)) {
        crtl_print_info("crtl_bitmap_equal equal.\n");
    } else {
        crtl_print_info("crtl_bitmap_equal not equal.\n");
    
}


        

    
    unsigned long * bm3 = crtl_bitmap_alloc(nbits);
    unsigned long * bm4 = crtl_bitmap_alloc(nbits);
    unsigned long * bm5 = crtl_bitmap_alloc(nbits);
    
    crtl_bitmap_set(bm3, 0, 100);
    crtl_bitmap_set(bm4, 100, 200);
    crtl_bitmap_set(bm5, 0, 200);

    if(crtl_bitmap_or_equal(bm1, bm2,bm5, 200)) {
        crtl_print_info("crtl_bitmap_or_equal equal.\n");
    } else {
        crtl_print_info("crtl_bitmap_or_equal not equal.\n");
    
}


    
    unsigned long * bm6 = crtl_bitmap_alloc(nbits);
    unsigned long * bm7 = crtl_bitmap_alloc(nbits);

    crtl_bitmap_set(bm6, 0, 100);
    crtl_bitmap_set(bm7, 99, 200);
    if(crtl_bitmap_intersects(bm6, bm7, 200)) {
        crtl_print_info("crtl_bitmap_intersects ok.\n");
    } else {
        crtl_print_info("crtl_bitmap_intersects not ok.\n");
    
}

    crtl_memshow(bm6, nbits/8);
    crtl_memshow(bm7, nbits/8);

    
    crtl_print_warning("crtl_bitmap_subset\n");
    unsigned long * bm9 = crtl_bitmap_alloc(nbits);
    unsigned long * bm10 = crtl_bitmap_alloc(nbits);
    crtl_bitmap_set(bm9, 3, 200);
    crtl_bitmap_set(bm10, 3, 100);
    crtl_memshow(bm9, nbits/8);
    crtl_memshow(bm10, nbits/8);
    if(crtl_bitmap_subset(bm10, bm9, nbits)) {
        crtl_print_info("crtl_bitmap_subset ok.\n");
    } else {
        crtl_print_info("crtl_bitmap_subset not ok.\n");
    
}

    
    int w = crtl_bitmap_weight(bm9, nbits);
    crtl_print_info("__bitmap_weight %d.\n", w);
    crtl_memshow(bm9, nbits/8);


    
}

static void demo_bitmap_test3_complement()
{
    int nbits= 1024;
    unsigned long * bm1 = crtl_bitmap_alloc(nbits);
    unsigned long * bm2 = crtl_bitmap_alloc(nbits);

    
    crtl_bitmap_set(bm1, 0, 200);
    
    crtl_bitmap_complement(bm2, bm1, nbits);
    
    crtl_memshow(bm1, nbits/32);
    crtl_memshow(bm2, nbits/32);
    
    if(crtl_bitmap_equal(bm1, bm2, 200)) {
        crtl_print_info("crtl_bitmap_equal equal.\n");
    } else {
        crtl_print_info("crtl_bitmap_equal not equal.\n");
    
}
    
}

    
static void demo_bitmap_test_shift()
{
    int nbits= 256;
    unsigned long * bm1 = crtl_bitmap_alloc(nbits);
    unsigned long * bm2 = crtl_bitmap_alloc(nbits);

    
    crtl_bitmap_set(bm1, 0, 200);
    crtl_memshow(bm1, nbits/8);
    
    crtl_bitmap_shift_right(bm2, bm1, 50, nbits);
    crtl_memshow(bm2, nbits/8);
    
    crtl_bitmap_shift_left(bm2, bm1, 50, nbits);
    crtl_memshow(bm2, nbits/8);
    
}
static void demo_bitmap_test_cut()
{
    int nbits= 256;
    unsigned long * bm1 = crtl_bitmap_alloc(nbits);
    unsigned long * bm2 = crtl_bitmap_alloc(nbits);
    
    crtl_bitmap_set(bm1, 0, 200);
    crtl_bitmap_cut(bm2, bm1, 10, 40, nbits);
    
    crtl_memshow(bm1, nbits/8);
    crtl_memshow(bm2, nbits/8);
    
}

static void demo_bitmap_test_replace()
{
    int nbits= 256;
    unsigned long * bm1 = crtl_bitmap_alloc(nbits);
    unsigned long * bm2 = crtl_bitmap_alloc(nbits);
    unsigned long * bm3 = crtl_bitmap_alloc(nbits);
    unsigned long * bm4 = crtl_bitmap_alloc(nbits);
    
    crtl_bitmap_set(bm1, 0, 100);
    crtl_bitmap_set(bm2, 90, 100);
    crtl_bitmap_set(bm3, 10, 200);

    crtl_bitmap_replace(bm4, bm1, bm2, bm3, nbits);

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
    unsigned long * bm1 = crtl_bitmap_alloc(nbits);
    crtl_bitmap_set(bm1, 0, 100);

    //01 00 00 00 01 00 00 00 01 00 00 00 01 00 00 00 01 00 00 00 01 00 00 00 ff ff ff ff 01 00 00 00 
    //demo_bitmap.c:240 demo_bitmap_test_parse: Info: 1,ffffffff,1,1,1,1,1,1,
    char buf[1024] = {"1,ffffffff,1,1,1,1,1,1,"};
    crtl_bitmap_parse(buf, strlen(buf), bm1, nbits);
    crtl_memshow(bm1, nbits/8);
    crtl_print_info("%s\n", buf);

    unsigned long * bm2 = crtl_bitmap_alloc(nbits);
    crtl_memshow(bm2, nbits/8);
    char buf2[256] = {"0-128:2/12"};
    int ret;
    if((ret = crtl_bitmap_parselist(buf2, bm2, nbits))==0) {
        crtl_print_info("crtl_bitmap_parselist ok, %s, %d\n", buf2, ret);
    } else {
        crtl_print_info("crtl_bitmap_parselist not ok, %s\n", buf2);
    }
    crtl_memshow(bm2, nbits/8);

    
}


static void demo_bitmap_test_map()
{
    int nbits= 32;
    unsigned long * src1 = crtl_bitmap_alloc(nbits);
    unsigned long * dst1 = crtl_bitmap_alloc(nbits);
    unsigned long * old1 = crtl_bitmap_alloc(nbits);
    unsigned long * new1 = crtl_bitmap_alloc(nbits);
    
    crtl_bitmap_set(src1, 1, 1);
    crtl_bitmap_set(src1, 5, 1);
    crtl_bitmap_set(src1, 7, 1);
    crtl_bitmap_set(old1, 4, 3);
    crtl_bitmap_set(new1, 12, 3);

    crtl_print_info("crtl_bitmap_remap\n");
    crtl_bitmap_remap(dst1, src1, old1, new1, nbits);    
    crtl_memshow(dst1, nbits/8);
    crtl_memshow(src1, nbits/8);
    crtl_memshow(old1, nbits/8);
    crtl_memshow(new1, nbits/8);

    crtl_print_info("crtl_bitmap_bitremap\n");
    crtl_bitmap_bitremap(8, old1, new1, 200);
    crtl_memshow(old1, nbits/8);
    crtl_memshow(new1, nbits/8);
    
}


static void demo_bitmap_test_onto()
{
    int nbits= 32;
    unsigned long * src1 = crtl_bitmap_alloc(nbits);
    unsigned long * dst1 = crtl_bitmap_alloc(nbits);

}

static void demo_bitmap_test_from_arr32()
{
    int nbits= 64;
    unsigned long * bitmap = crtl_bitmap_alloc(nbits);

    uint32_t buf[2] = {0x12345678, 0xabcdef};
    
    crtl_bitmap_from_arr32(bitmap, buf, nbits);
    
    crtl_memshow(bitmap, nbits/8);
}


int main()
{
//    demo_bitmap_test1();
//    demo_bitmap_test2_equal();
//    demo_bitmap_test3_complement();
//    demo_bitmap_test_shift();
//    demo_bitmap_test_cut();
//    demo_bitmap_test_replace();
//    demo_bitmap_test_parse();
//    demo_bitmap_test_map();
    demo_bitmap_test_from_arr32();

    return 0;
}

