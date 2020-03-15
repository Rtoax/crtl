#include "crtl/crtl_terminal.h"


_api void crtl_terminal_size(int *const x, int *const y)
{
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);

    *x	= sz.ws_col;
    *y	= sz.ws_row;
    
    return ;
}

