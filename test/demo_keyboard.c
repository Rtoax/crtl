#include "crtl/task.h"
#include "crtl/keyboard.h"


void demo_getch_test1()
{
    char ch = 'a';
    printf("\nPress 'q' to quit.\n");
    while(ch != '\n')
    {
        ch = crtl_keyboard_syncgetch();
            
        printf(">> %c (%d)\n", ch, ch);
    }
}

void *demo_multithread_getch_thread1(void*arg)
{
    
    char ch = 'a';
    printf("\nPress 'q' to quit.\n");
    while(ch != '\n')
    {
        ch = crtl_keyboard_syncgetch();
            
        printf("thread1>> %c (%d)\n", ch, ch);
    }
}

void *demo_multithread_getch_thread2(void*arg)
{
    while(1)
    {
        sleep(1);
        printf("thread2>> \n");
    }
}


void demo_multithread_getch()
{
    crtl_thread_t thread, thread2;
    crtl_thread_normal(&thread, demo_multithread_getch_thread1, NULL);
    crtl_thread_normal(&thread2, demo_multithread_getch_thread2, NULL);

    while(1)
    {
        sleep(1);
    }
    crtl_thread_cancel(thread);
    crtl_thread_cancel(thread2);
}

int main()
{
//    demo_getch_test1();
    demo_multithread_getch();


    
    return 0;
}
