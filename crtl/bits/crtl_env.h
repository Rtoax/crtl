#ifndef __CRTL_BITS_ENV_H
#define __CRTL_BITS_ENV_H 1

#include <stdlib.h>


char* crtl_getenv(char *key);
int crtl_getenv_safe(const char* name, char* buffer, size_t* size);
int crtl_setenv(const char *name, const char *value, int overwrite);
int crtl_unsetenv(const char *name);

#endif /*<__CRTL_BITS_ENV_H>*/