#include "crtl/error.h"
#include <crtl/log.h>

int main()
{
    int i;
    for(i=0;i<80;i++)
        printf("%d: %s, \t%s\n", i, crtl_err_name(i), crtl_strerror(i));
}
