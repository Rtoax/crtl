#ifndef __CRTL_TREE_H
#define __CRTL_TREE_H 1



#include "crtl/easy/attribute.h"

#include "crtl/bits/crtl_types_basic.h"

#include "crtl/bits/crtl_tree_avl.h"
#include "crtl/bits/crtl_tree_rbtree.h"
#include "crtl/bits/crtl_tree_splay.h"
#include "crtl/bits/crtl_tree_huffman.h"



typedef struct crtl_rbtree_struct*          crtl_rbtree_t;
typedef struct crtl_rbtree_node_struct      crtl_rbtree_node_t;
typedef struct crtl_rbtree_iterator_struct* crtl_rbtree_iterator_t;

/**
 * after you call "crtl_rbtree_iterator" get the red black tree's iterator
 * use the following macro to obtain data.
 *
 * @param p_iter: iterator of rbtree
 */
#define CRTL_RBTREE_ITER_FIRST(p_iter) __CRTL_RBTREE_ITER_FIRST(p_iter) 
#define CRTL_RBTREE_ITER_NEXT(p_iter)  __CRTL_RBTREE_ITER_NEXT(p_iter) 
#define CRTL_RBTREE_ITER_PREV(p_iter) __CRTL_RBTREE_ITER_PREV(p_iter)  
#define CRTL_RBTREE_ITER_LAST(p_iter)   __CRTL_RBTREE_ITER_LAST(p_iter)  


/**
 * after call "crtl_rbtree_search" or "crtl_rbtree_getxxxxx" get the red black tree's node
 *
 * @param crtl_rbtree_node: return of "crtl_rbtree_search" or "crtl_rbtree_getxxxxx"
 */
#define CRTL_RBTREE_DATA(crtl_rbtree_node) (void*)((struct crtl_rbtree_node_struct*)(crtl_rbtree_node))->data


/**
 * init a red black tree
 * @param cmp: data compare function
 * @param display: display function, not support yet
 * @return red black tree HANDLE or NULL
 */
_api crtl_rbtree_t crtl_rbtree_init(int (*cmp)(const void *, const void *), int (*display)(const void *));

/**
 * init a red black tree, the data when insert will be allocation memory
 * @param cmp: data compare function
 * @param display: display function, not support yet
 * @return red black tree HANDLE or NULL
 */
_api crtl_rbtree_t crtl_rbtree_init_alloc(int (*cmp)(const void *, const void *), int (*display)(const void *));

/**
 * insert data into a red black tree
 * @param rbtree: red black tree HANDLE
 * @param data: data
 * @param data_size: length of data
 * @return error number
 */
_api int crtl_rbtree_insert(crtl_rbtree_t rbtree, void *data, unsigned int data_size);


/**
 * search data from a red black tree
 * @param rbtree: red black tree HANDLE
 * @param data: data
 * @return rb tree node, crtl_rbtree_node_t
 */
_api crtl_rbtree_node_t *crtl_rbtree_search(crtl_rbtree_t rbtree, const void *data);


/**
 * delete data into a red black tree
 * @param rbtree: red black tree HANDLE
 * @param data: data
 * @return error number
 */
_api int crtl_rbtree_delete(crtl_rbtree_t rbtree, const void *data);



/**
 * get number of node in red black tree
 * @param rbtree: red black tree HANDLE
 * @return number of node in red black tree
 */
_api int crtl_rbtree_nnode(const crtl_rbtree_t rbtree);

/**
 * red black tree is empty or not
 * @param rbtree: red black tree HANDLE
 * @return error number
 */
_api int crtl_rbtree_is_empty(const crtl_rbtree_t rbtree);


/**
 * get red black tree's iterator
 * @param rbtree: red black tree HANDLE
 * @return red black tree's iterator, type crtl_rbtree_iterator_t
 *
 *  You can use "CRTL_RBTREE_ITER_XXXX" to polling the tree node.
 */
_api crtl_rbtree_iterator_t crtl_rbtree_iterator(crtl_rbtree_t rbtree);


/**
 * get first data from a red black tree
 * @param rbtree: red black tree HANDLE
 * @return rb tree node, crtl_rbtree_node_t or NULL
 */
_api crtl_rbtree_node_t* crtl_rbtree_getfirst(crtl_rbtree_t rbtree);

/**
 * get last data from a red black tree
 * @param rbtree: red black tree HANDLE
 * @return rb tree node, crtl_rbtree_node_t or NULL
 */
_api crtl_rbtree_node_t* crtl_rbtree_getlast(crtl_rbtree_t rbtree);

/**
 * get next data from a red black tree
 * @param node: red black node
 * @return rb tree node, crtl_rbtree_node_t or NULL
 */
_api crtl_rbtree_node_t* crtl_rbtree_getnext(crtl_rbtree_node_t* node);

/**
 * get last data from a red black tree
 * @param node: red black node
 * @return rb tree node, crtl_rbtree_node_t or NULL
 */
_api crtl_rbtree_node_t* crtl_rbtree_getprev(crtl_rbtree_node_t* node);


/**
 * destroy red black tree
 * @param rbtree: red black tree HANDLE
 * @return error number
 */
_api int crtl_rbtree_destroy(crtl_rbtree_t rbtree);



#endif /*<__CRTL_TREE_H>*/
