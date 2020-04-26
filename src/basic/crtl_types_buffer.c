#include "crtl/crtl_assert.h"
#include "crtl/bits/crtl_types_buffer.h"


crtl_buffer_head_t crtl_buffer_head_init(void) 
{
    crtl_buffer_head_t head = malloc(sizeof(struct llist_head));
    crtl_assert(head);
    init_llist_head(head);
    return head;
}


void *crtl_buffer_node_new(crtl_buffer_head_t buf_head, size_t size)
{
    crtl_assert(buf_head);
    crtl_assert(size);
    struct crtl_buffer_node_struct *buffer = malloc(sizeof(struct crtl_buffer_node_struct)+size);
    crtl_assert(buffer);

    memset(buffer, 0x00, sizeof(struct crtl_buffer_node_struct)+size);
    
    buffer->size = size;

    llist_add(&buffer->free, buf_head);

    return (void*)buffer->data;
}

void *crtl_buffer_node_grow(void *buf, size_t size)
{
    crtl_assert(buf);
    crtl_assert(size);
    struct crtl_buffer_node_struct *buffer = container_of(buf, struct crtl_buffer_node_struct, data);
    crtl_assert(buffer);
    
    buffer = realloc(buffer, sizeof(struct crtl_buffer_node_struct)+size);
    crtl_assert(buffer);

    
    buffer->size = size;

    return (void*)buffer->data;
}



void crtl_buffer_free(crtl_buffer_head_t buf_head)
{
    struct llist_node *node;
    struct crtl_buffer_node_struct *entry, *next;

    node = buf_head->first;
    llist_for_each_entry_safe(entry, next, node, free) {
        __crtl_dbg("free.\n");
        free(entry);
        entry = next;
    }
}


