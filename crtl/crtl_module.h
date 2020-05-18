#ifndef __CRTL_MODULE_H
#define __CRTL_MODULE_H 1

#include "crtl/bits/crtl_module_types.h"


/**
 * create application
 * @param app_name: application name
 * @return error number
 */
_api int crtl_application_create(const char *app_name);


/**
 * destory application
 * @param app_name: application name
 * @return error number
 */
_api int crtl_application_destroy(const char *app_name);


#endif /*<__CRTL_MODULE_H>*/
