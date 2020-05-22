#include <crtl/iterator.h>

#include <crtl/log.h>



struct iterator_text {
    int i;
};

struct iterator_text Text[] = {
    1,4,3,5,6,7,
};
#define TESTNUM (sizeof(Text)/sizeof(Text[0]))

struct iterator_text *test_first(struct iterator_text *root)
{
    return &root[0];
}
struct iterator_text *test_next(struct iterator_text *root, struct iterator_text *current)
{
    struct iterator_text *text = current;
    return (text+1);
}
struct iterator_text *test_prev(struct iterator_text *root, struct iterator_text *current)
{
    struct iterator_text *text = current;
    return (text-1);
}
struct iterator_text *test_last(struct iterator_text *root)
{
    return &root[TESTNUM-1];
}


void demo_iterator_test1()
{
    
}


int main()
{
    demo_iterator_test1();
    
    return 0;
}





