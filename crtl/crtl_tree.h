#ifndef __CRTL_TREE_H
#define __CRTL_TREE_H 1


#include "crtl/crtl_types.h"

#include "crtl/bits/crtl_tree_rbtree.h"
#include "crtl/bits/crtl_tree_huffman.h"



typedef struct crtl_rbtree_struct*          crtl_rbtree_t;
typedef struct crtl_rbtree_node_struct      crtl_rbtree_node_t;
typedef struct crtl_rbtree_iterator_struct* crtl_rbtree_iterator_t;



#define CRTL_RBTREE_DATA(crtl_rbtree_node) (void*)((struct crtl_rbtree_node_struct*)(crtl_rbtree_node))->data


crtl_rbtree_t crtl_rbtree_init(int (*cmp)(const void *, const void *), int (*display)(const void *));
crtl_rbtree_t crtl_rbtree_init_alloc(int (*cmp)(const void *, const void *), int (*display)(const void *));

int crtl_rbtree_insert(crtl_rbtree_t rbtree, void *data, unsigned int data_size);

crtl_rbtree_node_t *crtl_rbtree_search(crtl_rbtree_t rbtree, const void *data);

int crtl_rbtree_delete(crtl_rbtree_t rbtree, const void *data);

int crtl_rbtree_nnode(const crtl_rbtree_t rbtree);
int crtl_rbtree_is_empty(const crtl_rbtree_t rbtree);

crtl_rbtree_iterator_t crtl_rbtree_iterator(crtl_rbtree_t rbtree);
#define CRTL_RBTREE_ITER_FIRST(p_iter) __CRTL_RBTREE_ITER_FIRST(p_iter) 
#define CRTL_RBTREE_ITER_NEXT(p_iter)  __CRTL_RBTREE_ITER_NEXT(p_iter) 
#define CRTL_RBTREE_ITER_PREV(p_iter) __CRTL_RBTREE_ITER_PREV(p_iter)  
#define CRTL_RBTREE_ITER_LAST(p_iter)   __CRTL_RBTREE_ITER_LAST(p_iter)  

crtl_rbtree_node_t* crtl_rbtree_getfirst(crtl_rbtree_t rbtree);
crtl_rbtree_node_t* crtl_rbtree_getlast(crtl_rbtree_t rbtree);
crtl_rbtree_node_t* crtl_rbtree_getnext(crtl_rbtree_node_t* node);
crtl_rbtree_node_t* crtl_rbtree_getprev(crtl_rbtree_node_t* node);


int crtl_rbtree_destroy(crtl_rbtree_t rbtree);





#endif /*<__CRTL_TREE_H>*/
