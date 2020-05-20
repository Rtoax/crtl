#include <math.h>

#ifndef ARRAY_IDX 
#define ARRAY_IDX(type, startAddr, endAddr) (abs(((unsigned long)endAddr)-((unsigned long)startAddr))/sizeof(type))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

