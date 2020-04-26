#ifndef __CRTL_BITS_TYPES_BUFFER_H
#define __CRTL_BITS_TYPES_BUFFER_H 1

#include "crtl/bits/crtl_llist.h"


struct crtl_buffer_node_struct {
    union {
		struct crtl_buffer_node_struct *next;
		struct llist_node free;
	};
    int                 used;
    int                 size;
    int                 commit;
    int                 read;
	int                 flags;
    /* Data points here */
    unsigned long       data[0];
};

typedef struct llist_head *crtl_buffer_head_t;



crtl_buffer_head_t crtl_buffer_head_init(void);
void *crtl_buffer_node_new(crtl_buffer_head_t buf_head, size_t size);
void *crtl_buffer_node_grow(void *buf, size_t size);
void crtl_buffer_free(crtl_buffer_head_t buf_head);







#endif /*<__CRTL_BITS_TYPES_BUFFER_H>*/
