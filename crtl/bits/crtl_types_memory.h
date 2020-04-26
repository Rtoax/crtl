#ifndef __CRTL_TYPES_MEMORY_H
#define __CRTL_TYPES_MEMORY_H 1

#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"

#include "crtl/bits/crtl_types_basic.h"

#include "crtl/easy/macro.h"

#include "crtl/bits/crtl_list.h"
#include "crtl/bits/crtl_tree_rbtree.h"


/**
 *  内存状态
 */
typedef enum {
    __CRTL_MEMORY_NULL,
    __CRTL_MEMORY_ALLOC,
    __CRTL_MEMORY_FREE,
}__crtl_m_oflag;


/**
 *  内存结构体
 */
struct crtl_memory_struct {
    __crtl_m_oflag m_oflag;
    
    void *m_ptr;
    unsigned int m_size;
};


/**
 *  内存表结构体
 */
struct crtl_memory_list_struct {
    struct crtl_rbtree_struct  *m_rbtree; //node struct crtl_memory_struct
};







#endif //__CRTL_TYPES_MEMORY_H


