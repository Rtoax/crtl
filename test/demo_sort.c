#include <string.h>
#include "crtl/crtl_sort.h"
#include <crtl/crtl_mask.h>
#include <crtl/crtl_log.h>


static int demo_sort_test_cmp(const void *d1, const void *d2)
{
    const int *dd1 = d1;
    const int *dd2 = d2;
    
    if(*dd1 > *dd2) return 1;
    else if(*dd1 == *dd2) return 0;
    else if(*dd1 < *dd2) return -1;

    return 0;
}

#define ARRAY_INT {1,2,1,6,4,3,8,7,9,12,356,35,6,345,63,56,345,63,546,3,56,35,6,35,63,\
                    56,35,6,4567,5,678,567,8,576,8,12,312,13,41,34,12,312,3,23,23,\
                    42,435,3,456,345,2,45,1234,52,345,2345234,52,435,24,52,451,\
                    34,13,4,13,41,3,41,34,1,34,13,42,345,34,56,435,64,5673,12,\
                    31,23,123,12,323,4,34,5,4356,45,6546,2435,3,456,45,7,134,13,41,34,13,413,41,34,13,4};


void demo_sort_test1()
{
    int a[] = ARRAY_INT;

    crtl_sort_cocktailshaker(a, sizeof(a)/sizeof(a[0]), sizeof(a[0]), demo_sort_test_cmp);

    int i;
    for(i=0;i<sizeof(a)/sizeof(a[0]); i++)
    {
        printf("%d - ", a[i]);
    }
    printf("\n");
    
}
void demo_sort_test2()
{
    int a[] = ARRAY_INT;

    crtl_sort_heap(a, sizeof(a)/sizeof(a[0]), sizeof(a[0]), demo_sort_test_cmp);

    int i;
    for(i=0;i<sizeof(a)/sizeof(a[0]); i++)
    {
        printf("%d - ", a[i]);
    }
    printf("\n");
    
}


void demo_sort_test3()
{
    int a[] = ARRAY_INT;

    crtl_sort_insertion(a, sizeof(a)/sizeof(a[0]), sizeof(a[0]), demo_sort_test_cmp);

    int i;
    for(i=0;i<sizeof(a)/sizeof(a[0]); i++)
    {
        printf("%d - ", a[i]);
    }
    printf("\n");
    
}

void demo_sort_test4()
{
    int a[] = ARRAY_INT;

    crtl_sort_qsort3way(a, sizeof(a)/sizeof(a[0]), sizeof(a[0]), demo_sort_test_cmp);

    int i;
    for(i=0;i<sizeof(a)/sizeof(a[0]); i++)
    {
        printf("%d - ", a[i]);
    }
    printf("\n");
    
}
void demo_sort_test5()
{
    int a[] = ARRAY_INT;

    crtl_sort_qsortH(a, 10, sizeof(a)/sizeof(a[0])-10, sizeof(a[0]), demo_sort_test_cmp);
    
    int i;
    for(i=0;i<sizeof(a)/sizeof(a[0]); i++)
    {
        printf("%d - ", a[i]);
    }
    printf("\n");
    
}

void demo_sort_test6()
{
    int a[] = ARRAY_INT;

    crtl_sort_qsortL(a, 10, sizeof(a)/sizeof(a[0])-10, sizeof(a[0]), demo_sort_test_cmp);
    
    int i;
    for(i=0;i<sizeof(a)/sizeof(a[0]); i++)
    {
        printf("%d - ", a[i]);
    }
    printf("\n");
    
}

void demo_sort_test7()
{
    int a[] = ARRAY_INT;

    crtl_sort_selection(a, sizeof(a)/sizeof(a[0]), sizeof(a[0]), demo_sort_test_cmp);

    int i;
    for(i=0;i<sizeof(a)/sizeof(a[0]); i++)
    {
        printf("%d - ", a[i]);
    }
    printf("\n");
    
}

void demo_sort_test8()
{
    int a[] = ARRAY_INT;

    crtl_sort_shell(a, sizeof(a)/sizeof(a[0]), sizeof(a[0]), demo_sort_test_cmp);

    int i;
    for(i=0;i<sizeof(a)/sizeof(a[0]); i++)
    {
        printf("%d - ", a[i]);
    }
    printf("\n");
    
}


int main()
{
    demo_sort_test1();
    demo_sort_test2();
    demo_sort_test3();
    demo_sort_test4();
    demo_sort_test5();
    demo_sort_test6();
    demo_sort_test7();
    demo_sort_test8();
    
    return 0;
}



