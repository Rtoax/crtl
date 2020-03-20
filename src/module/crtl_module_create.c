
#include "crtl/bits/crtl_module_types.h"



/* 创建模块 */
int crtl_module_create(const char *app_name, const char *module_name, long mq_maxmsg, long mq_msgsize,
                         crtl_thread_start_routine_fn main_task, void *main_task_arg)
{
    /* 入参有误 */
    if(unlikely(!app_name) || unlikely(!module_name) || unlikely(!main_task) || unlikely(!mq_maxmsg) || unlikely(!mq_msgsize)) {
        crtl_print_err("wrong params error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    /* 如果app不存在，返回失败 */
    if(CRTL_SUCCESS != crtl_application_exist(app_name)) {
        return CRTL_ERROR;
    }
    
    /* 申请模块内存 */
    struct crtl_module_struct *__module = crtl_malloc1(1, sizeof(struct crtl_module_struct));
    if(!__module) {
        crtl_print_err("crtl_malloc1 error.\n");
        crtl_assert_fp(stderr, __module);
        return CRTL_ERROR;
    }

    memset(__module, 0, sizeof(struct crtl_module_struct));

    crtl_lock_rwattr_t rwlock_attr;
    crtl_rwlockattr_init(&rwlock_attr);
    crtl_rwlockattr_setpshared_shared(&rwlock_attr);
    crtl_rwlock_init(&__module->module_rw_lock, &rwlock_attr);

    sprintf(__module->module_msgq_name, "%s_%s",CRTL_MSGQ_NAME_GEN(app_name), module_name);

    sprintf(__module->module_name, "%s", module_name);
    
    __crtl_dbg("MSGQ: %s, maxmsg %d, msgsize %d\n", __module->module_msgq_name, mq_maxmsg, mq_msgsize);

    __module->main_msgqd = crtl_mq_open2(__module->module_msgq_name, mq_maxmsg, mq_msgsize);

    __module->main_task_fn = main_task;
    __module->main_task_fn_arg = main_task_arg;
        
    __crtl_application_add_module(app_name, __module);

    return CRTL_SUCCESS;
}

                         



/* 销毁模块 */
int crtl_module_destroy(const char *app_name, const char *module_name)
{
    /* 入参有误 */
    if(unlikely(!app_name) || unlikely(!module_name)) {
        crtl_print_err("wrong params error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    /* 如果app不存在，返回失败 */
    if(CRTL_SUCCESS != crtl_application_exist(app_name)) {
        return CRTL_ERROR;
    }
    
    struct crtl_module_struct * _unused __module = __crtl_application_del_module(app_name, module_name);
    if(unlikely(!__module)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    __crtl_dbg("MSGQ Close: %s\n", __module->module_msgq_name);
    
    crtl_mq_close(__module->main_msgqd);
    crtl_mq_unlink(__module->module_msgq_name);
    crtl_rwlock_destroy(&__module->module_rw_lock);
    crtl_mfree1(__module);
    
    return CRTL_SUCCESS;
}

