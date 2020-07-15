#include <stdlib.h>

#include "crtl/init.h"

#include "crypto/init.h"
#include "crypto/attribute.h"


/* 初始化标志 */
static volatile crtl_boolean crtl_init_flag = crtl_false;


/**
 *  crtl library initializer function
 *  @param argc: argument count
 *  @param argv: argument values
 *              int main(int argc, char *argv[]);
 *  @return error number
 */
_api _initfn int crtl_init(int argc, char **argv)
{
    /* 初始化内存分配函数 */
    crtl_allocator_init();

    /* 解析 argv，并且配置对应参数 */
    crtl_init_parse_config(argc, argv);

    /* 初始化 */
    
    //TODO: 初始化一些全局变量

    /* 初始化标志 */
    crtl_init_flag = crtl_true;

    return CRTL_SUCCESS;
}



/**
 *  crtl library exit function
 *  @param status: CRTL_SUCCESS, CRTL_ERROR
 *  @return error number
 */
_api _initfn void crtl_exit(int status)
{
    //TODO
    
    /* 初始化标志 */
    crtl_init_flag = crtl_false;
    exit(status);
    
    return;
}

