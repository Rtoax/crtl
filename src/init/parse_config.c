#include <getopt.h>
#include <unistd.h>

#include "crtl/config.h"
#include "crtl/log.h"
#include "crtl/file.h"
#include "crtl/assert.h"

#include "crypto/init.h"


/* 初始配置文件句柄 */
static crtl_config_t crtl_cfg_ini = NULL;



/* 检查初始配置文件是否存在，存在则打开 */
static void parse_config_ini_file_open(char *filename, crtl_config_t *cfg_ini)
{
    static crtl_boolean open_success = crtl_false;
    
    if(open_success == crtl_true) {
        crtl_print_debug("already parse config.ini file OK....\n");
        return;
    }
        
    /* check file exist: filename */
    if(filename)
    if(crtl_is_regular_file(filename) == CRTL_SUCCESS) {
        crtl_print_debug("exist %s file OK....\n", filename);
        if (crtl_config_readfile(filename, cfg_ini) != CRTL_SUCCESS) {
            crtl_print_err("open failed for %s, try %s\n", filename, CRTL_CONFIG_INI);
            crtl_assert(0);
        } else {
            /* 打开文件成功，直接返回 */
            open_success = crtl_true;
            return;
        }
    } 
    crtl_print_warning("file not exist %s, try %s\n", filename, CRTL_CONFIG_INI);
    
    /* 如果上面的文件不存在，先警告，再尝试当前目录下的默认名称的配置文件 */
    if(crtl_is_regular_file(CRTL_CONFIG_INI) == CRTL_SUCCESS) {
        if (crtl_config_readfile(CRTL_CONFIG_INI, cfg_ini) != CRTL_SUCCESS) {
            crtl_print_warning("open failed for %s, try %s\n", CRTL_CONFIG_INI, "/etc/"CRTL_CONFIG_INI);
        } else {
            /* 打开文件成功，直接返回 */
            open_success = crtl_true;
            return;
        }
    } 
    crtl_print_warning("file not exist %s, try %s\n", CRTL_CONFIG_INI, "/etc/"CRTL_CONFIG_INI);

    /* 如果上面的文件不存在，先警告，再尝试"/etc/"下的默认名称的配置文件 */
    if(crtl_is_regular_file("/etc/"CRTL_CONFIG_INI) == CRTL_SUCCESS) {
        if (crtl_config_readfile("/etc/"CRTL_CONFIG_INI, cfg_ini) != CRTL_SUCCESS) {
            crtl_print_warning("open failed for %s, init failed.\n", "/etc/"CRTL_CONFIG_INI);
            crtl_assert(0);
        } else {
            /* 打开文件成功，直接返回 */
            open_success = crtl_true;
            return;
        }
    } 

    crtl_print_err("file not exist %s, %s, and %s\n", filename, CRTL_CONFIG_INI, "/etc/"CRTL_CONFIG_INI);
    crtl_assert(0);
    
    return;
}


/* 初始配置文件加载 */
static void parse_config_ini_file_load(crtl_config_t cfg_ini)
{
    //TODO: 加载配置文件
    if(crtl_config_has_section(cfg_ini, "crtl")) {
        crtl_print_debug("Has Section crtl.\n");
    }

    //TODO: 在此处添加
}


/* 关闭初始配置文件句柄 */
static void parse_config_ini_file_close(crtl_config_t *cfg_ini)
{
    if(cfg_ini)
        crtl_config_free(*cfg_ini);
}


/* 解析命令行参数   */
static void parse_args(int argc, char **argv)
{
    crtl_print_debug("parse.\n");
    
    struct option options[] = {
        {CFG_ARGV(CRTL_CFG_ARGV_CONFIG_INI_NAME), required_argument, 0, CRTL_CFG_ARGV_CONFIG_INI_NAME},
        {0, 0, 0, 0}
    };
        
    while (1)
    {
        /* getopt_long stores the option index here. */
        int c, option_index = 0;
        
        c = getopt_long (argc, argv, CRTL_CFG_ARGV_LIST, options, &option_index);
        
        /* Detect the end of the options. */
        if (c == -1)
            break;
        switch (c)
        {
            case CRTL_CFG_ARGV_CONFIG_INI_NAME:
                crtl_print_debug("parse file %s.\n", optarg);
                parse_config_ini_file_open(optarg, &crtl_cfg_ini);
                break;
            case '?':
                /* getopt_long already printed an error message. */
                break;
            default:
                abort ();
        }
    }
    
    
}


/* 解析配置文件 */
_hidden int crtl_init_parse_config(int argc, char **argv)
{
    /* 解析参数 */
    parse_args(argc, argv);

    /* 打开配置文件, 当 argv中不存在这个文件，再次试图打开默认的配置文件 */
    parse_config_ini_file_open(NULL, &crtl_cfg_ini);

    parse_config_ini_file_load(crtl_cfg_ini);
    
    parse_config_ini_file_close(&crtl_cfg_ini);
    
    //TODO
    
    return CRTL_SUCCESS;
}

