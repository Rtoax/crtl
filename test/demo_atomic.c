#include <crtl/bits/crtl_atomic.h>
#include "crtl/bits/crtl_refcount.h"

#include <crtl/crtl_log.h>



int main()
{
    crtl_atomic_t atomic = CRTL_ATOMIC_INITIALIZER;
    
    crtl_atomic_inc(&atomic);

    printf("%d\n", atomic.val);
    crtl_atomic_inc(&atomic);

    printf("%d\n", atomic.val);

    crtl_atomic_set(&atomic, CRTL_REFCOUNT_MAX);

    printf("%d\n", atomic.val);
    
    return 0;
}



