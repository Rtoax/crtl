#include "crtl/crtl_keyboard.h"


void demo_getch()
{
    char ch = 'a';
    printf("\nPress 'q' to quit.\n");
    while(ch != '\n')
    {
        ch = crtl_keyboard_syncgetch();
            
        printf(">> %c (%d)\n", ch, ch);
    }
}

int main()
{
    demo_getch();

    return 0;
}
