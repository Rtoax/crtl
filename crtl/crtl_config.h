#ifndef __CRTL_CONFIG_H
#define __CRTL_CONFIG_H 1

#include "crtl/bits/crtl_configuration.h"


typedef __crtl_config_t crtl_config_t;


/**
 *  读写config文件（如xxx.conf文件）
 */
_api crtl_config_t *crtl_config_new(void);
_api void crtl_config_free(crtl_config_t *cfg);

_api const char *crtl_config_strerror(int ret);

_api int crtl_config_read(FILE *fp, crtl_config_t **cfg);
_api int crtl_config_readfile(const char *filename, crtl_config_t **cfg);

_api int crtl_config_print(const crtl_config_t *cfg, FILE *stream);
_api int crtl_config_printtofile(const crtl_config_t *cfg, char *filename);
_api int crtl_config_printsettings(const crtl_config_t *cfg, FILE *stream);

_api int crtl_config_get_sectioncount(const crtl_config_t *cfg);
_api int crtl_config_get_keycount(const crtl_config_t *cfg, const char *sect);

_api int crtl_config_set_commentcharset(crtl_config_t *cfg, const char *comment_ch);
_api int crtl_config_set_keyvalsepchar(crtl_config_t *cfg, char ch);
_api int crtl_config_set_boolstring(crtl_config_t *cfg, const char *true_str, const char *false_str);

_api int crtl_config_read_string(const crtl_config_t *cfg, const char *sect, const char *key, char *val, int size, const char *dfl_val);
_api int crtl_config_read_integer(const crtl_config_t *cfg, const char *sect, const char *key, int *val, int dfl_val);
_api int crtl_config_read_unsignedinteger(const crtl_config_t *cfg, const char *sect, const char *key, unsigned int *val, unsigned int dfl_val);
_api int crtl_config_read_float(const crtl_config_t *cfg, const char *sect, const char *key, float *val, float dfl_val);
_api int crtl_config_read_double(const crtl_config_t *cfg, const char *sect, const char *key, double *val, double dfl_val);
_api int crtl_config_read_bool(const crtl_config_t *cfg, const char *sect, const char *key, bool *val, bool dfl_val);

_api int crtl_config_add_string(crtl_config_t *cfg, const char *sect, const char *key, const char *val);
_api int crtl_config_add_integer(crtl_config_t *cfg, const char *sect, const char *key, int val);
_api int crtl_config_add_unsignedinteger(crtl_config_t *cfg, const char *sect, const char *key, unsigned int val);
_api int crtl_config_add_float(crtl_config_t *cfg, const char *sect, const char *key, float val);
_api int crtl_config_add_double(crtl_config_t *cfg, const char *sect, const char *key, double val);
_api int crtl_config_add_bool(crtl_config_t *cfg, const char *sect, const char *key, bool val);

_api bool crtl_config_has_section(const crtl_config_t *cfg, const char *sect);

_api int crtl_config_remove_section(crtl_config_t *cfg, const char *sect);
_api int crtl_config_remove_key(crtl_config_t *cfg, const char *sect, const char *key);



#endif /*<__CRTL_CONFIG_H>*/

