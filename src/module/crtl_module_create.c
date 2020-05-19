
#include "crypto/module/types.h"


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

    /* 初始化模块内容修改锁 */
    __CRTL_MODULE_RWLOCK_INIT(__module);
    
    /* 消息队列名称 */
    sprintf(__module->module_msgq_name, "%s", __CRTL_MSGQ_NAME_GEN(app_name, module_name));

    /* 模块名 */
    sprintf(__module->module_name, "%s", module_name);
    
    __crtl_dbg("MSGQ: %s, maxmsg %d, msgsize %d\n", __module->module_msgq_name, mq_maxmsg, mq_msgsize);

    /* 创建模块的消息队列 */
    __module->main_msgqd = crtl_mq_open_blk(__module->module_msgq_name, mq_maxmsg, mq_msgsize);

    /* 创建模块的线程 */
    __module->main_task_fn = main_task;
    __module->main_task_fn_arg = main_task_arg;


    /* 将模块添加至应用的模块数中 */
    if(__crtl_application_add_module(app_name, __module) != CRTL_SUCCESS) {
        crtl_print_err("add module %s to app %s error.\n", module_name, app_name);
        crtl_mfree1(__module);
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    return CRTL_SUCCESS;
}

                         
/* 执行这个模块 */
int crtl_module_start(const char *app_name, const char *module_name)
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

    
    return CRTL_SUCCESS;
}
                         
/* 停止这个模块 */
int crtl_module_stop(const char *app_name, const char *module_name)
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
    __CRTL_MODULE_RWLOCK_DESTROY(__module);
    crtl_mfree1(__module);
    
    return CRTL_SUCCESS;
}

