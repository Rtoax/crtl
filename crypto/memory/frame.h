#ifndef __CRTL_CRYPTO_MEMORY_FRAME_H
#define __CRTL_CRYPTO_MEMORY_FRAME_H 1

//#include "crypto/memory/block.h"
#include <crtl/bitmap.h>


struct mem_frame_struct {
    union {
        void *head;
        void *addr;
    };
    void *tail;

    unsigned long size;

//    bitsize             nbits           size
//    1(char)             10(64)          64
//    1(char)             20(64)          64
//    1(char)             65(128)         128
//    2(short)            10(64)          128
//    
//    bitmap:     0000 0001 1100 ...

    
    unsigned long *bitmap;  //标记内存空间使用情况
    unsigned long bitsize;  //一个bit标记的内存大小
    
    struct list_head list;
};







#endif /*<__CRTL_CRYPTO_MEMORY_FRAME_H>*/
