#ifndef __CRTL_KEYBOARD_H
#define __CRTL_KEYBOARD_H 1

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

#include "crtl/easy/attribute.h"


#define VK_LEFT 	(97)
#define VK_UP 		(119)
#define VK_RIGHT 	(100)
#define	VK_DOWN 	(115)
#define VK_TAB      (9)


/**
 * sync get charector from keyboard
 * @param void
 * @return -1 if failed, char if success.
 */
_api char crtl_keyboard_syncgetch();



#endif //__CRTL_KEYBOARD_H

