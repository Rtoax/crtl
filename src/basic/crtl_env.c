#include <string.h>
#include "crtl/bits/crtl_env.h"
#include "crtl/easy/attribute.h"
#include "crtl/crtl_types.h"
#include "crtl/crtl_assert.h"



/* get enviroment */
char* crtl_getenv(char *key)
{
    return (char*)getenv(key);
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

