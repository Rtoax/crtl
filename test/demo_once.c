#include <crtl/crtl_log.h>
#include <crtl/easy/once.h>

DECLARE_ONCE_FUN(rong);



int main()
{
    unsigned int times = 0;
    ONCE_RUN(&times, rong);
    ONCE_RUN(&times, rong);
    ONCE_RUN(&times, rong);

    
    pthread_once_t times_thread = PTHREAD_ONCE_INIT;
    ONCE_RUN(&times_thread, rong);
    ONCE_RUN(&times_thread, rong);
    ONCE_RUN(&times_thread, rong);
    
    return 0;
}


DEFINE_ONCE_FUN(rong)
{
    crtl_print_info("rongtao.\n");
}

