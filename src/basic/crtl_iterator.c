#include <malloc.h>
#include "crtl/assert.h"
#include "crtl/log.h"

#include "crtl/bits/types_basic.h"

#include "crypto/iterator/types.h"



//TODO



struct crtl_iterator_struct* __crtl_iterator_init(crtl_data_t root)
{
    if(unlikely(!root)) {
        crtl_print_err("null pointer error.\n");
        return NULL;
    }
    struct crtl_iterator_struct *iter = malloc(sizeof(struct crtl_iterator_struct));
    crtl_assert(iter);
    
    iter->root = root;
    iter->current_data = NULL;
    iter->operations = NULL;

    return iter;
}

int __crtl_iterator_register_operations(struct crtl_iterator_struct *iter, struct crtl_iterator_operations *operations)
{
    if(unlikely(!iter) || unlikely(!operations)) {
        crtl_print_err("null pointer error.\n");
        return CRTL_ERROR;
    }
    iter->operations = malloc(sizeof(struct crtl_iterator_operations));
    crtl_assert(iter->operations);

    memcpy(iter->operations, operations, sizeof(struct crtl_iterator_operations));

    return CRTL_SUCCESS;
}


