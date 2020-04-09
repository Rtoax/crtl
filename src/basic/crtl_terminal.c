#include <stdio.h>
#include <sys/ioctl.h>

#include "crtl/crtl_terminal.h"
#include "crtl/crtl_file.h"
#include "crtl/crtl_lock.h"



_api void crtl_terminal_size(int *const x, int *const y)
{
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);

    *x	= sz.ws_col;
    *y	= sz.ws_row;
    
    return ;
}


_api void crtl_terminal_gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}



_api int crtl_terminal_progress_bar(int posx, int posy, const int iTotal, const int Total, int colorful)
{
    int percent, ic=0, i, itotal, total;
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);
    int term_x = sz.ws_col - posx;
    
    if(term_x < 15)
    {
        return -1;
    }
    else
    {
        term_x -= 10;
        itotal = (int)(iTotal*1.0/Total*term_x);
        total  = term_x;
    }

    crtl_fd_lock(fileno(stdout));
    
    crtl_terminal_gotoxy(posx, posy);
    printf("[");
    ic++;
    for(percent=0; percent<=total; percent++)
    {
        if(percent<=itotal)
        {
            if(colorful)
                printf("\033[47m \033[m");   
            else
                printf("#"); 
        }
        else
        {
            printf(" ");
        }
        ic++;
    }
    if(colorful) {
        printf("]");
        ic++;
        for(i=0; i<ic-1; i++) { printf("\b"); }
        ic = 1;
        printf("\033[1;47m%3d%%\033[m", (int)(itotal*100.0/total));
        ic += 4;
    } else {
        printf("]%3d%%", (int)(itotal*100.0/total));
        ic += 5;
    }
    fflush(stdout);
    for(i=0; i<ic; i++) { printf("\b"); }

    
    crtl_fd_unlock(fileno(stdout));
    
    return 0;
    
}

