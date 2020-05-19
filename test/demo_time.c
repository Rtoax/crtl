#include <crtl/tree.h>
#include <crtl/log.h>
#include <crtl/crtl_time.h>


void demo_time_addsub_test()
{
    struct timespec t1 = {10110, 11123};
    struct timespec t2 = {11332, 300004};
    struct timespec t3 = {0, 0};
    int cmp = crtl_timespec_subabs(&t1, &t2, &t3);
    if(cmp == CRTL_GT) {
        crtl_print_info("Greate: sec %ld, nanosec %ld\n", t3.tv_sec, t3.tv_nsec);
    } else if(cmp == CRTL_EQ) {
        crtl_print_info("Equal:  sec %ld, nanosec %ld\n", t3.tv_sec, t3.tv_nsec);
    } else if(cmp == CRTL_LT) {
        crtl_print_info("Little: sec %ld, nanosec %ld\n", t3.tv_sec, t3.tv_nsec);
    }
}


int main()
{
    demo_time_addsub_test();

    return 0;
}



