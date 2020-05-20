#include <string.h>
#include "crtl/bits/core.h"
#include "crtl/easy/attribute.h"
#include "crtl/bits/types_basic.h"
#include "crtl/easy/macro.h"
#include "crtl/assert.h"
#include "crtl/log.h"



/* get enviroment */
char* crtl_getenv(char *key)
{
    return (char*)getenv(key);
}


int crtl_getenv_safe(const char* name, char* buffer, size_t* size)
{
  char* var;
  size_t len;

  if (name == NULL || buffer == NULL || size == NULL || *size == 0)
    return CRTL_ERROR;

  var = getenv(name);

  if (var == NULL)
    return CRTL_ERROR;

  len = strlen(var);

  if (len >= *size) {
    *size = len + 1;
    return CRTL_ERROR;
  }

  memcpy(buffer, var, len + 1);
  *size = len;

  return CRTL_SUCCESS;
}


/* set enviroment */
int crtl_setenv(const char *name, const char *value, int overwrite)
{
    if(unlikely(!name) || unlikely(!value))
    {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    int ret = setenv(name, value, overwrite);
    if(ret == 0)
        ret = CRTL_SUCCESS;
    else
        ret = CRTL_ERROR;

    return ret;
}

/* unset enviro */
int crtl_unsetenv(const char *name)
{
    if(unlikely(!name))
    {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    int ret = unsetenv(name);
    if(ret == 0)
        ret = CRTL_SUCCESS;
    else
        ret = CRTL_ERROR;

    return ret;
}



