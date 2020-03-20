#ifndef __CRTL_TYPES_MODULE_H
#define __CRTL_TYPES_MODULE_H 1

#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"
#include "crtl/crtl_types.h"
#include "crtl/crtl_alloc.h"

#include "crtl/easy/macro.h"

#include "crtl/bits/crtl_mask.h"
#include "crtl/bits/crtl_list.h"
#include "crtl/crtl_tree.h"

#include "crtl/bits/crtl_types_memory.h"
#include "crtl/bits/crtl_msgq_posix.h"
#include "crtl/bits/crtl_lock_rwlock.h"
#include "crtl/bits/crtl_thread.h"


struct crtl_module_struct;




typedef unsigned long crtl_module_id_t;
typedef unsigned long crtl_module_msg_type;

/**
 *  模块间消息头
 */
struct crtl_module_msghdr_struct {
    crtl_module_msg_type    msg_type;

    
    crtl_boolean            is_sync_msg;    //是否为同步消息
    crtl_boolean            is_timer_msg;   //是否为定时消息

    crtl_mqd_t              sync_msg_tmp_mqd;//同步消息临时消息队列
    
    crtl_boolean            has_data; 
    size_t                  msg_size; 
};

/**
 *  模块间消息体
 */
struct crtl_module_msg_sturct {
    struct crtl_module_msghdr_struct msg_hdr;
    char msg_data[0];
};

/**
 *  模块结构体
 */
struct crtl_module_struct{    
#define CRTL_MODULE_NAME_LEN    32
    char module_name[CRTL_MODULE_NAME_LEN];//模块名

    char module_msgq_name[CRTL_MODULE_NAME_LEN*4];

    crtl_lock_rw_t  module_rw_lock; //读写锁
    crtl_thread_t   main_task; //main task of this module
    
    crtl_thread_start_routine_fn main_task_fn;
    void *main_task_fn_arg;
    
    crtl_mqd_t  main_msgqd;//消息队列

    crtl_boolean module_already_in_app;
    
    struct list_head list; //模块组
};


struct __crtl_application_modules_lock {
    struct crtl_module_struct *module;
    crtl_lock_rw_t  module_rwlock; //读写锁
};

/* 应用程序结构体 */
struct crtl_application_struct {
    
    char app_name[CRTL_MODULE_NAME_LEN];//应用名

    crtl_lock_rw_t  app_rw_lock; //读写锁
    
    volatile long modules_rbtree_init_flag;
    crtl_rbtree_t modules_rbtree; //struct __crtl_application_modules_lock
    
};



_api int crtl_application_exist(const char *app_name);
_hidden int __crtl_application_add_module(const char *app_name, struct crtl_module_struct *const __module);
_hidden struct crtl_module_struct * _unused __crtl_application_del_module(const char *app_name, const char *module_name);


#endif //__CRTL_TYPES_MODULE_H

