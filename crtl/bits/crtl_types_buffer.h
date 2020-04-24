#ifndef __CRTL_BITS_TYPES_BUFFER_H
#define __CRTL_BITS_TYPES_BUFFER_H 1

#include "crtl/bits/crtl_llist.h"


struct crtl_buffer_struct {
    union {
		struct crtl_buffer_struct *next;
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


#endif /*<__CRTL_BITS_TYPES_BUFFER_H>*/
