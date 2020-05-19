#ifndef __CRTL_CONFIG_H
#define __CRTL_CONFIG_H 1

#include <stdio.h>

#include "crtl/bits/types_basic.h"

/**
 * configuration handle
 */
typedef void* crtl_config_t;


/**
 *  读写config文件（如xxx.conf文件）
 */
/**
 * create configuration 
 * @param void
 * @return type of crtl_config_t*
 */
crtl_config_t crtl_config_new(void);

/**
 * free configuration 
 * @param cfg: configuration that create by crtl_config_new
 * @return type of crtl_config_t*
 */
void crtl_config_free(crtl_config_t cfg);


/**
 * get error string of configuration 
 * @param ret: return value of crtl_config_xxx api
 * @return error string
 */
const char *crtl_config_strerror(int ret);

/**
 * read configuration from FILE pointer
 * @param fp: FILE* of configuration file
 * @param cfg: configuration that create by crtl_config_new
 * @return error number
 */
int crtl_config_read(FILE *fp, crtl_config_t *cfg);


/**
 * read configuration from file
 * @param filename: name string of configuration file
 * @param cfg: configuration that create by crtl_config_new
 * @return error number
 */
int crtl_config_readfile(const char *filename, crtl_config_t *cfg);

/**
 * print configuration to file pointer
 * @param cfg: configuration that create by crtl_config_new
 * @param stream: file pointer
 * @return error number
 */
int crtl_config_print(const crtl_config_t cfg, FILE *stream);

/**
 * print configuration to file 
 * @param cfg: configuration that create by crtl_config_new
 * @param filename: name string of configuration file
 * @return error number
 */
int crtl_config_printtofile(const crtl_config_t cfg, char *filename);

/**
 * print configuration settings to file pointer
 * @param cfg: configuration that create by crtl_config_new
 * @param stream: file pointer
 * @return error number
 */
int crtl_config_printsettings(const crtl_config_t cfg, FILE *stream);

/**
 * get number of section of configuration
 * @param cfg: configuration that create by crtl_config_new
 * @return section number if success, -1 if error
 */
int crtl_config_get_sectioncount(const crtl_config_t cfg);

/**
 * get number of key of section in configuration
 * @param cfg: configuration that create by crtl_config_new
 * @param sect: section name string of configuration
 * @return key number if success, -1 if error
 */
int crtl_config_get_keycount(const crtl_config_t cfg, const char *sect);

/**
 * set comment char of configuration
 * @param cfg: configuration that create by crtl_config_new
 * @param comment_ch: comment char, "#" is default
 * @return error number
 */
int crtl_config_set_commentcharset(crtl_config_t cfg, const char *comment_ch);

/**
 * set key value seperator char of configuration
 * @param cfg: configuration that create by crtl_config_new
 * @param ch: seperator char, "=" is default
 * @return error number
 */
int crtl_config_set_keyvalsepchar(crtl_config_t cfg, char ch);

/**
 * set boolean string of configuration
 * @param cfg: configuration that create by crtl_config_new
 * @param true_str: true string, "1" is default
 * @param false_str: true string, "0" is default
 * @return error number
 */
int crtl_config_set_boolstring(crtl_config_t cfg, const char *true_str, const char *false_str);


/**
 * \brief              crtl_config_read_string() reads a string value from the cfg.
 *                     If any error occurs default value is copied to 'value' buffer and
 *                     returns reason. If key is optional and does not exists in config,
 *                     the 'value' is default value and return is CRTL_CONFIG_ERR_NO_KEY
 *
 * \param cfg          config handle
 * \param section      section to search in
 * \param key          key to search for
 * \param value        value to save in
 * \param size         value buffer size
 * \param dfl_value    default value to copy back if any error occurs
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_read_string(const crtl_config_t cfg, const char *sect, const char *key, char *val, int size, const char *dfl_val);

/**
 * \brief              crtl_config_read_integer() reads an integer value from the cfg.
 *                     If any error occurs default value is copied to 'value' buffer and
 *                     returns reason. If key is optional and does not exists in config,
 *                     the 'value' is default value and return is CRTL_CONFIG_ERR_NO_KEY
 *
 * \param cfg          config handle
 * \param section      section to search in
 * \param key          key to search for
 * \param value        value to save in
 * \param dfl_value    default value to copy back if any error occurs
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_read_integer(const crtl_config_t cfg, const char *sect, const char *key, int *val, int dfl_val);

/**
 * \brief              crtl_config_read_unsignedinteger() reads an unsigned integer value from the cfg.
 *                     If any error occurs default value is copied to 'value' buffer and
 *                     returns reason. If key is optional and does not exists in config,
 *                     the 'value' is default value and return is CRTL_CONFIG_ERR_NO_KEY
 *
 * \param cfg          config handle
 * \param section      section to search in
 * \param key          key to search for
 * \param value        value to save in
 * \param dfl_value    default value to copy back if any error occurs
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_read_unsignedinteger(const crtl_config_t cfg, const char *sect, const char *key, unsigned int *val, unsigned int dfl_val);

/**
 * \brief              crtl_config_read_float() reads a float value from the cfg.
 *                     If any error occurs default value is copied to 'value' buffer and
 *                     returns reason. If key is optional and does not exists in config,
 *                     the 'value' is default value and return is CRTL_CONFIG_ERR_NO_KEY
 *
 * \param cfg          config handle
 * \param section      section to search in
 * \param key          key to search for
 * \param value        value to save in
 * \param dfl_value    default value to copy back if any error occurs
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_read_float(const crtl_config_t cfg, const char *sect, const char *key, float *val, float dfl_val);

/**
 * \brief              crtl_config_read_double() reads a double value from the cfg.
 *                     If any error occurs default value is copied to 'value' buffer and
 *                     returns reason. If key is optional and does not exists in config,
 *                     the 'value' is default value and return is CRTL_CONFIG_ERR_NO_KEY
 *
 * \param cfg          config handle
 * \param section      section to search in
 * \param key          key to search for
 * \param value        value to save in
 * \param dfl_value    default value to copy back if any error occurs
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_read_double(const crtl_config_t cfg, const char *sect, const char *key, double *val, double dfl_val);


/**
 * \brief              crtl_config_read_bool() reads a boolean value from the cfg.
 *                     If any error occurs default value is copied to 'value' buffer and
 *                     returns reason. If key is optional and does not exists in config,
 *                     the 'value' is default value and return is CRTL_CONFIG_ERR_NO_KEY
 *
 * \param cfg          config handle
 * \param section      section to search in
 * \param key          key to search for
 * \param value        value to save in
 * \param dfl_value    default value to copy back if any error occurs
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_read_bool(const crtl_config_t cfg, const char *sect, const char *key, bool *val, bool dfl_val);

/**
 * \brief              crtl_config_add_string() adds the key with string value to the cfg
 *
 * \param cfg          config handle
 * \param section      section to add in
 * \param key          key to save as
 * \param value        value to save as
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_add_string(crtl_config_t cfg, const char *sect, const char *key, const char *val);

/**
 * \brief              crtl_config_add_integer() adds the key with integer value to the cfg
 *
 * \param cfg          config handle
 * \param section      section to add in
 * \param key          key to save as
 * \param value        value to save as
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_add_integer(crtl_config_t cfg, const char *sect, const char *key, int val);

/**
 * \brief              crtl_config_add_unsignedinteger() adds the key with unsigned integer value to the cfg
 *
 * \param cfg          config handle
 * \param section      section to add in
 * \param key          key to save as
 * \param value        value to save as
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_add_unsignedinteger(crtl_config_t cfg, const char *sect, const char *key, unsigned int val);

/**
 * \brief              crtl_config_add_float() adds the key with float value to the cfg
 *
 * \param cfg          config handle
 * \param section      section to add in
 * \param key          key to save as
 * \param value        value to save as
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_add_float(crtl_config_t cfg, const char *sect, const char *key, float val);

/**
 * \brief              crtl_config_add_double() adds the key with double value to the cfg
 *
 * \param cfg          config handle
 * \param section      section to add in
 * \param key          key to save as
 * \param value        value to save as
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_add_double(crtl_config_t cfg, const char *sect, const char *key, double val);

/**
 * \brief              crtl_config_add_bool() adds the key with blooean value to the cfg
 *
 * \param cfg          config handle
 * \param section      section to add in
 * \param key          key to save as
 * \param value        value to save as
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_add_bool(crtl_config_t cfg, const char *sect, const char *key, bool val);

/**
 * \brief              Checks whether section exists
 *
 * \param cfg          config handle to search in
 * \param section      section name to search for
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
bool crtl_config_has_section(const crtl_config_t cfg, const char *sect);

/**
 * \brief              crtl_config_remove_section() removes section from the cfgfile
 *
 * \param cfg          config handle
 * \param section      section to remove
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_remove_section(crtl_config_t cfg, const char *sect);

/**
 * \brief              crtl_config_remove_key() removes the key which exists under section from the cfg
 *
 * \param cfg          config handle
 * \param section      section to seach in
 * \param key          key to remove
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
int crtl_config_remove_key(crtl_config_t cfg, const char *sect, const char *key);



#endif /*<__CRTL_CONFIG_H>*/


