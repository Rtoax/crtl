#include <string.h>
#include <unistd.h>

#include "crtl/bits/crtl_tree_huffman.h"



struct __crtl_huf_decoder {
    // Read-only field with decoder configuration.
    crtl_huf_config_t *config;

    // Stores leaves and the root of the Huffman tree.
    crtl_huf_tree_t *huffman_tree;

    // last_node stores pointer of the last decoded byte.
    crtl_huf_node_t *last_node;

    // Read-writer instance.
    crtl_huf_read_writer_t *read_writer;

    // Buffer for write operations.
    crtl_huf_bufio_read_writer_t *bufio_writer;

    // Buffer for read operations.
    crtl_huf_bufio_read_writer_t *bufio_reader;
};


// Decode the chunk of data.
static crtl_huf_error_t
__crtl_huf_decode_chunk(
        crtl_huf_decoder_t *self, size_t len)
{
    __crtl_huf_routine_m();

    crtl_huf_error_t err;

    uint8_t byte;
    uint8_t bit_offset;

    size_t restored = 0;

    __crtl_huf_routine_param_m(self);

    if (!self->last_node) {
        self->last_node = self->huffman_tree->root;
    }

    while (restored < len) {
        // Read the next chunk of bit stream.
        err = crtl_huf_bufio_read_uint8(self->bufio_reader, &byte);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        for (bit_offset = 8; bit_offset > 0; bit_offset--) {
            // If next bit equals to 1, then move to the right branch.
            // Otherwise move to the left branch.
            if ((byte >> (bit_offset - 1)) & 1) {
                self->last_node = self->last_node->right;
            } else {
                self->last_node = self->last_node->left;
            }

            // Continue until the leaf (encoded byte) will be found.
            if (self->last_node->left || self->last_node->right) {
                continue;
            }

            err = crtl_huf_bufio_write_uint8(self->bufio_writer,
                    self->last_node->index);
            if (err != CRTL_HUF_ERROR_SUCCESS) {
                __crtl_huf_routine_error_m(err);
            }

            restored++;

            // Reset last node value to tree root.
            self->last_node = self->huffman_tree->root;

            // Prevent unnecessary decoding of the bit fillers.
            if (restored >= len) {
                break;
            }
        }
    }

    __crtl_huf_routine_yield_m();
}


// Initialize a new instance of the Huffman-decoder.
crtl_huf_error_t
crtl_huf_decoder_init(
        crtl_huf_decoder_t **self,
        const crtl_huf_config_t *config)
{
    __crtl_huf_routine_m();

    crtl_huf_decoder_t *self_ptr = NULL;
    crtl_huf_config_t *decoder_config = NULL;

    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(config);

    // Allocate memory for a new decoder instance.
    crtl_huf_error_t err = crtl_huf_malloc(__crtl_void_pptr_m(&self_ptr),
            sizeof(crtl_huf_decoder_t), 1);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    *self = self_ptr;

    // Create a new instance of the decoder configuration.
    err = crtl_huf_config_init(&decoder_config);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    memcpy(decoder_config, config, sizeof(*config));
    self_ptr->config = decoder_config;

    // Initialize read-writer instance.
    err = crtl_huf_read_writer_init(&self_ptr->read_writer,
            self_ptr->config->reader, config->writer);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    // Allocate memory for Huffman tree.
    err = crtl_huf_tree_init(&self_ptr->huffman_tree);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    // Create buffered writer instance. If writer buffer
    // size set to zero, the 64 KiB buffer will be used
    // by default.
    err = crtl_huf_bufio_read_writer_init(&self_ptr->bufio_writer,
            self_ptr->read_writer,
            self_ptr->config->writer_buffer_size);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    // Create buffered reader instance. If reader buffer
    // size set to zero, the 64 KiB buffer will be used
    // by default.
    err = crtl_huf_bufio_read_writer_init(&self_ptr->bufio_reader,
            self_ptr->read_writer,
            self_ptr->config->reader_buffer_size);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    __crtl_huf_routine_yield_m();
}


// Release memory occupied by the Huffman-decoder.
crtl_huf_error_t
crtl_huf_decoder_free(crtl_huf_decoder_t **self)
{
    __crtl_huf_routine_m();

    crtl_huf_error_t err;
    crtl_huf_decoder_t *self_ptr;

    __crtl_huf_routine_param_m(self);

    self_ptr = *self;

    err = crtl_huf_tree_free(&self_ptr->huffman_tree);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    err = crtl_huf_bufio_read_writer_free(
            &self_ptr->bufio_writer);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    err = crtl_huf_bufio_read_writer_free(
            &self_ptr->bufio_reader);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    err = crtl_huf_read_writer_free(
            &self_ptr->read_writer);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    err = crtl_huf_config_free(
            &self_ptr->config);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    free(self_ptr);

    *self = NULL;

    __crtl_huf_routine_yield_m();
}


// Decodes the data according to the provide
// configuration.
crtl_huf_error_t
crtl_huf_decode(const crtl_huf_config_t *config)
{
    __crtl_huf_routine_m();

    crtl_huf_decoder_t *self = NULL;
    crtl_huf_error_t err;

    int16_t *tree_head = NULL;
    int16_t tree_length = 0;

    // Create a new decoder instance.
    err = crtl_huf_decoder_init(&self, config);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    while (self->config->length >
            self->bufio_reader->have_been_processed) {
        // Read the length of the next chunk (the original length of
        // encoded bytes).
        err = crtl_huf_bufio_read(self->bufio_reader,
                &self->config->chunk_size,
                sizeof(self->config->chunk_size));
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        // Read the length of the serialized Huffman tree.
        err = crtl_huf_bufio_read(self->bufio_reader,
                &tree_length, sizeof(tree_length));
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        // Allocate memory for serialized Huffman tree.
        err = crtl_huf_malloc(__crtl_void_pptr_m(&tree_head),
                sizeof(int16_t), tree_length);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        // Read serialized Huffman tree.
        err = crtl_huf_bufio_read(self->bufio_reader, tree_head,
                tree_length * sizeof(int16_t));
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        // Create linked tree structure.
        err = crtl_huf_tree_deserialize(self->huffman_tree,
                tree_head, tree_length);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        // Decode the next chunk of data.
        err = __crtl_huf_decode_chunk(self, self->config->chunk_size);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        err = crtl_huf_tree_reset(self->huffman_tree);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        self->last_node = NULL;

        // TODO: make an optimization to reduce allocations.
        free(tree_head);
        tree_head = NULL;
    }

    err = crtl_huf_bufio_read_writer_flush(self->bufio_writer);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    __crtl_huf_routine_ensure_m();
    crtl_huf_decoder_free(&self);

    __crtl_huf_routine_defer_m();
}


