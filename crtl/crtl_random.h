#ifndef __CRTL_RANDOM_H
#define __CRTL_RANDOM_H 1

#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


int crtl_random_int(int min, int max);


#endif /*<__CRTL_RANDOM_H>*/


