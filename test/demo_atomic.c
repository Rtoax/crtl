#include <crypto/atomic/atomic.h>
#include "crypto/atomic/refcount.h"

#include <crtl/log.h>



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



