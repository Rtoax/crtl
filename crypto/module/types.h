#ifndef __CRTL_TYPES_MODULE_H
#define __CRTL_TYPES_MODULE_H 1

#include "crtl/log.h"
#include "crtl/assert.h"
#include "crtl/bits/types_basic.h"
#include "crtl/alloc.h"

#include "crtl/easy/macro.h"

#include "crtl/bits/mask.h"
#include "crtl/tree.h"

#include "crtl/bits/msgq.h"
#include "crtl/bits/rwlock.h"
#include "crtl/bits/thread.h"

#include "crypto/list/list.h"




#define CRTL_MODULE_NAME_LEN    32


typedef     char    crtl_module_name_t[CRTL_MODULE_NAME_LEN];
typedef     char    crtl_app_name_t[CRTL_MODULE_NAME_LEN];




typedef void*           crtl_module_id_t;
typedef unsigned long   crtl_module_msg_type;


/**
 *  模块结构体
 */
struct crtl_module_struct{    
    crtl_module_name_t module_name;//模块名
    crtl_msgq_name_t module_msgq_name;

    
    crtl_lock_rw_t  module_rw_lock; //读写锁
    
    crtl_thread_t   main_task; //main task of this module
    crtl_thread_start_routine_fn main_task_fn;
    void *main_task_fn_arg;
    crtl_mqd_t  main_msgqd;//消息队列
    crtl_boolean module_already_in_app;

    
#define __CRTL_MODULE_RWLOCK_INIT(p_module)   CRTL_LOCK_RWLOCK_SHARE_INIT(&p_module->module_rw_lock)
#define __CRTL_MODULE_RWLOCK_RDLOCK(p_module) CRTL_LOCK_RWLOCK_RDLOCK(&p_module->module_rw_lock)
#define __CRTL_MODULE_RWLOCK_WRLOCK(p_module) CRTL_LOCK_RWLOCK_WRLOCK(&p_module->module_rw_lock)
#define __CRTL_MODULE_RWLOCK_UNLOCK(p_module) CRTL_LOCK_RWLOCK_UNLOCK(&p_module->module_rw_lock)
#define __CRTL_MODULE_RWLOCK_DESTROY(p_module) CRTL_LOCK_RWLOCK_DESTROY(&p_module->module_rw_lock)

    
    struct list_head list; //模块组
};



/* 应用程序结构体 */
struct crtl_application_struct {
    crtl_app_name_t app_name;//应用名
    
    /* 模块信息 */
    volatile long modules_rbtree_init_flag;
    crtl_lock_rw_t  modules_rbtree_rwlock; //读写锁
    crtl_rbtree_t modules_rbtree; //struct crtl_module_struct
#define __CRTL_APP_MODULES_RWLOCK_INIT(p_app)   CRTL_LOCK_RWLOCK_SHARE_INIT(&p_app->modules_rbtree_rwlock)
#define __CRTL_APP_MODULES_RWLOCK_RDLOCK(p_app) CRTL_LOCK_RWLOCK_RDLOCK(&p_app->modules_rbtree_rwlock)
#define __CRTL_APP_MODULES_RWLOCK_WRLOCK(p_app) CRTL_LOCK_RWLOCK_WRLOCK(&p_app->modules_rbtree_rwlock)
#define __CRTL_APP_MODULES_RWLOCK_UNLOCK(p_app) CRTL_LOCK_RWLOCK_UNLOCK(&p_app->modules_rbtree_rwlock)
#define __CRTL_APP_MODULES_RWLOCK_DESTROY(p_app) CRTL_LOCK_RWLOCK_DESTROY(&p_app->modules_rbtree_rwlock)
    
};



_api int crtl_application_exist(const char *app_name);
_hidden int __crtl_application_add_module(const char *app_name, struct crtl_module_struct *const __module);
_hidden struct crtl_module_struct * _unused __crtl_application_del_module(const char *app_name, const char *module_name);


#endif //__CRTL_TYPES_MODULE_H

