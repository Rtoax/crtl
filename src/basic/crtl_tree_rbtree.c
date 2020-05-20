#include <malloc.h>
#include <string.h>

#include "crtl/bits/types_basic.h"

#include "crtl/log.h"
#include "crtl/assert.h"

#include "crypto/tree/rbtree.h"
#include "crtl/easy/attribute.h"

#include "crypto/align/container.h"
#include "crypto/expect.h"

/*
  Red Black Trees
  (C) 1999  Andrea Arcangeli <andrea@suse.de>
  (C) 2002  David Woodhouse <dwmw2@infradead.org>
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  linux/lib/rbtree.c
*/

#ifndef EXPORT_SYMBOL
#define EXPORT_SYMBOL(v)
#endif

static void __rb_rotate_left(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *right = node->rb_right;
	struct rb_node *parent = rb_parent(node);

	if ((node->rb_right = right->rb_left))
		rb_set_parent(right->rb_left, node);
	right->rb_left = node;

	rb_set_parent(right, parent);

	if (parent)
	{
		if (node == parent->rb_left)
			parent->rb_left = right;
		else
			parent->rb_right = right;
	}
	else
		root->rb_node = right;
	rb_set_parent(node, right);
}

static void __rb_rotate_right(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *left = node->rb_left;
	struct rb_node *parent = rb_parent(node);

	if ((node->rb_left = left->rb_right))
		rb_set_parent(left->rb_right, node);
	left->rb_right = node;

	rb_set_parent(left, parent);

	if (parent)
	{
		if (node == parent->rb_right)
			parent->rb_right = left;
		else
			parent->rb_left = left;
	}
	else
		root->rb_node = left;
	rb_set_parent(node, left);
}

void rb_insert_color(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *parent, *gparent;

	while ((parent = rb_parent(node)) && rb_is_red(parent))
	{
		gparent = rb_parent(parent);

		if (parent == gparent->rb_left)
		{
			{
				register struct rb_node *uncle = gparent->rb_right;
				if (uncle && rb_is_red(uncle))
				{
					rb_set_black(uncle);
					rb_set_black(parent);
					rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->rb_right == node)
			{
				register struct rb_node *tmp;
				__rb_rotate_left(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			rb_set_black(parent);
			rb_set_red(gparent);
			__rb_rotate_right(gparent, root);
		} else {
			{
				register struct rb_node *uncle = gparent->rb_left;
				if (uncle && rb_is_red(uncle))
				{
					rb_set_black(uncle);
					rb_set_black(parent);
					rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->rb_left == node)
			{
				register struct rb_node *tmp;
				__rb_rotate_right(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			rb_set_black(parent);
			rb_set_red(gparent);
			__rb_rotate_left(gparent, root);
		}
	}

	rb_set_black(root->rb_node);
}
EXPORT_SYMBOL(rb_insert_color);

static void __rb_erase_color(struct rb_node *node, struct rb_node *parent,
			     struct rb_root *root)
{
	struct rb_node *other;

	while ((!node || rb_is_black(node)) && node != root->rb_node)
	{
		if (parent->rb_left == node)
		{
			other = parent->rb_right;
			if (rb_is_red(other))
			{
				rb_set_black(other);
				rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			if ((!other->rb_left || rb_is_black(other->rb_left)) &&
			    (!other->rb_right || rb_is_black(other->rb_right)))
			{
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}
			else
			{
				if (!other->rb_right || rb_is_black(other->rb_right))
				{
					rb_set_black(other->rb_left);
					rb_set_red(other);
					__rb_rotate_right(other, root);
					other = parent->rb_right;
				}
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->rb_right);
				__rb_rotate_left(parent, root);
				node = root->rb_node;
				break;
			}
		}
		else
		{
			other = parent->rb_left;
			if (rb_is_red(other))
			{
				rb_set_black(other);
				rb_set_red(parent);
				__rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			if ((!other->rb_left || rb_is_black(other->rb_left)) &&
			    (!other->rb_right || rb_is_black(other->rb_right)))
			{
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}
			else
			{
				if (!other->rb_left || rb_is_black(other->rb_left))
				{
					rb_set_black(other->rb_right);
					rb_set_red(other);
					__rb_rotate_left(other, root);
					other = parent->rb_left;
				}
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->rb_left);
				__rb_rotate_right(parent, root);
				node = root->rb_node;
				break;
			}
		}
	}
	if (node)
		rb_set_black(node);
}

void rb_erase(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *child, *parent;
	int color;

	if (!node->rb_left)
		child = node->rb_right;
	else if (!node->rb_right)
		child = node->rb_left;
	else
	{
		struct rb_node *old = node, *left;

		node = node->rb_right;
		while ((left = node->rb_left) != NULL)
			node = left;

		if (rb_parent(old)) {
			if (rb_parent(old)->rb_left == old)
				rb_parent(old)->rb_left = node;
			else
				rb_parent(old)->rb_right = node;
		} else
			root->rb_node = node;

		child = node->rb_right;
		parent = rb_parent(node);
		color = rb_color(node);

		if (parent == old) {
			parent = node;
		} else {
			if (child)
				rb_set_parent(child, parent);
			parent->rb_left = child;

			node->rb_right = old->rb_right;
			rb_set_parent(old->rb_right, node);
		}

		node->rb_parent_color = old->rb_parent_color;
		node->rb_left = old->rb_left;
		rb_set_parent(old->rb_left, node);

		goto color;
	}

	parent = rb_parent(node);
	color = rb_color(node);

	if (child)
		rb_set_parent(child, parent);
	if (parent)
	{
		if (parent->rb_left == node)
			parent->rb_left = child;
		else
			parent->rb_right = child;
	}
	else
		root->rb_node = child;

 color:
	if (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}
EXPORT_SYMBOL(rb_erase);

static void rb_augment_path(struct rb_node *node, rb_augment_f func, void *data)
{
	struct rb_node *parent;

up:
	func(node, data);
	parent = rb_parent(node);
	if (!parent)
		return;

	if (node == parent->rb_left && parent->rb_right)
		func(parent->rb_right, data);
	else if (parent->rb_left)
		func(parent->rb_left, data);

	node = parent;
	goto up;
}

/*
 * after inserting @node into the tree, update the tree to account for
 * both the new entry and any damage done by rebalance
 */
void rb_augment_insert(struct rb_node *node, rb_augment_f func, void *data)
{
	if (node->rb_left)
		node = node->rb_left;
	else if (node->rb_right)
		node = node->rb_right;

	rb_augment_path(node, func, data);
}
EXPORT_SYMBOL(rb_augment_insert);

/*
 * before removing the node, find the deepest node on the rebalance path
 * that will still be there after @node gets removed
 */
struct rb_node *rb_augment_erase_begin(struct rb_node *node)
{
	struct rb_node *deepest;

	if (!node->rb_right && !node->rb_left)
		deepest = rb_parent(node);
	else if (!node->rb_right)
		deepest = node->rb_left;
	else if (!node->rb_left)
		deepest = node->rb_right;
	else {
		deepest = rb_next(node);
		if (deepest->rb_right)
			deepest = deepest->rb_right;
		else if (rb_parent(deepest) != node)
			deepest = rb_parent(deepest);
	}

	return deepest;
}
EXPORT_SYMBOL(rb_augment_erase_begin);

/*
 * after removal, update the tree to account for the removed entry
 * and any rebalance damage.
 */
void rb_augment_erase_end(struct rb_node *node, rb_augment_f func, void *data)
{
	if (node)
		rb_augment_path(node, func, data);
}
EXPORT_SYMBOL(rb_augment_erase_end);

/*
 * This function returns the first node (in sort order) of the tree.
 */
struct rb_node *rb_first(const struct rb_root *root)
{
	struct rb_node	*n;

	n = root->rb_node;
	if (!n)
		return NULL;
	while (n->rb_left)
		n = n->rb_left;
	return n;
}
EXPORT_SYMBOL(rb_first);

struct rb_node *rb_last(const struct rb_root *root)
{
	struct rb_node	*n;

	n = root->rb_node;
	if (!n)
		return NULL;
	while (n->rb_right)
		n = n->rb_right;
	return n;
}
EXPORT_SYMBOL(rb_last);

struct rb_node *rb_next(const struct rb_node *node)
{
	struct rb_node *parent;

	if (rb_parent(node) == node)
		return NULL;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	if (node->rb_right) {
		node = node->rb_right; 
		while (node->rb_left)
			node=node->rb_left;
		return (struct rb_node *)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	while ((parent = rb_parent(node)) && node == parent->rb_right)
		node = parent;

	return parent;
}
EXPORT_SYMBOL(rb_next);

struct rb_node *rb_prev(const struct rb_node *node)
{
	struct rb_node *parent;

	if (rb_parent(node) == node)
		return NULL;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	if (node->rb_left) {
		node = node->rb_left; 
		while (node->rb_right)
			node=node->rb_right;
		return (struct rb_node *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	while ((parent = rb_parent(node)) && node == parent->rb_left)
		node = parent;

	return parent;
}
EXPORT_SYMBOL(rb_prev);

void rb_replace_node(struct rb_node *victim, struct rb_node *new,
		     struct rb_root *root)
{
	struct rb_node *parent = rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	if (parent) {
		if (victim == parent->rb_left)
			parent->rb_left = new;
		else
			parent->rb_right = new;
	} else {
		root->rb_node = new;
	}
	if (victim->rb_left)
		rb_set_parent(victim->rb_left, new);
	if (victim->rb_right)
		rb_set_parent(victim->rb_right, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
EXPORT_SYMBOL(rb_replace_node);



//inline static void _unused* crtl_rbtree_iterator_first(struct crtl_rbtree_iterator_struct *iter);
//inline static void _unused* crtl_rbtree_iterator_next(struct crtl_rbtree_iterator_struct *iter);
//inline static void _unused* crtl_rbtree_iterator_prev(struct crtl_rbtree_iterator_struct *iter);
//inline static void _unused* crtl_rbtree_iterator_last(struct crtl_rbtree_iterator_struct *iter);

static struct crtl_rbtree_struct* __crtl_rbtree_init(int (*cmp)(const void *, const void *), crtl_boolean alloc)
{
    struct crtl_rbtree_struct* rbtree = malloc(sizeof(struct crtl_rbtree_struct));
    if(!rbtree)
    {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, rbtree);
        return NULL;
    }
    memset(rbtree, 0, sizeof(struct crtl_rbtree_struct));
    
    rbtree->alloc = alloc;
    
    rbtree->root.rb_node = NULL;

    rbtree->nnode = 0;
    rbtree->cmp = cmp;
//    rbtree->display = display;

    /* 初始化迭代器 */
//    rbtree->iter.rbtree = rbtree;
//    rbtree->iter.curr_node = NULL;
//    
//    rbtree->iter.first = crtl_rbtree_iterator_first;
//    rbtree->iter.next = crtl_rbtree_iterator_next;
//    rbtree->iter.prev = crtl_rbtree_iterator_prev;
//    rbtree->iter.last = crtl_rbtree_iterator_last;
    
    return rbtree;
}

_api struct crtl_rbtree_struct* crtl_rbtree_init(int (*cmp)(const void *, const void *))
{
    return __crtl_rbtree_init(cmp, crtl_false);
}
_api struct crtl_rbtree_struct* crtl_rbtree_init_alloc(int (*cmp)(const void *, const void *))
{
    return __crtl_rbtree_init(cmp, crtl_true);
}


/* insert */
_api int crtl_rbtree_insert(struct crtl_rbtree_struct* rbtree, void *data, unsigned int data_size)
{
    crtl_assert_fp(stderr, rbtree);
    if(!data || data_size<=0)
    {
        crtl_print_err("null pointer or out of range error.\n");
        crtl_assert_fp(stderr, 0);
        return CRTL_ERROR;
    }
    
    struct rb_node **tmp = &(rbtree->root.rb_node), *parent= NULL;
    
    struct crtl_rbtree_node_struct *newnode = malloc(sizeof(struct crtl_rbtree_node_struct));
    newnode->data_size = data_size;

    if(rbtree->alloc) {
        newnode->data = malloc(data_size);
        memcpy(newnode->data, data, data_size);
    } else {
        newnode->data = (void*)data;
    }
    
    
    while(*tmp) {
        struct crtl_rbtree_node_struct *this = container_of(*tmp, struct crtl_rbtree_node_struct, node);
        parent = *tmp;
        
        int cmp_ret = rbtree->cmp(this->data, data);
        if(cmp_ret == CRTL_GT || cmp_ret > 0) {
            tmp = &((*tmp)->rb_left);
        } else if(cmp_ret == CRTL_LT || cmp_ret < 0) {
            tmp = &((*tmp)->rb_right);
        } else {
            crtl_print_err("This RBnode already exist.\n");
            crtl_assert_fp(stderr, 0);
            return CRTL_ERROR;
        }
    }
    rb_link_node(&newnode->node, parent, tmp);
    rb_insert_color(&newnode->node, &rbtree->root);

    rbtree->nnode += 1;
    
    return CRTL_SUCCESS;
}


/* search */
_api struct crtl_rbtree_node_struct *crtl_rbtree_search(struct crtl_rbtree_struct* rbtree, const void *data)
{
    crtl_assert_fp(stderr, rbtree);
    if(!data)
    {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, data);
        return NULL;
    }
    
    struct rb_node *node = rbtree->root.rb_node;
    while(node)
    {
        struct crtl_rbtree_node_struct *node_data = container_of(node, struct crtl_rbtree_node_struct, node);
        int cmp_ret = rbtree->cmp(node_data->data, data);
        if(cmp_ret == CRTL_GT || cmp_ret > 0) {
            node = node->rb_left;
        } else if(cmp_ret == CRTL_LT || cmp_ret < 0) {
            node = node->rb_right;
        } else if(cmp_ret == CRTL_EQ) {
            return node_data;
        } else {
            crtl_print_err("This RBnode not exist.\n");
            crtl_assert_fp(stderr, 0);
            return NULL;
        }
    }

    return NULL;
}


/* delete */
_api int crtl_rbtree_delete(struct crtl_rbtree_struct* rbtree, const void *data)
{
    if(unlikely(!data) || unlikely(!rbtree))
    {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, data);
        return CRTL_ERROR;
    }
    
    struct crtl_rbtree_node_struct *node_data = crtl_rbtree_search(rbtree, data);
    if(!node_data)
    {
        crtl_print_err("Not found rbtree node.\n");
        return CRTL_ERROR;
    }
    rb_erase(&node_data->node, &rbtree->root);
    
    if(rbtree->alloc) {
        free(node_data->data);
    }
    free(node_data);

    rbtree->nnode -= 1;
    
    return CRTL_SUCCESS;
}


_api int crtl_rbtree_nnode(const struct crtl_rbtree_struct* rbtree)
{
    if(unlikely(!rbtree))
    {
        crtl_print_err("null pointer error.\n");
        crtl_assert_fp(stderr, rbtree);
        return CRTL_ERROR;
    }
    
    return rbtree->nnode;
}

_api int crtl_rbtree_is_empty(const struct crtl_rbtree_struct* rbtree)
{
    return crtl_rbtree_nnode(rbtree)==0?CRTL_SUCCESS:CRTL_ERROR;
}


/* getfirst */
_api struct crtl_rbtree_node_struct* crtl_rbtree_getfirst(struct crtl_rbtree_struct* rbtree)
{
    crtl_assert_fp(stderr, rbtree);
    
    struct rb_node *first_rb_node = rb_first(&rbtree->root);
    if(!first_rb_node) 
        return NULL;
    struct crtl_rbtree_node_struct  *first_rt_rbtree_node = rb_entry(first_rb_node, struct crtl_rbtree_node_struct, node);
    
    if(first_rt_rbtree_node)
        return first_rt_rbtree_node;

    return NULL;
}
/* getfirst */
_api struct crtl_rbtree_node_struct* crtl_rbtree_getlast(struct crtl_rbtree_struct* rbtree)
{
    crtl_assert_fp(stderr, rbtree);
    
    struct rb_node *last_rb_node = rb_last(&rbtree->root);
    if(!last_rb_node) 
        return NULL;
    struct crtl_rbtree_node_struct  *last_rt_rbtree_node = rb_entry(last_rb_node, struct crtl_rbtree_node_struct, node);
    
    if(last_rt_rbtree_node)
        return last_rt_rbtree_node;

    return NULL;
}

/* getnext */
_api struct crtl_rbtree_node_struct* crtl_rbtree_getnext(struct crtl_rbtree_node_struct* node)
{   
    crtl_assert_fp(stderr, node);
    
    struct rb_node *next_rb_node = rb_next(&node->node);
    if(!next_rb_node) 
        return NULL;
    struct crtl_rbtree_node_struct  *next_rt_rbtree_node = rb_entry(next_rb_node, struct crtl_rbtree_node_struct, node);
    
    if(next_rt_rbtree_node)
        return next_rt_rbtree_node;
    
    return NULL;
}
/* getnext */
_api struct crtl_rbtree_node_struct* crtl_rbtree_getprev(struct crtl_rbtree_node_struct* node)
{   
    crtl_assert_fp(stderr, node);
    
    struct rb_node *prev_rb_node = rb_prev(&node->node);
    if(!prev_rb_node) 
        return NULL;
    struct crtl_rbtree_node_struct  *prev_rt_rbtree_node = rb_entry(prev_rb_node, struct crtl_rbtree_node_struct, node);
    
    if(prev_rt_rbtree_node)
        return prev_rt_rbtree_node;
    
    return NULL;
}


/**
 * get data from red black tree node
 * @param node: red black node
 * @return node data
 */
_api void* crtl_rbtree_node_data(struct crtl_rbtree_node_struct* node)
{
    crtl_assert_fp(stderr, node);
    return node->data;
}


/* destroy */
_api int crtl_rbtree_destroy(struct crtl_rbtree_struct* rbtree)
{
    crtl_assert_fp(stderr, rbtree);
    
    struct crtl_rbtree_node_struct *find_node = NULL;

    for(find_node=crtl_rbtree_getfirst(rbtree); find_node; find_node=crtl_rbtree_getfirst(rbtree))
    {
        crtl_rbtree_delete(rbtree, find_node->data);
    }
    
    free(rbtree);
    rbtree = NULL;
    
    return CRTL_SUCCESS;
}

//_api struct crtl_rbtree_iterator_struct* crtl_rbtree_iterator(struct crtl_rbtree_struct* rbtree)
//{
//    crtl_assert_fp(stderr, rbtree);
//    return &(rbtree->iter);
//}
//
//inline static void _unused* crtl_rbtree_iterator_first(struct crtl_rbtree_iterator_struct *iter)
//{
//    crtl_assert_fp(stderr, iter);
////    container_of(ptr, type, member)
//    iter->curr_node = crtl_rbtree_getfirst(iter->rbtree);
//    return iter->curr_node?(void*)iter->curr_node->data:NULL;
//}
//inline static void _unused* crtl_rbtree_iterator_next(struct crtl_rbtree_iterator_struct *iter)
//{
//    crtl_assert_fp(stderr, iter);
//    
//    struct crtl_rbtree_node_struct _unused *tmp_node = iter->curr_node;
//    
//    iter->curr_node = crtl_rbtree_getnext(iter->curr_node);
//    
//    if(iter->curr_node == NULL) {//如果想获取最后一个的下一个，currentnode指向最后一个节点，返回NULL
//        iter->curr_node = tmp_node;
//        return NULL;
//    } else {
//        return (void*)iter->curr_node->data;
//    }
//}
//inline static void _unused* crtl_rbtree_iterator_prev(struct crtl_rbtree_iterator_struct *iter)
//{
//    crtl_assert_fp(stderr, iter);
//    struct crtl_rbtree_node_struct _unused *tmp_node = iter->curr_node;
//    
//    iter->curr_node = crtl_rbtree_getprev(iter->curr_node);
//    
//    if(iter->curr_node == NULL) { //如果想获取第一个的上一个，currentnode指向第一个节点，返回NULL
//        iter->curr_node = tmp_node;
//        return NULL;
//    } else {
//        return (void*)iter->curr_node->data;
//    }
//}
//inline static void _unused* crtl_rbtree_iterator_last(struct crtl_rbtree_iterator_struct *iter)
//{
//    crtl_assert_fp(stderr, iter);
//    iter->curr_node = crtl_rbtree_getlast(iter->rbtree);
//    return iter->curr_node?(void*)iter->curr_node->data:NULL;
//}
//
//

