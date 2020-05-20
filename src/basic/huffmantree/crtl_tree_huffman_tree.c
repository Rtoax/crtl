#include <string.h>

#include <unistd.h>

#include "crtl/bits/tree_huffman.h"


// Write the path to the root node of the Huffman
// tree starting from the specified node into the
// buffer.
crtl_huf_error_t
crtl_huf_node_to_string(
        const crtl_huf_node_t *self,
        uint8_t *buf, size_t *len)
{
    __crtl_huf_routine_m();

    const crtl_huf_node_t *node = self;
    size_t position = 0;

    __crtl_huf_routine_param_m(buf);
    __crtl_huf_routine_param_m(len);

    while(node) {
        if (!node->parent) {
            break;
        }

        // Keep borders of the buffer.
        if (position >= *len) {
            __crtl_huf_routine_success_m();
        }

        if (node->parent->left == node) {
            buf[position] = '0';
        } else {
            buf[position] = '1';
        }

        position++;
        node = node->parent;
    }

    __crtl_huf_routine_ensure_m();
    *len = position;

    __crtl_huf_routine_defer_m();
}


// Initialize a new instance of the Huffman tree.
crtl_huf_error_t
crtl_huf_tree_init(crtl_huf_tree_t **self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    crtl_huf_error_t err = crtl_huf_malloc(__crtl_void_pptr_m(self),
            sizeof(crtl_huf_tree_t), 1);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    crtl_huf_tree_t *self_ptr = *self;

    err = crtl_huf_malloc(__crtl_void_pptr_m(&self_ptr->leaves),
            sizeof(crtl_huf_node_t*), CRTL_HUF_ASCII_COUNT * 2);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    __crtl_huf_routine_yield_m();
}


// Recursively release memory occupied
// by the Huffman nodes.
static void
__crtl_huf_tree_free(crtl_huf_node_t* node)
{
    if (!node) {
        return;
    }

    if (node->left) {
        __crtl_huf_tree_free(node->left);
        free(node->left);
        node->left = NULL;
    }

    if (node->right) {
        __crtl_huf_tree_free(node->right);
        free(node->right);
        node->left = NULL;
    }
}


// Release memory occupied by the Huffman tree.
crtl_huf_error_t
crtl_huf_tree_free(crtl_huf_tree_t **self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    crtl_huf_tree_t *self_ptr = *self;

    __crtl_huf_tree_free(self_ptr->root);

    free(self_ptr->root);
    free(self_ptr->leaves);
    free(self_ptr);

    *self = NULL;

    __crtl_huf_routine_yield_m();
}


// Release the memory occupied by the Huffman
// tree nodes.
crtl_huf_error_t
crtl_huf_tree_reset(crtl_huf_tree_t *self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    __crtl_huf_tree_free(self->root);
    free(self->root);
    self->root = NULL;

    // Reset the memory occupied by the leaves.
    memset(self->leaves, 0, (sizeof(crtl_huf_node_t*)
                * CRTL_HUF_ASCII_COUNT * 2));

    __crtl_huf_routine_yield_m();
}


// Recursively de-serialize the Huffman tree into the
// provided buffer.
static crtl_huf_error_t
__crtl_huf_deserialize_tree(
        crtl_huf_node_t **node,
        const int16_t *buf, size_t *len)
{
    __crtl_huf_routine_m();

    int16_t node_index = CRTL_HUF_LEAF_NODE;

    size_t left_branch_len = 0;
    size_t right_branch_len = 0;

    __crtl_huf_routine_param_m(node);
    __crtl_huf_routine_param_m(buf);
    __crtl_huf_routine_param_m(len);

    size_t buf_len = *len;

    if (buf_len < 1) {
        // Set length of the read data to zero, since there is
        // nothing to read;
        *len = 0;

        __crtl_huf_routine_success_m();
    }

    crtl_huf_node_t *node_ptr = *node;
    node_index = *buf;

    if (node_index == CRTL_HUF_LEAF_NODE) {
        // Set length of the read data to one, since leaf is a
        // part of the serialized tree.
        *len = 1;

        __crtl_huf_routine_success_m();
    }

    crtl_huf_error_t err = crtl_huf_malloc(__crtl_void_pptr_m(&node_ptr),
            sizeof(crtl_huf_node_t), 1);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    *node = node_ptr;
    node_ptr->index = node_index;

    crtl_huf_node_t **node_left = &node_ptr->left;
    crtl_huf_node_t **node_right = &node_ptr->right;

    const int16_t *buf_ptr = buf + 1;
    // Current node is de-serialized, so shift pointer to
    // the next one and reduce overall length of buffer by one.
    left_branch_len = buf_len - 1;

    // Recursively de-serialize a left branch of the tree.
    err = __crtl_huf_deserialize_tree(node_left,
            buf_ptr, &left_branch_len);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    buf_ptr += left_branch_len;
    right_branch_len = buf_len - left_branch_len - 1;

    // Recursively de-serialize a right branch of the tree.
    err = __crtl_huf_deserialize_tree(node_right,
            buf_ptr, &right_branch_len);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    // Return in *len* argument length of the read data.
    *len = left_branch_len + right_branch_len + 1;

    __crtl_huf_routine_yield_m();
}


// De-serialize the Huffman tree into the
// provided buffer.
crtl_huf_error_t
crtl_huf_tree_deserialize(
        crtl_huf_tree_t *self,
        const int16_t *buf, size_t len)
{
    __crtl_huf_routine_m();

    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(buf);

    crtl_huf_error_t err = __crtl_huf_deserialize_tree(
            &self->root, buf, &len);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    __crtl_huf_routine_yield_m();
}



// Recursively serialize the Huffman tree from the
// provided buffer.
static crtl_huf_error_t
__crtl_huf_serialize_tree(
        const crtl_huf_node_t *node,
        int16_t *buf, size_t *len)
{
    __crtl_huf_routine_m();

    size_t left_branch_len = 0;
    size_t right_branch_len = 0;

    __crtl_huf_routine_param_m(buf);
    __crtl_huf_routine_param_m(len);

    if (node) {
        *buf = node->index;

        int16_t *buf_ptr = buf + 1;

        // Serialize the left branch of the tree.
        crtl_huf_error_t err = __crtl_huf_serialize_tree(
                node->left, buf_ptr, &left_branch_len);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        buf_ptr += left_branch_len;

        // Serialize the right branch of the tree.
        err = __crtl_huf_serialize_tree(node->right,
                buf_ptr, &right_branch_len);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

    } else {
        *buf = CRTL_HUF_LEAF_NODE;
    }

    *len = left_branch_len + right_branch_len + 1;

    __crtl_huf_routine_yield_m();
}


// Serialize the Huffman tree from the
// provided buffer.
crtl_huf_error_t
crtl_huf_tree_serialize(
        crtl_huf_tree_t *self,
        int16_t *buf, size_t *len)
{
    __crtl_huf_routine_m();

    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(buf);
    __crtl_huf_routine_param_m(len);

    crtl_huf_error_t err = __crtl_huf_serialize_tree(
            self->root, buf, len);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    __crtl_huf_routine_yield_m();
}


