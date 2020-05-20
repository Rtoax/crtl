/*
 * This is a test program for crtl_avl_tree.h and crtl_avl_tree.c.  Compile with:
 *
 *	$ gcc test.c crtl_avl_tree.c -o test -std=c99 -Wall -O2
 *
 * The test strategy isn't very sophisticated; it just relies on repeated random
 * operations to cover as many cases as possible.  Feel free to improve it.
 *
 * -----------------------------------------------------------------------------
 *
 * Written in 2014-2016 by Eric Biggers <ebiggers3@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide via the Creative Commons Zero 1.0 Universal Public Domain
 * Dedication (the "CC0").
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the CC0 for more details.
 *
 * You should have received a copy of the CC0 along with this software; if not
 * see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "crtl/tree.h"
#include "crypto/tree/avl.h"

/* Change this to 0, and this turns into a benchmark program.  */
#define __AVL_VERIFY 1

struct test_node {
#if __AVL_VERIFY
	int height;
	int reached;
#endif
	int n;
	struct crtl_avl_tree_node node;
};

static struct crtl_avl_tree_node *root = NULL;

static struct test_node *nodes;
static int node_idx;

static inline int crtl_avl_get_balance_factor(const struct crtl_avl_tree_node *node)
{
	return (int)(node->parent_balance & 3) - 1;
}

#define TEST_NODE(__node) crtl_avl_tree_entry(__node, struct test_node, node)
#define INT_VALUE(node) TEST_NODE(node)->n
#define HEIGHT(node) ((node) ? TEST_NODE(node)->height : 0)
#define max(a, b) ((a) > (b) ? (a) : (b))

static int cmp_int_nodes(const struct crtl_avl_tree_node *node1,
	      const struct crtl_avl_tree_node *node2)
{
	return INT_VALUE(node1) - INT_VALUE(node2);
}

static void insert(int n)
{
	struct test_node *i = &nodes[node_idx++];
	i->n = n;
	assert(NULL == crtl_avl_tree_insert(&root, &i->node, cmp_int_nodes));
}

static struct test_node *lookup(int n)
{
	struct test_node query;
	struct crtl_avl_tree_node *result;

	query.n = n;

	result = crtl_avl_tree_lookup_node(root, &query.node, cmp_int_nodes);

	return result ? TEST_NODE(result) : NULL;
}

static void deletenode(struct test_node *node)
{
	crtl_avl_tree_remove(&root, &node->node);
}

static void delete(int n)
{
	struct test_node *node;
	
	node = lookup(n);
	assert(node);
	deletenode(node);
}

#if __AVL_VERIFY
static void __setheights(struct crtl_avl_tree_node *node)
{
	if (node) {
		assert(node->left != node);
		assert(node->right != node);
		__setheights(node->left);
		__setheights(node->right);
		TEST_NODE(node)->height = max(HEIGHT(node->left), HEIGHT(node->right)) + 1;
	}
}

static void __checktree(struct crtl_avl_tree_node *node)
{
	int f = crtl_avl_get_balance_factor(node);
	assert(f >= -1 && f <= 1);
	assert(f == HEIGHT(node->right) - HEIGHT(node->left));
	if (node->left) {
		assert(INT_VALUE(node->left) < INT_VALUE(node));
		__checktree(node->left);
	}
	if (node->right) {
		assert(INT_VALUE(node->right) > INT_VALUE(node));
		__checktree(node->right);
	}
}

static void setheights(void)
{
	__setheights(root);
}

static void checktree(void)
{
	if (root)
		__checktree(root);
}

static int cmp_ints(const void *p1, const void *p2)
{
	return *(const int *)p1 - *(const int *)p2;
}

static void verify(const int *data, int count)
{
	const struct crtl_avl_tree_node *cur;
	int x;
	int data_sorted[count];

	memcpy(data_sorted, data, count * sizeof(data[0]));
	qsort(data_sorted, count, sizeof(data[0]), cmp_ints);

	/* Check in-order traversal.  */
	for (cur = crtl_avl_tree_first_in_order(root), x = 0;
	     cur;
	     cur = crtl_avl_tree_next_in_order(cur), x++)
	{
		assert(INT_VALUE(cur) == data_sorted[x]);
		TEST_NODE(cur)->reached = 0;
        
	}
	assert(x == count);

	/* Check reverse in-order traversal.  */
	for (cur = crtl_avl_tree_last_in_order(root), x = count - 1;
	     cur;
	     cur = crtl_avl_tree_prev_in_order(cur), x--)
	{
		assert(INT_VALUE(cur) == data_sorted[x]);
		TEST_NODE(cur)->reached = 0;
	}
	assert(x == -1);

	/* Check postorder traversal.  */
	for (cur = crtl_avl_tree_first_in_postorder(root), x = 0;
	     cur;
	     cur = crtl_avl_tree_next_in_postorder(cur, crtl_avl_get_parent(cur)), x++)
	{
		assert(!(TEST_NODE(cur)->reached));
		TEST_NODE(cur)->reached = 1;
		assert(!crtl_avl_get_parent(cur) ||
		       !TEST_NODE(crtl_avl_get_parent(cur))->reached);
		assert(!cur->left   || TEST_NODE(cur->left)->reached);
		assert(!cur->right  || TEST_NODE(cur->right)->reached);
	}
	assert(x == count);
}
#endif

static void shuffle(int data[], int count)
{
    int i;
	for (i = 0; i < count; i++) {
		const int x = rand() % count;
		const int v0 = data[0];
		const int vx = data[x];
		data[0] = vx;
		data[x] = v0;
	}
}

static void test(int data[], int count)
{
	shuffle(data, count);
	node_idx = 0;
    int i;
	/* Insert the data, checking the AVL tree invariants after each step.  */
	for ( i = 0; i < count; i++) {
		insert(data[i]);
	#if __AVL_VERIFY
		setheights();
		checktree();
		verify(data, i + 1);
	#endif
	}

	/* Delete the data in random order, checking the AVL tree invariants
	 * after each step.  */
	shuffle(data, count);
	for (i = 0; i < count; i++) {
		delete(data[i]);
	#if __AVL_VERIFY
		setheights();
		checktree();
		verify(data + (i + 1), count - (i + 1));
	#endif
	}
}

int main(void)
{
	const int num_iterations = 10000;
	const int max_node_count = 50;
	int data[max_node_count];
    int i;
	for ( i = 0; i < max_node_count; i++)
		data[i] = i;

	nodes = malloc(max_node_count * sizeof(nodes[0]));

	printf("Using max_node_count=%d\n", max_node_count);

	for ( i = 0; i < num_iterations; i++) {

		if (i % 1024 == 0)
			printf("Iteration %d/%d\n", i, num_iterations);

		/* Reset the tree.  */
		root = NULL;

		/* Do the test with a random number of nodes, up to the
		 * 'max_node_count'.  */
		test(data, rand() % max_node_count);

		/* Shuffle the array.  */
		shuffle(data, max_node_count);
	}

	printf("Done.\n");
    
    struct test_node *_i = NULL;
    struct crtl_avl_tree_node *currnode = NULL;
    crtl_avl_tree_for_each_in_order(currnode, _i, root, struct test_node, node) {
        printf("\t%d", TEST_NODE(currnode)->n);
//        printf("\t%d", _i->n);
    }

	free(nodes);

	return 0;
}


