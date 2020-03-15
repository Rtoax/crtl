#ifndef __CRTL_ITERATOR_H
#define __CRTL_ITERATOR_H 1

#include "crtl/crtl_types.h"
#include "crtl/easy/attribute.h"



struct crtl_iterator_struct {
    void *root;
    
    void* (*start)();
    void* (*next)(void* data);
    void* (*prev)(void* data);
    void* (*end)();
    
    void* (*search)(void* data);
};

#define CRTL_ITERATOR_NAME      iterator
#define CRTL_ITERATOR_ELEMENT   struct crtl_iterator_struct CRTL_ITERATOR_NAME


#define crtl_iterator(TYPE, data) ({\
    struct crtl_iterator_struct *_iter = NULL;\
    _iter = (struct crtl_iterator_struct *)(data+offsetof(TYPE, CRTL_ITERATOR_NAME));\
    _iter;\
})

static struct crtl_iterator_struct _unused* crtl_iterator_gen(void *root, int iterator_pos,
                                                                 void* (*start)(), 
                                                                 void* (*next)(void* data), 
                                                                 void* (*prev)(void* data), 
                                                                 void* (*end)(),
                                                                 void* (*search)(void* data))
{
    struct crtl_iterator_struct *iterator = (struct crtl_iterator_struct *)(root+iterator_pos);
    iterator->start = start;
    iterator->next = next;
    iterator->prev = prev;
    iterator->end = end;
    iterator->search = search;
    return iterator;
}


#endif /*<__CRTL_ITERATOR_H>*/

