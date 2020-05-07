
#include "crtl/bits/crtl_core.h"
#include "crtl/crtl_log.h"

static void demo_core_test1()
{
    crtl_utsname_t buffer;
    crtl_uname(&buffer);

    crtl_print_info("machine = %s\n", buffer.machine);
    crtl_print_info("release = %s\n", buffer.release);
    crtl_print_info("sysname = %s\n", buffer.sysname);
    crtl_print_info("version = %s\n", buffer.version);

    int priority = -1;
    crtl_getpriority(crtl_getpid(), &priority);
    crtl_print_info("priority = %d\n", priority);

}


int main()
{
    demo_core_test1();

    return 0;
}

