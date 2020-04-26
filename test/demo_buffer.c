#include "crtl/bits/crtl_types_buffer.h"
#include <crtl/crtl_log.h>

struct test_data {
    int a;
    char data[0];
};

static void demo_1_create_buffer_test()
{
    crtl_buffer_head_t head = crtl_buffer_head_init();
    
    struct test_data *data = crtl_buffer_node_new(head, sizeof(struct test_data));    
    data = crtl_buffer_node_grow(data, sizeof(struct test_data)*2);

    data = crtl_buffer_node_new(head, sizeof(struct test_data));  
    data = crtl_buffer_node_new(head, sizeof(struct test_data));  
    data = crtl_buffer_node_new(head, sizeof(struct test_data));  
    data = crtl_buffer_node_new(head, sizeof(struct test_data)); 
    data = crtl_buffer_node_new(head, sizeof(struct test_data)); 
    data = crtl_buffer_node_new(head, sizeof(struct test_data)); 
    data = crtl_buffer_node_new(head, sizeof(struct test_data)); 
    data = crtl_buffer_node_new(head, sizeof(struct test_data));  


    crtl_buffer_free(head);
}


int main()
{
    demo_1_create_buffer_test();

    return 0;
}
