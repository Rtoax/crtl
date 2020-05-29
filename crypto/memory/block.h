#ifndef __CRTL_CRYPTO_MEMORY_BLOCK_H
#define __CRTL_CRYPTO_MEMORY_BLOCK_H 1

#include "crypto/list/list.h"

//                                mem_frame_struct
//            +-----------------------------------------------------------------+
//    parent->|             {###}                                 {#########}   |   
//            +-----------------------------------------------------------------+
//            |<--offset--->^   ^                                 ^         ^
//                          |   |
//                          |   |
//                          |   +--------------tail-----------+
//                        head                                | 
//                          |   +------------------------+    |
//                          +-->|          size          |<---+
//                              +------------------------+

struct mem_frame_struct;

struct mem_block_struct {
    union {
        void *head;
        void *addr;
    };
    void *tail;
    
    unsigned long offset;
    unsigned long size;
    
    struct list_head blk_list;
    
    struct mem_frame_struct *parent;
};

//static inline struct mem_block_struct* memblk_create(struct mem_frame_struct *frame)
//{
//    
//}


#endif /*<__CRTL_CRYPTO_MEMORY_BLOCK_H>*/
