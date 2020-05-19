#include "crtl/keyboard.h"
#include "crtl/assert.h"



/* API */
/*https://blog.csdn.net/Timsley/article/details/51424068*/
_api char crtl_keyboard_syncgetch()
{
#define _DEV_TTY    "/dev/tty"
	FILE *input;
    FILE *output;
    struct termios initial_settings, new_settings;
    int inputString;

    input = fopen(_DEV_TTY, "r");
    output = fopen(_DEV_TTY, "w");
    if(!input || !output)
    {
        crtl_print_err("Unable to open %s\n", _DEV_TTY);
        crtl_assert_fp(stderr, 0);
//        exit(0);
        return -1;
    }

    tcgetattr(fileno(input), &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_lflag &= ~ISIG;

    if(tcsetattr(fileno(input), TCSANOW, &new_settings) != 0)
    {
        crtl_print_err("Could not set attributes\n");
        fclose(input);
        fclose(output);
        crtl_assert_fp(stderr, 0);
//        exit(0);
        return -1;
    }
    
    //while(inputString != 'q')
    //{
        //do
        //{
            inputString = fgetc(input);
        //}while(inputString == '\n' || inputString == '\r');
        //return (char )inputString;
    //}

    tcsetattr(fileno(input), TCSANOW, &initial_settings);
	
    fclose(input);
    fclose(output);
    
	return (char)inputString;
}

