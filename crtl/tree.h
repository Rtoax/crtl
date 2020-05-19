#ifndef __CRTL_TREE_H
#define __CRTL_TREE_H 1




#include "crtl/bits/types_basic.h"

#include "crtl/bits/crtl_tree_avl.h"
#include "crtl/bits/crtl_tree_splay.h"
#include "crtl/bits/crtl_tree_huffman.h"



typedef void*   crtl_rbtree_t;
typedef void*   crtl_rbtree_node_t;


/**
 * init a red black tree
 * @param cmp: data compare function
 * @return red black tree HANDLE or NULL
 */
crtl_rbtree_t crtl_rbtree_init(int (*cmp)(const void *, const void *));

/**
 * init a red black tree, the data when insert will be allocation memory
 * @param cmp: data compare function
 * @return red black tree HANDLE or NULL
 */
crtl_rbtree_t crtl_rbtree_init_alloc(int (*cmp)(const void *, const void *));

/**
 * insert data into a red black tree
 * @param rbtree: red black tree HANDLE
 * @param data: data
 * @param data_size: length of data
 * @return error number
 */
int crtl_rbtree_insert(crtl_rbtree_t rbtree, void *data, unsigned int data_size);


/**
 * search data from a red black tree
 * @param rbtree: red black tree HANDLE
 * @param data: data
 * @return rb tree node, crtl_rbtree_node_t
 */
crtl_rbtree_node_t crtl_rbtree_search(crtl_rbtree_t rbtree, const void *data);


/**
 * delete data into a red black tree
 * @param rbtree: red black tree HANDLE
 * @param data: data
 * @return error number
 */
int crtl_rbtree_delete(crtl_rbtree_t rbtree, const void *data);



/**
 * get number of node in red black tree
 * @param rbtree: red black tree HANDLE
 * @return number of node in red black tree
 */
int crtl_rbtree_nnode(const crtl_rbtree_t rbtree);

/**
 * red black tree is empty or not
 * @param rbtree: red black tree HANDLE
 * @return error number
 */
int crtl_rbtree_is_empty(const crtl_rbtree_t rbtree);


/**
 * get first data from a red black tree
 * @param rbtree: red black tree HANDLE
 * @return rb tree node, crtl_rbtree_node_t or NULL
 */
crtl_rbtree_node_t crtl_rbtree_getfirst(crtl_rbtree_t rbtree);

/**
 * get last data from a red black tree
 * @param rbtree: red black tree HANDLE
 * @return rb tree node, crtl_rbtree_node_t or NULL
 */
crtl_rbtree_node_t crtl_rbtree_getlast(crtl_rbtree_t rbtree);

/**
 * get next data from a red black tree
 * @param node: red black node
 * @return rb tree node, crtl_rbtree_node_t or NULL
 */
crtl_rbtree_node_t crtl_rbtree_getnext(crtl_rbtree_node_t node);

/**
 * get last data from a red black tree
 * @param node: red black node
 * @return rb tree node, crtl_rbtree_node_t or NULL
 */
crtl_rbtree_node_t crtl_rbtree_getprev(crtl_rbtree_node_t node);



/**
 * get data from red black tree node
 * @param node: red black node
 * @return node data
 */
void* crtl_rbtree_node_data(crtl_rbtree_node_t node);



/**
 * destroy red black tree
 * @param rbtree: red black tree HANDLE
 * @return error number
 */
int crtl_rbtree_destroy(crtl_rbtree_t rbtree);



#endif /*<__CRTL_TREE_H>*/

