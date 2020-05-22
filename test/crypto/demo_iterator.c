#include "crtl/iterator.h"
#include "crtl/log.h"

#include "crypto/iterator/types.h"

struct text {
    int i;
};

struct text T[4] = {1,2,3,4};


crtl_data_t test_begin(struct crtl_iterator_struct *ite)
{
    crtl_print_info("\n");
    return &T[0];
}
crtl_data_t test_next(struct crtl_iterator_struct *ite)
{
    crtl_print_info("\n");
    return &T[1];
}
crtl_data_t test_prev(struct crtl_iterator_struct *ite)
{
    crtl_print_info("\n");
    return &T[2];
}
crtl_data_t test_end(struct crtl_iterator_struct *ite)
{
    crtl_print_info("\n");
    return &T[3];
}


struct crtl_iterator_operations operations = {
    .begin = test_begin,
    .next = test_next,
    .prev = test_prev,
    .end = test_end,
};



void demo_iterator_crypto_test1()
{
    struct crtl_iterator_struct *iter = __crtl_iterator_init(&T);
    __crtl_iterator_register_operations(iter, &operations);

    struct text *i = iter->operations->begin(iter);
    crtl_print_info("begin: %d\n", i->i);

    i = iter->operations->next(iter);
    crtl_print_info("next: %d\n", i->i);

    
    i = iter->operations->prev(iter);
    crtl_print_info("prev: %d\n", i->i);
    
    i = iter->operations->end(iter);
    crtl_print_info("end: %d\n", i->i);

}


int main()
{
    demo_iterator_crypto_test1();
    
    return 0;
}
