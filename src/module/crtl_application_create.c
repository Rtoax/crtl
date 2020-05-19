
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
inline static int _unused __crtl_app_modules_cmp(const void *data1, const void *data2)
{
    struct crtl_module_struct *module1 = (struct crtl_module_struct *)data1;
    struct crtl_module_struct *module2 = (struct crtl_module_struct *)data2;
    
    if(strcmp(module1->module_name, module2->module_name) > 0) return CRTL_GT;
    else if(strcmp(module1->module_name, module2->module_name) == 0) return CRTL_EQ;
    else if(strcmp(module1->module_name, module2->module_name) < 0) return CRTL_LT;
    else return CRTL_EQ;
}



/* app结构体比较 */
inline static int _unused __crtl_app_apps_cmp(const void *data1, const void *data2)
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
    crtl_rbtree_node_t rbtree_node = NULL;
    
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

    __crtl_apps_rdlock;
    
    /* 当保存所有应用信息的标志被设定，查找app名称是否冲突 */
    if(CAS(&__crtl_apps_tree_init_flag, 1, 1)) {
        __crtl_dbg("__crtl_apps_tree_init_flag = %ld.\n", __crtl_apps_tree_init_flag);
        
        /* 如果app已经存在 */
        if(NULL != __crtl_application_getappbyname(app_name)) {
            crtl_print_err("app already create %s.\n", app_name);
            crtl_assert_fp(stderr, 0);
            __crtl_apps_unlock;
            
            return CRTL_ERROR;
        }
        
    }
    __crtl_apps_unlock;

    __crtl_apps_wrlock;
    /* 当保存所有应用信息的标志未被设定，初始化这个树 */
    if(CAS(&__crtl_apps_tree_init_flag, 0, 1)) {
        __crtl_dbg("__crtl_apps_tree_init_flag = %ld.\n", __crtl_apps_tree_init_flag);
        
        /* 初始化红黑树 */
        __crtl_apps_tree = crtl_rbtree_init(&__crtl_app_apps_cmp);
        crtl_assert_fp(stderr, __crtl_apps_tree);
        if(!__crtl_apps_tree) {/* 初始化失败 */
            crtl_print_err("crtl_rbtree_init error.\n");
            crtl_assert_fp(stderr, __crtl_apps_tree);
            __crtl_apps_unlock;
            return CRTL_ERROR;
        }
    }
    __crtl_apps_unlock;
    
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
    __CRTL_APP_MODULES_RWLOCK_INIT(__app);
    
    /* 初始化属于该应用的模块存放树 */
    if(CAS(&__app->modules_rbtree_init_flag, 0, 1)) {
        __app->modules_rbtree = crtl_rbtree_init(&__crtl_app_modules_cmp);
    }
    
    strcpy(__app->app_name, app_name);
    
    __crtl_apps_wrlock;
    
    /* 将app添加至app树中 */
    int ret = crtl_rbtree_insert(__crtl_apps_tree, __app, sizeof(struct crtl_application_struct));
    if(ret != CRTL_SUCCESS) {
        crtl_print_err("insert app %s to apptree %s error.\n", app_name);

        __CRTL_APP_MODULES_RWLOCK_DESTROY(__app);
    
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

    /* 模块已经在APP中 */
    __module->module_already_in_app = true;
    
    __crtl_dbg("insert appname %s, modulename %s\n", app_name, __module->module_name);

    /* 读取应用树-锁住 */
    __crtl_apps_rdlock;

    /* 读取应用树-如果不存在，解锁后退出 */
    struct crtl_application_struct _unused* __app = __crtl_application_getappbyname(app_name);
    if(unlikely(!__app)) {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        __crtl_apps_unlock;
        return CRTL_ERROR;
    }

    /* 准备向该应用中添加模块-锁住应用中读写模块的写锁 */
    __CRTL_APP_MODULES_RWLOCK_WRLOCK(__app);

    /* 向该应用的模块树种添加新的模块 */
    int ret = crtl_rbtree_insert(__app->modules_rbtree, __module, sizeof(struct crtl_module_struct));
    if(ret != CRTL_SUCCESS) {
        crtl_print_err("insert module %s to app %s error.\n", __module->module_name, app_name);
        crtl_assert_fp(stderr, 0);

        /* 向该应用的模块树种添加新的模块失败-释放锁 */
        __CRTL_APP_MODULES_RWLOCK_UNLOCK(__app);

        __crtl_apps_unlock;
        return CRTL_ERROR;
    }
    /* 向该应用的模块树种添加新的模块成功后，也要释放锁 */
    __CRTL_APP_MODULES_RWLOCK_UNLOCK(__app);
    
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
    crtl_rbtree_node_t rbtree_node = NULL;
    struct crtl_module_struct *__module = NULL;

    
    /* 查询这个APP结构 */
    __crtl_apps_rdlock;
    
    struct crtl_application_struct _unused* __app = __crtl_application_getappbyname(app_name);

    /* 准备从该应用中删除模块-锁住应用中读写模块的写锁 */
    __CRTL_APP_MODULES_RWLOCK_WRLOCK(__app);

    /* 搜索 */
    /* 组件一个临时模块 */
    struct crtl_module_struct __tmp_module;
    memset(&__tmp_module, 0, sizeof(struct crtl_module_struct));
    strcpy(__tmp_module.module_name, module_name);

    /* 查找 */
    rbtree_node = crtl_rbtree_search(__app->modules_rbtree, &__tmp_module);
    if(!rbtree_node) {
        
        /* 如果该模块在该应用中不存在，解锁后退出 */
        crtl_print_err("not exist module %s in app %s error.\n",module_name, app_name);
        crtl_assert_fp(stderr, 0);
        
        __CRTL_APP_MODULES_RWLOCK_UNLOCK(__app);
        __crtl_apps_unlock;
        return NULL;
    }

    /* 如果该模块在该应用中存在，将模块只想该模块，用于后续释放资源 */
    __module = rbtree_node->data;
    
    /* 删除这个APP中的该模块 */
    crtl_rbtree_delete(__app->modules_rbtree, __module);

    /* 解锁 */
    __CRTL_APP_MODULES_RWLOCK_UNLOCK(__app);
    __crtl_apps_unlock;

    return __module;
}


/* 销毁应用 */
_api int crtl_application_destroy(const char *app_name)
{
    /* 监测参数合法性 */
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
        
    /* 如果app存在 */
    } else {
        /* 如果模块树已经被初始化过，那么准备删除该应用的模块存放树 */
        if(CAS(&__app->modules_rbtree_init_flag, 1, 1)) {

            /* 应用内是否存在模块，若存在模块，那么删除失败 */
            if(crtl_rbtree_is_empty(__app->modules_rbtree) != CRTL_SUCCESS) {
                crtl_print_err("app  %s module tree is not empty, has %d's modules.\n", 
                                    app_name, crtl_rbtree_nnode(__app->modules_rbtree));
                crtl_assert_fp(stderr, 0);
                __crtl_apps_unlock;
                return CRTL_ERROR;

            /* 应用内是否存在模块，若不存在模块，那么销毁模块存放树，并清理内存 */
            } else {
                /*  */
                __crtl_apps_unlock;
                __crtl_apps_wrlock;
                crtl_rbtree_destroy(__app->modules_rbtree);
                crtl_rbtree_delete(__crtl_apps_tree, __app);
                crtl_mfree1(__app);

                /* 当保存所有应用信息的标志被设定，查找app树是否为空，如果为空，销毁这个树*/
                if(CAS(&__crtl_apps_tree_init_flag, 1, 1)) {
                    if(crtl_rbtree_is_empty(__crtl_apps_tree) == CRTL_SUCCESS) {
                        crtl_rbtree_destroy(__crtl_apps_tree);
                        __crtl_apps_tree = NULL;
                        CAS(&__crtl_apps_tree_init_flag, 1, 0);
                    }
                }
            }
        }
    }
    __crtl_apps_unlock;
    
    return CRTL_SUCCESS;
}





