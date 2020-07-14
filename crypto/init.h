#ifndef __CRTL_CRYPTO_INIT_H
#define __CRTL_CRYPTO_INIT_H 1


#include "crtl/bits/types_basic.h"
#include "crypto/attribute.h"

#define CRTL_CONFIG_INI "crtl.ini"

#define CFG_ARGV(e) e##_V
enum cfg_argv {
    CRTL_CFG_ARGV_CONFIG_INI_NAME = 'c',
#define CRTL_CFG_ARGV_CONFIG_INI_NAME_V "crtl-cfg-ini"
};
#define CRTL_CFG_ARGV_LIST "c:"


/**
 *  解析配置 args 和 配置文件 CRTL_CONFIG_INI(crtl.ini)
 *  int crtl_init(int argc, char **argv)
 */
_hidden int crtl_init_parse_config(int argc, char **argv);



#endif /*<__CRTL_CRYPTO_INIT_H>*/

