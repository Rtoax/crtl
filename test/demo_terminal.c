#include "crtl/crtl_terminal.h"
#include "crtl/crtl_log.h"


void demo_terminal_size()
{
    int x, y;
    crtl_terminal_size(&x, &y);

    crtl_print_info("%d,%d\n", x, y);
}


void demo_terminal_progress_bar()
{
    int x, y;
    crtl_terminal_progress_bar(10, 10, 12, 23, 0);
}

int main()
{
    demo_terminal_size();
    demo_terminal_progress_bar();

    return 0;
}

