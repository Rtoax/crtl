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



/**
 * get terminal size
 * @param x: col size of terminal
 * @param y: row size of terminal
 * @return void
 */
_api void crtl_terminal_size(int *const x, int *const y);

/**
 * locate cursor to a very terminal position
 * @param x: col position of terminal
 * @param y: row position of terminal
 * @return void
 */
_api void crtl_terminal_gotoxy(int x, int y);

/**
 * show progress bar in terminal position
 * @param posx: col position of terminal
 * @param posy: row position of terminal
 * @param iTotal: current progress
 * @param Total: total
 * @param colorful: have color
 * @return error number
 *
 * Such as:
 *  [##############################           ] 52%
 *  [ 52%                                     ]
 */
_api int crtl_terminal_progress_bar(int posx, int posy, const int iTotal, const int Total, int colorful);


#endif //__CRTL_TERMINAL_H


