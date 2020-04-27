
#include "crtl/bits/crtl_bitmap.h"
#include <crtl/crtl_string.h>


int main()
{
    int nbits= 1024;
    unsigned long * bm = bitmap_alloc(nbits);
    unsigned long * bm2 = bitmap_alloc(nbits);
    unsigned long * bm3 = bitmap_alloc(nbits);
    unsigned long * bm4 = bitmap_alloc(nbits);
    
    bitmap_zero(bm, nbits);
    crtl_memshow(bm, nbits/32);

    bitmap_fill(bm, 128);

    bitmap_set(bm, 2, 200);
    bitmap_set(bm2, 150, 30);

    
    crtl_memshow(bm, nbits/32);
    crtl_memshow(bm2, nbits/32);

    bitmap_xor(bm3, bm2, bm, nbits);
    crtl_memshow(bm3, nbits/32);

    bitmap_or(bm4, bm2, bm, nbits);
    crtl_memshow(bm4, nbits/32);

    bitmap_complement(bm2, bm, nbits);
    crtl_memshow(bm2, nbits/32);

    bitmap_clear(bm2, 200, 30);
    crtl_memshow(bm2, nbits/32);
    
    bitmap_free(bm);

    return 0;
}
