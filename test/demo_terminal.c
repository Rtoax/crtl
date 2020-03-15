#include "crtl/crtl_terminal.h"
#include "crtl/crtl_log.h"


void demo_terminal_size()
{
    int x, y;
    crtl_terminal_size(&x, &y);

    crtl_print_info("%d,%d\n", x, y);
}


int main()
{
    demo_terminal_size();

    return 0;
}

