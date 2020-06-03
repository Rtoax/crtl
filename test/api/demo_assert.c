#include <crtl/assert.h>
#include <crtl/log.h>


void demo_assert_fp()
{
    crtl_print_emerg("assert:\n");
    crtl_assert_fp(stderr, 1==0);
    
    crtl_print_emerg("assert:\n");
    crtl_assert_fp(stderr, 1==1);
}


void demo_assert_print()
{
    crtl_assert_print(1==0, "Hello %s-%d\n", "World", 1024);
}

int main()
{
//    demo_assert_fp();
    demo_assert_print();

    
    return 0;
}


