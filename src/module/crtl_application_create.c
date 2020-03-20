
#include "crtl/bits/crtl_module_types.h"



/* app列表的读写锁 */
static crtl_lock_rw_t _unused __crtl_apps_tree_rwlock = CRTL_LOCK_RWLOCK_INITIALIZER;
#define __crtl_apps_rdlock crtl_rwlock_rdlock(&__crtl_apps_tree_rwlock, 0,0,0,0)
#define __crtl_apps_wrlock crtl_rwlock_wrlock(&__crtl_apps_tree_rwlock, 0,0,0,0)
#define __crtl_apps_unlock crtl_rwlock_unlock(&__crtl_apps_tree_rwlock)


/* app列表是否初始化标识位 */
static volatile long _unused __crtl_apps_tree_init_flag = 0;

/* app列表 */
static crtl_rbtree_t _unused __crtl_apps_tree = NULL;  //所有应用


/* 模块结构体比较 */
inline static int _unused __crtl_app_modules_cmp(void *data1, void *data2)
{
    struct crtl_module_struct *module1 = (struct crtl_module_struct *)data1;
    struct crtl_module_struct *module2 = (struct crtl_module_struct *)data2;
    
    if(strcmp(module1->module_name, module2->module_name) > 0) return CRTL_GT;
    else if(strcmp(module1->module_name, module2->module_name) == 0) return CRTL_EQ;
    else if(strcmp(module1->module_name, module2->module_name) < 0) return CRTL_LT;
    else return CRTL_EQ;
}

/* app模块锁结构体比较 */
inline static int _unused __crtl_app_app_module_lock_cmp(void *data1, void *data2)
{
    struct __crtl_application_modules_lock *module1 = (struct __crtl_application_modules_lock *)data1;
    struct __crtl_application_modules_lock *module2 = (struct __crtl_application_modules_lock *)data2;
    
    if(strcmp(module1->module->module_name, module2->module->module_name) > 0) return CRTL_GT;
    else if(strcmp(module1->module->module_name, module2->module->module_name) == 0) return CRTL_EQ;
    else if(strcmp(module1->module->module_name, module2->module->module_name) < 0) return CRTL_LT;
    else return CRTL_EQ;
}




/* app结构体比较 */
inline static int _unused __crtl_app_apps_cmp(void *data1, void *data2)
{
    struct crtl_application_struct *app1 = (struct crtl_application_struct *)data1;
    struct crtl_application_struct *app2 = (struct crtl_application_struct *)data2;
    
    if(strcmp(app1->app_name, app2->app_name) > 0) return CRTL_GT;
    else if(strcmp(app1->app_name, app2->app_name) == 0) return CRTL_EQ;
    else if(strcmp(app1->app_name, app2->app_name) < 0) return CRTL_LT;
    else return CRTL_EQ;
}


/* 获取应用 , 线程不安全 */
static struct crtl_application_struct _unused* __crtl_application_getappbyname(const char *app_name)
{
    crtl_rbtree_node_t *rbtree_node = NULL;
    
    struct crtl_application_struct app;
    memset(&app, 0, sizeof(struct crtl_application_struct));
    strcpy(app.app_name, app_name);

    rbtree_node = crtl_rbtree_search(__crtl_apps_tree, &app);
    if(!rbtree_node) {
        return NULL;
    }
    
    return rbtree_node->data;    
}


/* 应用是否存在 , 线程安全 */
_api int crtl_application_exist(const char *app_name)
{
    /* 入参有误 */
    if(unlikely(!app_name)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }

    /* 当保存所有应用信息的标志未被设定 */
    if(CAS(&__crtl_apps_tree_init_flag, 0, 0)) {
        __crtl_dbg("__crtl_apps_tree_init_flag = %ld.\n", __crtl_apps_tree_init_flag);
        
        return CRTL_ERROR;
    }
    
    /* 如果app不存在，返回失败 */
    __crtl_apps_rdlock;
    struct crtl_application_struct _unused*__app = __crtl_application_getappbyname(app_name);
    if(unlikely(!__app)) {
        crtl_print_err("app not exist %s.\n", app_name);
        crtl_assert_fp(stderr, 0);
        __crtl_apps_unlock;
        return CRTL_ERROR;
    }
    __crtl_apps_unlock;
    
    return CRTL_SUCCESS;
}


/* 创建应用 - 线程安全 */
_api int crtl_application_create(const char *app_name)
{
    /* 入参有误 */
    if(unlikely(!app_name)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    /* 当保存所有应用信息的标志被设定，查找app名称是否冲突 */
    if(CAS(&__crtl_apps_tree_init_flag, 1, 1)) {
        __crtl_dbg("__crtl_apps_tree_init_flag = %ld.\n", __crtl_apps_tree_init_flag);

        __crtl_apps_rdlock;
        
        /* 如果app已经存在 */
        if(NULL != __crtl_application_getappbyname(app_name)) {
            crtl_print_err("app already create %s.\n", app_name);
            crtl_assert_fp(stderr, 0);
            __crtl_apps_unlock;
            
            return CRTL_ERROR;
        }
        __crtl_apps_unlock;
    }
    
    /* 当保存所有应用信息的标志未被设定，初始化这个树 */
    if(CAS(&__crtl_apps_tree_init_flag, 0, 1)) {
        __crtl_dbg("__crtl_apps_tree_init_flag = %ld.\n", __crtl_apps_tree_init_flag);
        
        __crtl_apps_wrlock;
        /* 初始化红黑树 */
        __crtl_apps_tree = crtl_rbtree_init(&__crtl_app_apps_cmp, NULL);
        crtl_assert_fp(stderr, __crtl_apps_tree);
        if(!__crtl_apps_tree) {/* 初始化失败 */
            crtl_print_err("crtl_rbtree_init error.\n");
            crtl_assert_fp(stderr, __crtl_apps_tree);
            __crtl_apps_unlock;
            return CRTL_ERROR;
        }
        __crtl_apps_unlock;
    }

    /* 这个应用不存在，创建 */
    /* 申请app内存 */
    
    struct crtl_application_struct *__app = crtl_malloc1(1, sizeof(struct crtl_application_struct));
    if(!__app) {/*  */
        crtl_print_err("crtl_malloc1 error.\n");
        crtl_assert_fp(stderr, __app);
        return CRTL_ERROR;
    }
    memset(__app, 0, sizeof(struct crtl_application_struct));

    /* 将锁属性设为共享 */
    crtl_lock_rwattr_t rwlock_attr;
    crtl_rwlockattr_init(&rwlock_attr);
    crtl_rwlockattr_setpshared_shared(&rwlock_attr);
    crtl_rwlock_init(&__app->app_rw_lock, &rwlock_attr);
    
    /* 初始化属于该应用的模块存放树 */
    if(CAS(&__app->modules_rbtree_init_flag, 0, 1)) {
        __app->modules_rbtree = crtl_rbtree_init(&__crtl_app_app_module_lock_cmp, NULL);
    }
    
    strcpy(__app->app_name, app_name);
    
    __crtl_apps_wrlock;
    
    /* 将app添加至app树中 */
    int ret = crtl_rbtree_insert(__crtl_apps_tree, __app, sizeof(struct crtl_application_struct));
    if(ret != CRTL_SUCCESS) {
        crtl_print_err("insert app %s to apptree %s error.\n", app_name);
        crtl_rwlock_destroy(&__app->app_rw_lock);
        crtl_mfree1(__app);
        crtl_assert_fp(stderr, 0);
        __crtl_apps_unlock;
        return CRTL_ERROR;
    }
    __crtl_apps_unlock;
        
    return CRTL_SUCCESS;
}


/* 在应用中添加模块 - 线程安全 */
_hidden int __crtl_application_add_module(const char *app_name, struct crtl_module_struct *const __module)
{
    if(unlikely(!app_name) || unlikely(!__module)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    /* 如果app不存在，返回失败 */
    if(CRTL_SUCCESS != crtl_application_exist(app_name)) {
        return CRTL_ERROR;
    }
    
    /* 模块已经在APP中 */
    if(__module->module_already_in_app == true) {
        crtl_print_err("module already in app. module %s\n", __module->module_name);
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }

    /* 分配app内存 */
    struct __crtl_application_modules_lock *app_module = crtl_malloc1(1, sizeof(struct __crtl_application_modules_lock));
    if(unlikely(!app_module)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }

    /* 模块已经在APP中 */
    __module->module_already_in_app = true;
    
    app_module->module = __module;
    
    /* 将锁属性设为共享 */
    crtl_lock_rwattr_t rwlock_attr;
    crtl_rwlockattr_init(&rwlock_attr);
    crtl_rwlockattr_setpshared_shared(&rwlock_attr);
    crtl_rwlock_init(&app_module->module_rwlock, &rwlock_attr);

    __crtl_dbg("insert appname %s, modulename %s\n", app_name, __module->module_name);

    __crtl_apps_wrlock;
    
    struct crtl_application_struct _unused* __app = __crtl_application_getappbyname(app_name);
    if(unlikely(!__app)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        __crtl_apps_unlock;
        return CRTL_ERROR;
    }
    crtl_rwlock_wrlock(&__app->app_rw_lock, 0,0,0,0);
    
    int ret = crtl_rbtree_insert(__app->modules_rbtree, app_module, sizeof(struct __crtl_application_modules_lock));
    if(ret != CRTL_SUCCESS) {
        crtl_print_err("insert module %s to app %s error.\n", app_module->module->module_name, app_name);
        crtl_rwlock_destroy(&app_module->module_rwlock);
        crtl_mfree1(app_module);
        crtl_assert_fp(stderr, 0);
        crtl_rwlock_unlock(&__app->app_rw_lock);
        __crtl_apps_unlock;
        return CRTL_ERROR;
    }
    crtl_rwlock_unlock(&__app->app_rw_lock);
    __crtl_apps_unlock;
    
    return CRTL_SUCCESS;
}



/* 删除应用中的模块 , 线程安全 */
_hidden struct crtl_module_struct * _unused __crtl_application_del_module(const char *app_name, const char *module_name)
{
    /* 如果app不存在，返回失败 */
    if(CRTL_SUCCESS != crtl_application_exist(app_name)) {
        crtl_print_err("not exist app %s error.\n", app_name);
        crtl_assert_fp(stderr, 0);
        return NULL;
    }

    /* 组装临时模块结构用于查询 */
    crtl_rbtree_node_t *rbtree_node = NULL;
    struct crtl_module_struct *__module = NULL;
    
    struct __crtl_application_modules_lock __tmp_app_module, *__app_module = NULL;
    struct crtl_module_struct __tmp_module;
    memset(&__tmp_app_module, 0, sizeof(struct __crtl_application_modules_lock));
    memset(&__tmp_module, 0, sizeof(struct crtl_module_struct));
    
    strcpy(__tmp_module.module_name, module_name);

    __tmp_app_module.module = &__tmp_module;

    /* 查询这个APP结构 */
    __crtl_apps_rdlock;
    
    struct crtl_application_struct _unused* __app = __crtl_application_getappbyname(app_name);
    
    crtl_rwlock_wrlock(&__app->app_rw_lock, 0,0,0,0);
    
    rbtree_node = crtl_rbtree_search(__app->modules_rbtree, &__tmp_app_module);
    if(!rbtree_node) {
        crtl_print_err("not exist module %s in app %s error.\n",module_name, app_name);
        crtl_assert_fp(stderr, 0);
        __crtl_apps_unlock;
        return NULL;
    }
    
    __app_module = rbtree_node->data;
    __module = __app_module->module;
    
    /* 删除这个APP结构 */
    crtl_rbtree_delete(__app->modules_rbtree, __app_module);
    
    crtl_rwlock_unlock(&__app->app_rw_lock);

    crtl_rwlock_destroy(&__app_module->module_rwlock);

    crtl_mfree1(__app_module);
    
    __crtl_apps_unlock;

    return __module;
}


/* 销毁应用 */
_api int crtl_application_destroy(const char *app_name)
{
    if(unlikely(!app_name)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    /* 当保存所有应用信息的标志未被设定，直接返回失败*/
    if(CAS(&__crtl_apps_tree_init_flag, 0, 0)) {
        __crtl_dbg("__crtl_apps_tree_init_flag = %ld.\n", __crtl_apps_tree_init_flag);
        crtl_print_err("none of app be founded error.\n");
        return CRTL_ERROR;
    }

    struct crtl_application_struct *__app = NULL;

    __crtl_apps_rdlock;
    
    /* 如果app不存在 */
    if(NULL == (__app = __crtl_application_getappbyname(app_name))) {
        crtl_print_err("app not exist %s.\n", app_name);
        crtl_assert_fp(stderr, 0);
        __crtl_apps_unlock;
        return CRTL_ERROR;
        
    } else {
    /* 如果app存在 */
        crtl_rbtree_delete(__crtl_apps_tree, __app);

        /* 删除属于该应用的模块存放树 */
        if(CAS(&__app->modules_rbtree_init_flag, 1, 0)) {
            crtl_rbtree_destroy(__app->modules_rbtree);
        }
        
        crtl_mfree1(__app);
    }
    __crtl_apps_unlock;
    
    return CRTL_SUCCESS;
}

