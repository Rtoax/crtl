#ifndef __CRTL_BITS_ENV_H
#define __CRTL_BITS_ENV_H 1

#include <stdlib.h>

    
/**
 * get an environment variable
 *  The  getenv() function searches the environment 
 *  list to find the environment variable name, and returns a
 *  pointer to the corresponding value string.
 *
 * @param name: name to get
 * @return error number
 */
char* crtl_getenv(char *name);

/**
 * get an environment variable
 *  The  getenv() function searches the environment 
 *  list to find the environment variable name, and returns a
 *  pointer to the corresponding value string.
 *
 * @param name: name to get
 * @param buffer: environment variable buffer
 * @param size: environment variable buffer size
 * @return error number
 */
int crtl_getenv_safe(const char* name, char* buffer, size_t* size);

/**
 * change or add an environment variable
 *  adds the variable name to the environment with the value value
 *
 * @param name: name to get
 * @param value: adds the variable name to the environment with the value value
 * @param overwrite: If name does exist in the environment, then its value is changed to value .
 *                   if overwrite is nonzero;  if overwrite is zero, then the value of name is not changed
 * @return error number
 */
int crtl_setenv(const char *name, const char *value, int overwrite);

/**
 * deletes the variable name from the environment
 *
 * @param name: name to get
 * @return error number
 */
int crtl_unsetenv(const char *name);

#endif /*<__CRTL_BITS_ENV_H>*/