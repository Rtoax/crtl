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


#endif /*<__CRTL_INIT_H>*/
