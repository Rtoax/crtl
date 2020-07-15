#ifndef __CRTL_CRYPTO_INIT_H
#define __CRTL_CRYPTO_INIT_H 1


#include "crtl/bits/types_basic.h"
#include "crypto/attribute.h"
#include "crypto/section.h"

#include "crypto/timer/types.h"


#define CRTL_CONFIG_INI "crtl.ini"

#define CFG_ARGV(e) e##_V
enum cfg_argv {
    CRTL_CFG_ARGV_CONFIG_INI_NAME = 'c',
#define CRTL_CFG_ARGV_CONFIG_INI_NAME_V "crtl-cfg-ini"
    //MORE...
};
#define CRTL_CFG_ARGV_LIST "c:"


/**
 *  解析配置 args 和 配置文件 CRTL_CONFIG_INI(crtl.ini)
 *  int crtl_init(int argc, char **argv)
 */
_hidden int crtl_init_parse_config(int argc, char **argv);

/**
 *  初始化内存分配器： malloc free等
 */
_hidden _initfn void crtl_allocator_init();

/**
 *  更新定时器更新事件间隔：单位 纳秒
 */
void crtl_timer_set_update_interval(long int nanosec);


#endif /*<__CRTL_CRYPTO_INIT_H>*/

