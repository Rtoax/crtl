#include <crypto/xarray.h>
#include <crtl/log.h>



void demo_xarray_test1()
{
    struct xtest {
        int i;
    }test[] = {1,2,3,4,5,6,7,8,9,10};

    struct xarray xa;
    xa_init(&xa);
    if(xa_empty(&xa)) {
        crtl_print_info("Is empty.\n");
    }
    unsigned long index;
    for(index=0;index<5;index++) {
        xa_insert(&xa, index, &test[index], 1);
    }
    for(;index<10;index++) {
        xa_store(&xa, index, &test[index], 1);
    }

    struct xtest *t1;
    xa_for_each(&xa, index, t1) {
        crtl_print_info("xa_for_each: %d\n", t1->i);
    }

    t1 = xa_load(&xa, 3);
    crtl_print_info("xa_load: %d\n", t1->i);

    xa_erase(&xa, 3);
    
    xa_for_each(&xa, index, t1) {
        crtl_print_info("xa_for_each: %d\n", t1->i);
    }
    
    xa_reserve(&xa, 4, 0);
    xa_reserve(&xa, 5, 0);
    xa_reserve(&xa, 6, 0);

    xa_for_each(&xa, index, t1) {
        crtl_print_info("xa_reserve: %d\n", t1->i);
    }


//    struct xtest (*Textract)[10];
//    xa_extract(&xa, (void**)Textract, 1, 9, 9, XA_MARK_0); //error
//    for(index=1;index<10;index++) {
//        crtl_print_info("xa_extract: %d\n", (*Textract[index]).i);
//    }

    XA_STATE(xas, &xa, 1);
    if(xas_invalid(&xas)) {
        crtl_print_info("Is xas_invalid.\n");
    }

    xas_reset(&xas);

    struct xtest *t2 = xas_load(&xas);
    crtl_print_info("xas_load: %d.\n", t2->i);
    t2 = xas_next_entry(&xas, 7);
    crtl_print_info("xas_next_entry: %d.\n", t2->i);
    t2 = xas_next_entry(&xas, 7);
    crtl_print_info("xas_next_entry: %d.\n", t2->i);
    t2 = xas_next_entry(&xas, 7);
    crtl_print_info("xas_next_entry: %d.\n", t2->i);

    t2 = xas_prev(&xas);
    crtl_print_info("xas_prev: %d.\n", t2->i);
    
    t2 = xas_next(&xas);
    crtl_print_info("xas_next: %d.\n", t2->i);


    

    xa_destroy(&xa);
}



void demo_xarray_test2()
{
    struct xtest {
        int i;
    }test[] = {1,2,3,4,5,6,7,8,9,10};
    
    struct xarray xa;
    xa_init(&xa);

    
    
}

int main()
{
//    demo_xarray_test1();
    demo_xarray_test2();
    
    
    return 0;
}

