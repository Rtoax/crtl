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
    crtl_terminal_size(&x, &y);
    system("clear");
    crtl_terminal_progress_bar(1, y-2, 12, 23, 0);
    crtl_terminal_progress_bar(1, y-1, 12, 23, 1);
    crtl_terminal_gotoxy(x,y);
}


void demo_terminal_progress_bar_pro()
{
    int x, y;
    crtl_terminal_size(&x, &y);
    system("clear");

    int itotal, total = 10;
    for(itotal=0;itotal<=total;itotal++) {
        sleep(1);
        crtl_terminal_progress_bar(1, y-2, itotal, total, 0);
    }
    
    
//    crtl_terminal_progress_bar(1, y-1, 12, 23, 1);
    
    crtl_terminal_gotoxy(x,y);
}


int main()
{
    demo_terminal_size();
    demo_terminal_progress_bar();

    demo_terminal_progress_bar_pro();

    return 0;
}

