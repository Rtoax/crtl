#include "crtl/random.h"
#include "crtl/log.h"

void demo_random_int()
{
    int i;
    for (i=1;i<=100;i++)
    {
        printf("%4x ", crtl_random_int(1, 100));
        if(i%10==0)
            printf("\n");
    }
    printf("\n");
}

int main()
{
    demo_random_int();

    return 0;
}

