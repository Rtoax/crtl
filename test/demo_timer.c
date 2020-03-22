#include <crtl/crtl_tree.h>
#include <crtl/crtl_log.h>
#include <crtl/crtl_timer.h>


void timer_callback(void * arg)
{
    
}

void demo_timer_create_test()
{
   long timerid = crtl_timer_create(crtl_true, &timer_callback, NULL, 1, 1000);

   crtl_print_info("timerid = %d\n", timerid);
}


int main()
{
    demo_timer_create_test();

    return 0;
}


