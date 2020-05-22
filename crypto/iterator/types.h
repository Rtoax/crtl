#ifndef __CRTL_CRYPTO_ITERATOR_TYPES_H
#define __CRTL_CRYPTO_ITERATOR_TYPES_H 1

#include <malloc.h>

#include "crtl/iterator.h"
#include "crtl/assert.h"
#include "crtl/log.h"


#include "crypto/attribute.h"
#include "crypto/align/container.h"
#include "crypto/expect.h"


struct crtl_iterator_struct;


struct crtl_iterator_operations {
    crtl_data_t (*begin)(struct crtl_iterator_struct *ite);
    crtl_data_t (*next)(struct crtl_iterator_struct *ite);
    crtl_data_t (*prev)(struct crtl_iterator_struct *ite);
    crtl_data_t (*end)(struct crtl_iterator_struct *ite);
};



struct crtl_iterator_struct {
    
    crtl_data_t root;
    crtl_data_t current_data;
    
    struct crtl_iterator_operations *operations;
};


struct crtl_iterator_struct* __crtl_iterator_init(crtl_data_t root);

int __crtl_iterator_register_operations(struct crtl_iterator_struct *iter, struct crtl_iterator_operations *operations);

//TODO


#endif /*<__CRTL_CRYPTO_ITERATOR_TYPES_H>*/
