#include <crtl/assert.h>
#include <crtl/log.h>



int main()
{
    crtl_print_emerg("assert:\n");
    crtl_assert_fp(stderr, 1==0);
    
    crtl_print_emerg("assert:\n");
    crtl_assert_fp(stderr, 1==1);

    return 0;
}


