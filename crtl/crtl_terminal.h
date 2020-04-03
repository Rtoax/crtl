#ifndef __CRTL_TERMINAL_H
#define __CRTL_TERMINAL_H 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#if HAVE_CURSES
#include <curses.h>
#endif
#include <time.h>
#include <math.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "crtl/easy/attribute.h"


_api void crtl_terminal_size(int *const x, int *const y);
_api void crtl_terminal_gotoxy(int x, int y);
_api int crtl_terminal_progress_bar(int posx, int posy, const int iTotal, const int Total, int colorful);


#endif //__CRTL_TERMINAL_H


