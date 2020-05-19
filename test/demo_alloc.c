#include <crtl/assert.h>
#include <crtl/log.h>
#include <crtl/alloc.h>

void demo_alloc_test_malloc1()
{
    int * I = crtl_malloc1(10, sizeof(int));
    int i=0;
    for(i=0;i<10;i++) {
        I[i] = i+1;
    }
    for(i=0;i<10;i++) {
        printf("%d\n", I[i]);
    }
    I = crtl_remalloc1(I, 20, sizeof(int));
    for(i=10;i<20;i++) {
        I[i] = i+100;
    }
    for(i=0;i<20;i++) {
        printf("%d\n", I[i]);
    }
    crtl_mfree1(I);
}

void demo_alloc_test_malloc2()
{
    int ** I = (int **)crtl_malloc2(3, 4, sizeof(int));
    int i=0, j=0;
    for(i=0;i<4;i++) {
        for(j=0;j<3;j++) {
            I[i][j] = i*10+j;
        }
    }
    for(i=0;i<4;i++) {
        for(j=0;j<3;j++) {
            printf("(%d,%d)%2d  ", i, j, I[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //这样使用时危险的，原来的数值会变动
    //(0,0) 0  (0,1) 1  (0,2) 2  
    //(1,0)10  (1,1)11  (1,2)12  
    //(2,0)20  (2,1)21  (2,2)22  
    //(3,0)30  (3,1)31  (3,2)32  
    //
    //(0,0) 0  (0,1)11  (0,2)22  (0,3)60  
    //(1,0) 1  (1,1)12  (1,2)30  (1,3)61  
    //(2,0) 2  (2,1)20  (2,2)31  (2,3)134961  
    //(3,0)10  (3,1)21  (3,2)32  (3,3)63  
    //(4,0)11  (4,1)22  (4,2)60  (4,3)80
    I = (int **)crtl_remalloc2((void **)I, 4, 5, sizeof(int));
    for(i=0;i<5;i++) {
        for(j=0;j<4;j++) {
            if(I[i][j] == 0)
                I[i][j] = i*20+j;
        }
    }
    for(i=0;i<5;i++) {
        for(j=0;j<4;j++) {
            printf("(%d,%d)%2d  ", i, j, I[j][i]);
        }
        printf("\n");
    }
    printf("\n");
    
    crtl_mfree2((void **)I);
}
int main()
{
    demo_alloc_test_malloc1();
    demo_alloc_test_malloc2();

    

    return 0;
}



