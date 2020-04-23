#ifndef __CRTL_BITS_SM_H
#define __CRTL_BITS_SM_H 1


#include "crtl/bits/crtl_types_fn.h"
#include "crtl/bits/crtl_list.h"
#include "crtl/bits/crtl_tree_rbtree.h"


typedef int (*crtl_sm_handler_fn_t)(const unsigned long curr_state, const signed long event, const unsigned long next_state, ...);

/* 状态机事件处理结构体 */
struct __crtl_sm_evthandler {
    
    signed long wait_event; /* 等待发生的事件 */
    
    crtl_sm_handler_fn_t event_handler; /* 事件处理函数 */

    unsigned long next_state;   /* 下一个状态 */

    struct rb_node node_of_handler;
};

/* 状态机节点结构体 */
struct crtl_sm_node {

    unsigned long curr_state;   /* 当前状态 */
    
    struct rb_node node_of_state;
    
    struct rb_root handlers;  /* struct __crtl_sm_evthandler */
};



#endif /*<__CRTL_BITS_SM_H>*/
