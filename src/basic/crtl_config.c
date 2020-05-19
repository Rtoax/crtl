#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "crtl/bits/types_basic.h"
#include "crtl/easy/macro.h"
#include "crtl/easy/attribute.h"

#include "crypto/queue/queue.h"

//#include "crtl/config.h"

/*
	libconfigini - an ini formatted configuration parser library
	Copyright (C) 2013-present Taner YILMAZ <taner44@gmail.com>

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	 1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
	 2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
	 3. Neither the name of copyright holders nor the names of its
		contributors may be used to endorse or promote products derived
		from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
	TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
	PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
	BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
 */



static char _unused __comment_chars[] = {"#"};
static char _unused __keyval_sep = '=';
static char _unused __str_true[] = "1";
static char _unused __str_false[] = "0";

#define COMMENT_CHARS        __comment_chars    /* default comment chars */
#define KEYVAL_SEP           __keyval_sep       /* default key-val seperator character */
#define STR_TRUE             __str_true         /* default string valu of true */
#define STR_FALSE            __str_false        /* default string valu of false */

#define CRTL_CONFIG_INIT_MAGIC    0x12F0ED1

#define CRTL_CONFIG_SECTION_FLAT		NULL	/* config is flat data (has no section) */


/**
 * \brief Return types
 */
enum {
	CRTL_CONFIG_OK = CRTL_SUCCESS,                     /* ok (no error) */
	CRTL_CONFIG_ERR_FILE = -1,              /* file io error (file not exists, cannot open file, ...) */
	CRTL_CONFIG_ERR_NO_SECTION = -2,        /* section does not exist */
	CRTL_CONFIG_ERR_NO_KEY = -3,            /* key does not exist */
	CRTL_CONFIG_ERR_MEMALLOC = -4,          /* memory allocation failed */
	CRTL_CONFIG_ERR_INVALID_PARAM = -5,     /* invalid parametrs (as NULL) */
	CRTL_CONFIG_ERR_INVALID_VALUE = -6,     /* value of key is invalid (inconsistent data, empty data) */
	CRTL_CONFIG_ERR_PARSING = -7,           /* parsing error of data (does not fit to config format) */
};

/**
 * \brief Configuration key-value
 */
typedef struct __crtl_config_key_value
{
	char *key;
	char *value;
	TAILQ_ENTRY(__crtl_config_key_value) next;
} __crtl_config_key_value;

/**
 * \brief Configuration section
 */
typedef struct __crtl_config_section
{
	char *name;
	int numofkv;
	TAILQ_HEAD(, __crtl_config_key_value) kv_list;
	TAILQ_ENTRY(__crtl_config_section) next;
} __crtl_config_section;

/**
 * \brief Configuration handle
 */
struct crtl_config_struct
{
	char *comment_chars;
	char keyval_sep;
	char *true_str;
	char *false_str;
	int  initnum;
	int  numofsect;
	TAILQ_HEAD(, __crtl_config_section) sect_list;
};




static int __crtl_config_StrSafeCopy(char *dst, const char *src, int size)
{
	char *d = dst;
	const char *s = src;
	int n = size;

	if (n != 0 && --n != 0) {
		do {
			if ((*d++ = *s++) == 0)
				break;
		} while (--n != 0);
	}

	if (n == 0) {
		if (size != 0)
			*d = '\0';
		while (*s++)
			;
	}

	return (s - src - 1);
}

static bool __crtl_config_StrIsTypeOfTrue(const char *s)
{
	if ( !strcasecmp(s, "true") || !strcasecmp(s, "yes") || !strcasecmp(s, "1") )
		return true;
	return false;
}

static bool __crtl_config_StrIsTypeOfFalse(const char *s)
{
	if ( !strcasecmp(s, "false") || !strcasecmp(s, "no") || !strcasecmp(s, "0") )
		return true;
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


_api const char *crtl_config_strerror(int ret)
{
	switch(ret) {
		case CRTL_CONFIG_OK:                return "OK";
		case CRTL_CONFIG_ERR_FILE:          return "File IO error";
		case CRTL_CONFIG_ERR_NO_SECTION:    return "No section";
		case CRTL_CONFIG_ERR_NO_KEY:        return "No key";
		case CRTL_CONFIG_ERR_MEMALLOC:      return "Memory allocation failed";
		case CRTL_CONFIG_ERR_INVALID_PARAM: return "Invalid parameter";
		case CRTL_CONFIG_ERR_INVALID_VALUE: return "Invalid value";
		case CRTL_CONFIG_ERR_PARSING:       return "Parse error";
		default:                       return "Unknown";
	}
}

/**
 * \brief              crtl_config_set_commentcharset() sets comment characters
 *
 * \param cfg          config handle
 * \param comment_ch   charaters to consider as comments
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
_api int crtl_config_set_commentcharset(struct crtl_config_struct *cfg, const char *comment_ch)
{
	char *p;

	if (!cfg || !comment_ch)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	if ((p = strdup(comment_ch)) == NULL)
		return CRTL_CONFIG_ERR_MEMALLOC;

	if (cfg->comment_chars)
		free(cfg->comment_chars);
	cfg->comment_chars = p;

	return CRTL_CONFIG_OK;
}

/**
 * \brief              crtl_config_set_commentcharset() sets comment characters
 *
 * \param cfg          config handle
 * \param ch           charater to consider as key-value seperator
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
_api int crtl_config_set_keyvalsepchar(struct crtl_config_struct *cfg, char ch)
{
	if (!cfg)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	cfg->keyval_sep = ch;

	return CRTL_CONFIG_OK;
}

/**
 * \brief              crtl_config_set_commentcharset() sets comment characters
 *
 * \param cfg          config handle
 * \param true_str     string value of boolean true (must be one of these: "true", "yes", "1")
 * \param false_str    string value of boolean false (must be one of these: "false", "no", "0")
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
_api int crtl_config_set_boolstring(struct crtl_config_struct *cfg, const char *true_str, const char *false_str)
{
	char *t, *f;

	if ( !cfg ||
		 !true_str || !*true_str || !__crtl_config_StrIsTypeOfTrue(true_str) ||
		 !false_str || !*false_str || !__crtl_config_StrIsTypeOfFalse(false_str) )
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	if ((t = strdup(true_str)) == NULL)
		return CRTL_CONFIG_ERR_MEMALLOC;

	if ((f = strdup(false_str)) == NULL) {
		free(t);
		return CRTL_CONFIG_ERR_MEMALLOC;
	}

	if (cfg->true_str)
		free(cfg->true_str);
	if (cfg->false_str)
		free(cfg->false_str);

	cfg->true_str = t;
	cfg->false_str = f;

	return CRTL_CONFIG_OK;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * \brief              __crtl_config_GetSection() gets the requested section
 *
 * \param cfg          config handle to search in
 * \param section      section name to search for
 * \param sect         pointer to __crtl_config_section* searched for to save
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
static int __crtl_config_GetSection(const struct crtl_config_struct *cfg, const char *section, __crtl_config_section **sect)
{
	if (!cfg || !sect)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	TAILQ_FOREACH(*sect, &cfg->sect_list, next) {
		if ( (section && (*sect)->name && !strcmp((*sect)->name, section)) ||
			 (!section && !(*sect)->name) ) {
			return CRTL_CONFIG_OK;
		}
	}

	return CRTL_CONFIG_ERR_NO_SECTION;
}

/**
 * \brief              Checks whether section exists
 *
 * \param cfg          config handle to search in
 * \param section      section name to search for
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
_api bool crtl_config_has_section(const struct crtl_config_struct *cfg, const char *section)
{
	__crtl_config_section *sect = NULL;

	return ( (__crtl_config_GetSection(cfg, section, &sect) == CRTL_CONFIG_OK) ? true : false );
}

/**
 * \brief              __crtl_config_GetKeyValue() gets the __crtl_config_key_value *
 *
 * \param cfg          config handle
 * \param sect         section to search in
 * \param key          key to search for
 * \param kv           pointer to __crtl_config_key_value* searched for to save
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
static int __crtl_config_GetKeyValue(const struct crtl_config_struct *cfg, __crtl_config_section *sect, const char *key,
		__crtl_config_key_value **kv)
{
	if (!sect || !key || !kv)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	TAILQ_FOREACH(*kv, &sect->kv_list, next) {
		if (!strcmp((*kv)->key, key))
			return CRTL_CONFIG_OK;
	}

	return CRTL_CONFIG_ERR_NO_KEY;
}

/**
 * \brief            crtl_config_get_sectioncount() gets number of sections
 *
 * \param cfg        config handle to search in
 *
 * \return           Returns number of sections on success, -1 on failure.
 */
_api int crtl_config_get_sectioncount(const struct crtl_config_struct *cfg)
{
	if (!cfg)
		return -1;

	return (TAILQ_FIRST(&cfg->sect_list)->numofkv > 0 ? cfg->numofsect : cfg->numofsect - 1);
}

/**
 * \brief            crtl_config_get_keycount() gets number of keys
 *
 * \param cfg        config handle to search in
 * \param section    section name to search for
 *
 * \return           Returns number of keys on success, -1 on failure.
 */
_api int crtl_config_get_keycount(const struct crtl_config_struct *cfg, const char *section)
{
	__crtl_config_section *sect = NULL;

	if (!cfg)
		return -1;

	if (__crtl_config_GetSection(cfg, section, &sect) != CRTL_CONFIG_OK)
		return -1;

	return sect->numofkv;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


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
_api int crtl_config_read_string(const struct crtl_config_struct *cfg, const char *section, const char *key,
		                                char *value, int size, const char *dfl_value)
{
	__crtl_config_section  *sect = NULL;
	__crtl_config_key_value *kv   = NULL;
	int       ret  = CRTL_CONFIG_OK;

	if (!cfg || !key || !value || (size < 1))
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	*value = '\0';

	if ( ((ret = __crtl_config_GetSection(cfg, section, &sect)) != CRTL_CONFIG_OK) ||
		 ((ret = __crtl_config_GetKeyValue(cfg, sect, key, &kv)) != CRTL_CONFIG_OK) ) {
		if (dfl_value)
			__crtl_config_StrSafeCopy(value, dfl_value, size);
		return ret;
	}

	__crtl_config_StrSafeCopy(value, kv->value, size);

	return CRTL_CONFIG_OK;
}

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
_api int crtl_config_read_integer(const struct crtl_config_struct *cfg, const char *section, const char *key,
		int *value, int dfl_value)
{
	__crtl_config_section  *sect = NULL;
	__crtl_config_key_value *kv   = NULL;
	int       ret  = CRTL_CONFIG_OK;
	char           *p    = NULL;

	if (!cfg || !key || !value)
		return  CRTL_CONFIG_ERR_INVALID_PARAM;

	*value = dfl_value;

	if ( ((ret = __crtl_config_GetSection(cfg, section, &sect)) != CRTL_CONFIG_OK) ||
		 ((ret = __crtl_config_GetKeyValue(cfg, sect, key, &kv)) != CRTL_CONFIG_OK) ) {
		return ret;
	}

	*value = (int) strtol(kv->value, &p, 10);
	if (*p || (errno == ERANGE))
		return CRTL_CONFIG_ERR_INVALID_VALUE;

	return CRTL_CONFIG_OK;
}

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
_api int crtl_config_read_unsignedinteger(const struct crtl_config_struct *cfg, const char *section, const char *key,
		unsigned int *value, unsigned int dfl_value)
{
	__crtl_config_section  *sect = NULL;
	__crtl_config_key_value *kv   = NULL;
	int       ret  = CRTL_CONFIG_OK;
	char           *p    = NULL;

	if (!cfg || !key || !value)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	*value = dfl_value;

	if ( ((ret = __crtl_config_GetSection(cfg, section, &sect)) != CRTL_CONFIG_OK) ||
		 ((ret = __crtl_config_GetKeyValue(cfg, sect, key, &kv)) != CRTL_CONFIG_OK) ) {
		return ret;
	}

	*value = (unsigned int) strtoul(kv->value, &p, 10);
	if (*p || (errno == ERANGE))
		return CRTL_CONFIG_ERR_INVALID_VALUE;

	return CRTL_CONFIG_OK;
}

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
_api int crtl_config_read_float(const struct crtl_config_struct *cfg, const char *section, const char *key,
		float *value, float dfl_value)
{
	__crtl_config_section  *sect = NULL;
	__crtl_config_key_value *kv   = NULL;
	int       ret  = CRTL_CONFIG_OK;
	char           *p    = NULL;

	if (!cfg || !key || !value)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	*value = dfl_value;

	if ( ((ret = __crtl_config_GetSection(cfg, section, &sect)) != CRTL_CONFIG_OK) ||
		 ((ret = __crtl_config_GetKeyValue(cfg, sect, key, &kv)) != CRTL_CONFIG_OK) ) {
		return ret;
	}

	*value = strtof(kv->value, &p);
	if (*p || (errno == ERANGE))
		return CRTL_CONFIG_ERR_INVALID_VALUE;

	return CRTL_CONFIG_OK;
}

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
_api int crtl_config_read_double(const struct crtl_config_struct *cfg, const char *section, const char *key,
		double *value, double dfl_value)
{
	__crtl_config_section  *sect = NULL;
	__crtl_config_key_value *kv   = NULL;
	int       ret  = CRTL_CONFIG_OK;
	char           *p    = NULL;

	if (!cfg || !key || !value)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	*value = dfl_value;

	if ( ((ret = __crtl_config_GetSection(cfg, section, &sect)) != CRTL_CONFIG_OK) ||
		 ((ret = __crtl_config_GetKeyValue(cfg, sect, key, &kv)) != CRTL_CONFIG_OK) ) {
		return ret;
	}

	*value = strtod(kv->value, &p);
	if (*p || (errno == ERANGE))
		return CRTL_CONFIG_ERR_INVALID_VALUE;

	return CRTL_CONFIG_OK;
}

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
_api int crtl_config_read_bool(const struct crtl_config_struct *cfg, const char *section, const char *key,
		bool *value, bool dfl_value)
{
	__crtl_config_section  *sect = NULL;
	__crtl_config_key_value *kv   = NULL;
	int       ret  = CRTL_CONFIG_OK;

	if (!cfg || !key || !value)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	*value = dfl_value;

	if ( ((ret = __crtl_config_GetSection(cfg, section, &sect)) != CRTL_CONFIG_OK) ||
		 ((ret = __crtl_config_GetKeyValue(cfg, sect, key, &kv)) != CRTL_CONFIG_OK) ) {
		return ret;
	}

	if (__crtl_config_StrIsTypeOfTrue(kv->value))
		*value = true;
	else if (__crtl_config_StrIsTypeOfFalse(kv->value))
		*value = false;
	else
		return CRTL_CONFIG_ERR_INVALID_VALUE;

	return CRTL_CONFIG_OK;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * \brief              __crtl_config_AddSection() creates a section in the cfg
 *
 * \param cfg          config handle
 * \param section      section to add
 * \param sect         pointer to added __crtl_config_section* or NULL if not needed
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
static int __crtl_config_AddSection(struct crtl_config_struct *cfg, const char *section, __crtl_config_section **sect)
{
	__crtl_config_section *_sect = NULL;
	int      ret   = CRTL_CONFIG_OK;

	if (!cfg)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	if (!sect)
		sect = &_sect;

	if ((ret = __crtl_config_GetSection(cfg, section, sect)) != CRTL_CONFIG_ERR_NO_SECTION)
		return ret;

	*sect = calloc(1, sizeof(__crtl_config_section));
	if (*sect == NULL)
		return CRTL_CONFIG_ERR_MEMALLOC;

	if (section) {
		if (((*sect)->name = strdup(section)) == NULL) {
			free(*sect);
			return CRTL_CONFIG_ERR_MEMALLOC;
		}
	}

	TAILQ_INIT(&(*sect)->kv_list);
	TAILQ_INSERT_TAIL(&cfg->sect_list, *sect, next);
	++(cfg->numofsect);

	return CRTL_CONFIG_OK;
}

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
_api int crtl_config_add_string(struct crtl_config_struct *cfg, const char *section, const char *key, const char *value)
{
	__crtl_config_section  *sect = NULL;
	__crtl_config_key_value *kv   = NULL;
	int       ret  = CRTL_CONFIG_OK;
	const char     *p    = NULL;
	const char     *q    = NULL;

	if (!cfg || !key || !value)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	if ((ret = __crtl_config_AddSection(cfg, section, &sect)) != CRTL_CONFIG_OK)
		return ret;

	switch (ret = __crtl_config_GetKeyValue(cfg, sect, key, &kv)) {
		case CRTL_CONFIG_OK:
			if (kv->value) {
				free(kv->value);
				kv->value = NULL;
			}
			break;

		case CRTL_CONFIG_ERR_NO_KEY:
			if ((kv = calloc(1, sizeof(__crtl_config_key_value))) == NULL)
				return CRTL_CONFIG_ERR_MEMALLOC;
			if ((kv->key = strdup(key)) == NULL) {
				free(kv);
				return CRTL_CONFIG_ERR_MEMALLOC;
			}
			TAILQ_INSERT_TAIL(&sect->kv_list, kv, next);
			++(sect->numofkv);
			break;

		default:
			return ret;
	}

	for (p = value; *p && isspace(*p); ++p)
		;
	for (q = p; *q && (*q != '\r') && (*q != '\n') && !strchr(cfg->comment_chars, *q); ++q)
		;
	while (*q && (q > p) && isspace(*(q - 1)))
		--q;

	kv->value = (char *) malloc(q - p + 1);
	if (kv->value == NULL) {
		TAILQ_REMOVE(&sect->kv_list, kv, next);
		--(sect->numofkv);
		free(kv->key);
		free(kv);
		return CRTL_CONFIG_ERR_MEMALLOC;
	}
	memcpy(kv->value, p, q - p);
	kv->value[q - p] = '\0';

	return CRTL_CONFIG_OK;
}

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
_api int crtl_config_add_integer(struct crtl_config_struct *cfg, const char *section, const char *key, int value)
{
	char buf[64];

	snprintf(buf, sizeof(buf), "%d", value);

	return crtl_config_add_string(cfg, section, key, buf);
}

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
_api int crtl_config_add_unsignedinteger(struct crtl_config_struct *cfg, const char *section, const char *key, unsigned int value)
{
	char buf[64];

	snprintf(buf, sizeof(buf), "%u", value);

	return crtl_config_add_string(cfg, section, key, buf);
}

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
_api int crtl_config_add_float(struct crtl_config_struct * cfg, const char *section, const char *key, float value)
{
	char buf[64];

	snprintf(buf, sizeof(buf), "%f", value);

	return crtl_config_add_string(cfg, section, key, buf);
}

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
_api int crtl_config_add_double(struct crtl_config_struct *cfg, const char *section, const char *key, double value)
{
	char buf[64];

	snprintf(buf, sizeof(buf), "%f", value);

	return crtl_config_add_string(cfg, section, key, buf);
}

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
_api int crtl_config_add_bool(struct crtl_config_struct * cfg, const char *section, const char *key, bool value)
{
	return crtl_config_add_string(cfg, section, key, value ? cfg->true_str : cfg->false_str);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


static void __crtl_config__ConfigRemoveKey(__crtl_config_section *sect, __crtl_config_key_value *kv)
{
	TAILQ_REMOVE(&sect->kv_list, kv, next);
	--(sect->numofkv);

	if (kv->key)
		free(kv->key);
	if (kv->value)
		free(kv->value);
	free(kv);
}

/**
 * \brief              crtl_config_remove_key() removes the key which exists under section from the cfg
 *
 * \param cfg          config handle
 * \param section      section to seach in
 * \param key          key to remove
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
_api int crtl_config_remove_key(struct crtl_config_struct *cfg, const char *section, const char *key)
{
	__crtl_config_section  *sect = NULL;
	__crtl_config_key_value *kv   = NULL;
	int       ret  = CRTL_CONFIG_OK;

	if (!cfg || !key)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	if ((ret = __crtl_config_GetSection(cfg, section, &sect)) == CRTL_CONFIG_OK) {
		if ((ret = __crtl_config_GetKeyValue(cfg, sect, key, &kv)) == CRTL_CONFIG_OK)
			__crtl_config__ConfigRemoveKey(sect, kv);
	}

	return ret;
}

static void __crtl_config_RemoveSection(struct crtl_config_struct *cfg, __crtl_config_section *sect)
{
	__crtl_config_key_value *kv, *t_kv;

	if (!cfg || !sect)
		return;

	TAILQ_REMOVE(&cfg->sect_list, sect, next);
	--(cfg->numofsect);

	TAILQ_FOREACH_SAFE(kv, &sect->kv_list, next, t_kv) {
		__crtl_config__ConfigRemoveKey(sect, kv);
	}

	if (sect->name)
		free(sect->name);
	free(sect);
}

/**
 * \brief              crtl_config_remove_section() removes section from the cfgfile
 *
 * \param cfg          config handle
 * \param section      section to remove
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
_api int crtl_config_remove_section(struct crtl_config_struct *cfg, const char *section)
{
	__crtl_config_section *sect = NULL;
	int      ret  = CRTL_CONFIG_OK;

	if (!cfg)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	if ((ret = __crtl_config_GetSection(cfg, section, &sect)) == CRTL_CONFIG_OK)
		__crtl_config_RemoveSection(cfg, sect);

	return ret;
}

/**
 * \brief              crtl_config_new() creates a cfg handle with
 *                     default section which has no section name
 *
 * \return             Config* handle on success, NULL on failure
 */
_api struct crtl_config_struct *crtl_config_new()
{
	struct crtl_config_struct *cfg = NULL;

	cfg = calloc(1, sizeof(struct crtl_config_struct));
	if (cfg == NULL)
		return NULL;

	TAILQ_INIT(&cfg->sect_list);

	/* add default section */
	if (__crtl_config_AddSection(cfg, CRTL_CONFIG_SECTION_FLAT, NULL) != CRTL_CONFIG_OK) {
		free(cfg);
		return NULL;
	}

	cfg->comment_chars = strdup(COMMENT_CHARS);
	cfg->keyval_sep = KEYVAL_SEP;
	cfg->true_str = strdup(STR_TRUE);
	cfg->false_str = strdup(STR_FALSE);
	cfg->initnum = CRTL_CONFIG_INIT_MAGIC;

	return cfg;
}

/**
 * \brief          crtl_config_free() frees the memory for the cfg handle
 *
 * \param cfg      config handle
 */
_api void crtl_config_free(struct crtl_config_struct *cfg)
{
	__crtl_config_section *sect, *t_sect;

	if (cfg == NULL)
		return;

	TAILQ_FOREACH_SAFE(sect, &cfg->sect_list, next, t_sect) {
		__crtl_config_RemoveSection(cfg, sect);
	}

	if (cfg->comment_chars) free(cfg->comment_chars);
	if (cfg->true_str)      free(cfg->true_str);
	if (cfg->false_str)     free(cfg->false_str);

	free(cfg);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * \brief              Gets section name on the buffer p
 *
 * \param cfg          config handle
 * \param p            read buffer
 * \param section      pointer address to section
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
static int __crtl_config__GetSectName(struct crtl_config_struct *cfg, char *p, char **section)
{
	char *q, *r;

	if (!cfg || !p || !*p || !section)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	*section = NULL;

	/* get section name */
	while (*p && isspace(*p))
		++p;

	if (*p != '[')
		return CRTL_CONFIG_ERR_PARSING;

	++p;
	while (*p && isspace(*p))
		++p;

	for (q = p;
		 *q && (*q != '\r') && (*q != '\n') && (*q != ']') && !strchr(cfg->comment_chars, *q);
		 ++q)
		;

	if (*q != ']')
		return CRTL_CONFIG_ERR_PARSING;

	r = q + 1;

	while (*q && (q > p) && isspace(*(q - 1)))
		--q;

	if (q == p) /* section has no name */
		return CRTL_CONFIG_ERR_PARSING;

	*q = '\0';
	*section = p;

	/* check rest of section */
	while (*r && isspace(*r))
		++r;

	/* there are unrecognized trailing data */
	if (*r && !strchr(cfg->comment_chars, *r))
		return CRTL_CONFIG_ERR_PARSING;

	return CRTL_CONFIG_OK;
}

/**
 * \brief              Gets key and value on the buffer p
 *
 * \param cfg          config handle
 * \param p            read buffer
 * \param key          pointer address to key
 * \param val          pointer address to value
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
static int __crtl_config__GetKeyVal(struct crtl_config_struct *cfg, char *p, char **key, char **val)
{
	char *q, *v;

	if (!cfg || !p || !*p || !key || !val)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	*key = *val = NULL;

	/* get key */
	while (*p && isspace(*p))
		++p;

	for (q = p;
		 *q && (*q != '\r') && (*q != '\n') && (*q != cfg->keyval_sep) && !strchr(cfg->comment_chars, *q);
		 ++q)
		;

	if (*q != cfg->keyval_sep)
		return CRTL_CONFIG_ERR_PARSING;

	v = q + 1;

	while (*q && (q > p) && isspace(*(q - 1)))
		--q;

	if (q == p) /* no key name */
		return CRTL_CONFIG_ERR_PARSING;

	*q = '\0';
	*key = p;

	/* get value */
	while (*v && isspace(*v))
		++v;

	for (q = v;
		 *q && (*q != '\r') && (*q != '\n') && !strchr(cfg->comment_chars, *q);
		 ++q)
		;

	while (*q && (q > v) && isspace(*(q - 1)))
		--q;

	if (q == v) /* no value */
		return CRTL_CONFIG_ERR_INVALID_VALUE;

	*q = '\0';
	*val = v;

	return CRTL_CONFIG_OK;
}

/**
 * \brief              crtl_config_read() reads the stream and populates the entire content to cfg handle
 *
 * \param fp           FILE handle to read
 * \param cfg          pointer to config handle.
 *                     If not NULL a handle created with crtl_config_new() must be given.
 *                     If cfg is NULL a new one is created and saved to cfg.
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
_api int crtl_config_read(FILE *fp, struct crtl_config_struct **cfg)
{
	__crtl_config_section *sect    = NULL;
	char          *p       = NULL;
	char          *section = NULL;
	char          *key     = NULL;
	char          *val     = NULL;
	char           buf[4096];
	struct crtl_config_struct        *_cfg    = NULL;
	bool           newcfg  = false;
	int      ret     = CRTL_CONFIG_OK;

	if ( !fp || !cfg || (*cfg && ((*cfg)->initnum != CRTL_CONFIG_INIT_MAGIC)) )
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	if (*cfg == NULL) {
		_cfg = crtl_config_new();
		if (_cfg == NULL)
			return CRTL_CONFIG_ERR_MEMALLOC;
		*cfg = _cfg;
		newcfg = true;
	}
	else
		_cfg = *cfg;

	while (!feof(fp)) {
		if (fgets(buf, sizeof(buf), fp) == NULL)
			continue;

		for (p = buf; *p && isspace(*p) ; ++p)
			;
		if (!*p || strchr(_cfg->comment_chars, *p))
			continue;

		if (*p == '[') {
			if ((ret = __crtl_config__GetSectName(_cfg, p, &section)) != CRTL_CONFIG_OK)
				goto loop_error;

			if ((ret = __crtl_config_AddSection(_cfg, section, &sect)) != CRTL_CONFIG_OK)
				goto loop_error;
		}
		else {
			if ((ret = __crtl_config__GetKeyVal(_cfg, p, &key, &val)) != CRTL_CONFIG_OK)
				goto loop_error;

			if ((ret = crtl_config_add_string(_cfg, sect->name, key, val)) != CRTL_CONFIG_OK)
				goto loop_error;
		}
	}

	return CRTL_CONFIG_OK;

loop_error:
	if (newcfg) {
		crtl_config_free(_cfg);
		*cfg = NULL;
	}

	return ret;
}

/**
 * \brief              crtl_config_readfile() opens and reads the file and populates the
 *                     entire content to cfg handle
 *
 * \param filename     name of file to open and load
 * \param cfg          pointer to config handle.
 *                     If not NULL a handle created with crtl_config_new() must be given.
 *                     If cfg is NULL a new one is created and saved to cfg.
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
_api int crtl_config_readfile(const char *filename, struct crtl_config_struct **cfg)
{
	FILE      *fp  = NULL;
	int  ret = CRTL_CONFIG_OK;

	if ( !filename || !cfg || (*cfg && ((*cfg)->initnum != CRTL_CONFIG_INIT_MAGIC)) )
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	if ((fp = fopen(filename, "r")) == NULL)
		return CRTL_CONFIG_ERR_FILE;

	ret = crtl_config_read(fp, cfg);

	fclose(fp);

	return ret;
}

/**
 * \brief              crtl_config_print() prints all cfg content to the stream
 *
 * \param cfg          config handle
 * \param stream       stream to print
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
_api int crtl_config_print(const struct crtl_config_struct *cfg, FILE *stream)
{
	__crtl_config_section  *sect = NULL;
	__crtl_config_key_value *kv   = NULL;

	if (!cfg || !stream)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	TAILQ_FOREACH(sect, &cfg->sect_list, next) {
		if (sect->name)
			fprintf(stream, "[%s]\n", sect->name);

		TAILQ_FOREACH(kv, &sect->kv_list, next)
			fprintf(stream, "%s=%s\n", kv->key, kv->value);

		fprintf(stream, "\n");
	}

	return CRTL_CONFIG_OK;
}

/**
 * \brief              crtl_config_printtofile() prints (saves) all cfg content to the file
 *
 * \param cfg          config handle
 * \param filename     filename to save in
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
_api int crtl_config_printtofile(const struct crtl_config_struct *cfg, char *filename)
{
	FILE      *fp  = NULL;
	int  ret = CRTL_CONFIG_OK;

	if (!cfg || !filename)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	if ((fp = fopen(filename, "wb")) == NULL)
		return CRTL_CONFIG_ERR_FILE;

	ret = crtl_config_print(cfg, fp);

	fclose(fp);

	return ret;
}

/**
 * \brief              crtl_config_printsettings() prints settings to the stream
 *
 * \param cfg          config handle
 * \param stream       stream to print
 *
 * \return             Returns CRTL_CONFIG_RET_OK as success, otherwise is an error.
 */
_api int crtl_config_printsettings(const struct crtl_config_struct *cfg, FILE *stream)
{
	if (!cfg || !stream)
		return CRTL_CONFIG_ERR_INVALID_PARAM;

	fprintf(stream, "\n");
	fprintf(stream, "Configuration settings: \n");
	fprintf(stream, "   Comment characters : %s\n", cfg->comment_chars);
	fprintf(stream, "   Key-Value seperator: %c\n", cfg->keyval_sep);
	fprintf(stream, "   True-False strings : %s-%s\n", cfg->true_str, cfg->false_str);
	fprintf(stream, "\n");

	return CRTL_CONFIG_OK;
}



