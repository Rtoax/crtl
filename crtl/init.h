#ifndef __CRTL_INIT_H
#define __CRTL_INIT_H 1


#include "crtl/bits/types_basic.h"

/**
 *  crtl library initializer function
 *  @param argc: argument count
 *  @param argv: argument values
 *              int main(int argc, char *argv[]);
 *  @return error number
 */
int crtl_init(int argc, char **argv);


/**
 *  crtl library exit function
 *  @param status: CRTL_SUCCESS, CRTL_ERROR
 *  @return error number
 */
void crtl_exit(int status);



#endif /*<__CRTL_INIT_H>*/
