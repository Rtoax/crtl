#include <string.h>
#include <unistd.h>

#include "crtl/bits/crtl_tree_huffman.h"


extern void
huf_bit_write(huf_bit_read_writer_t *, uint8_t);


extern void
huf_bit_read_writer_reset(huf_bit_read_writer_t *);


struct __huf_encoder {
    // Read-only field with encoder configuration.
    huf_config_t *config;

    // Bit buffer.
    huf_bit_read_writer_t bit_writer;

    // Stores leaves and the root of the Huffman tree.
    huf_tree_t *huffman_tree;

    // char_coding represents map of binary encoding for
    // particular byte.
    huf_symbol_mapping_t *mapping;

    // Frequencies of the symbols occurrence.
    huf_histogram_t *histogram;

    // Read-writer instance.
    huf_read_writer_t *read_writer;

    // Buffered reader instance.
    huf_bufio_read_writer_t *bufio_writer;

    // Buffered writer instance.
    huf_bufio_read_writer_t *bufio_reader;
};


// Build a histogram by the frequency of symbol occurrence.
static huf_error_t
__huf_create_tree_by_histogram(huf_encoder_t *self)
{
    routine_m();

    huf_error_t err;
    huf_node_t *shadow_tree[HUF_ASCII_COUNT * 2] = {0};

    size_t j;
    int64_t rate, rate1, rate2;
    int16_t index1, index2;
    int16_t node = HUF_ASCII_COUNT;

    size_t start;
    uint64_t *rates = NULL;

    routine_param_m(self);

    // Create local copies of the histogram instance attibutes.
    start = self->histogram->start;
    rates = self->histogram->frequencies;

    // Calculate the length of the shadow tree.
    size_t shadow_tree_len = (sizeof(shadow_tree)
            / sizeof(*shadow_tree));

    while (start < shadow_tree_len) {
        index1 = index2 = -1;
        rate1 = rate2 = 0;

        // Skip zero-value frequencies, since they are not
        // paricipating in the building of the Huffman tree.
        while (!rates[start]) {
            start++;
        }

        // Find next minimum frequencies to construct a new tree node.
        for (j = start; j < node; j++) {
            rate = rates[j];

            // Skip zero-value frequencies.
            if (!rate) {
                continue;
            }

            if (!rate1) {
                // Initialize the first frequecy value first.
                rate1 = rate;
                index1 = j;
            } else if (rate <= rate1) {
                // Swap values of the rate1 and rate2.
                rate2 = rate1;
                rate1 = rate;
                index2 = index1;
                index1 = j;
            } else if (!rate2 || rate <= rate2) {
                // If the rate2 is lower, than found.
                rate2 = rate;
                index2 = j;
            }
        }

        // Tree is constructed, leave the while loop.
        if (index1 == -1 || index2 == -1) {
            self->huffman_tree->root = shadow_tree[node - 1];
            break;
        }

        if (!shadow_tree[index1]) {
            // Allocate memory for the left child of the node.
            err = huf_malloc(void_pptr_m(&shadow_tree[index1]),
                    sizeof(huf_node_t), 1);
            if (err != HUF_ERROR_SUCCESS) {
                routine_error_m(err);
            }
        }

        if (!shadow_tree[index2]) {
            // Allocate memory for the right child of the node.
            err = huf_malloc(void_pptr_m(&shadow_tree[index2]),
                    sizeof(huf_node_t), 1);
            if (err != HUF_ERROR_SUCCESS) {
                routine_error_m(err);
            }
        }

        // Allocate memory for the node itself.
        err = huf_malloc(void_pptr_m(&shadow_tree[node]),
                sizeof(huf_node_t), 1);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        if (index1 < HUF_ASCII_COUNT) {
            self->huffman_tree->leaves[index1] = shadow_tree[index1];
        }

        if (index2 < HUF_ASCII_COUNT) {
            self->huffman_tree->leaves[index2] = shadow_tree[index2];
        }

        shadow_tree[index1]->parent = shadow_tree[node];
        shadow_tree[index2]->parent = shadow_tree[node];
        shadow_tree[node]->left = shadow_tree[index1];
        shadow_tree[node]->right = shadow_tree[index2];

        shadow_tree[index1]->index = index1;
        shadow_tree[index2]->index = index2;
        shadow_tree[node]->index = node;

        rates[node] = rate1 + rate2;
        rates[index1] = 0;
        rates[index2] = 0;
        node++;
    }

    routine_ensure_m();

    // If the routine was interrupted by an error we should
    // release the memory block occupied by the shadow tree.
    if (routine_violation_m()) {
        for (j = 0; j < shadow_tree_len; j++) {
            free(shadow_tree[j]);
        }
    }

    routine_defer_m();
}


// Create a mapping of 8-bit bytes to the Huffman encoding.
static huf_error_t
__huf_create_char_coding(huf_encoder_t *self)
{
    routine_m();

    huf_error_t err;
    huf_symbol_mapping_element_t *element = NULL;

    uint8_t coding[HUF_1KIB_BUFFER] = {0};

    routine_param_m(self);

    size_t index;
    for ( index = 0; index < self->mapping->length; index++) {
        const huf_node_t *node = self->huffman_tree->leaves[index];

        if (!node) {
            continue;
        }

        size_t position = sizeof(coding);

        // Print node to the string.
        err = huf_node_to_string(node, coding, &position);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        // Create mapping element and inialize it with coding string.
        err = huf_symbol_mapping_element_init(
                &element, coding, position);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        // Insert coding element to the symbol-aware position.
        err = huf_symbol_mapping_insert(
                self->mapping, index, element);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }
    }

    routine_yield_m();
}


// Encode chunk of data.
static huf_error_t
__huf_encode_chunk(
        huf_encoder_t* self,
        const uint8_t *buf, uint64_t len)
{
    routine_m();

    huf_error_t err;
    huf_symbol_mapping_element_t *element = NULL;

    uint64_t pos;
    size_t index;

    routine_param_m(self);
    routine_param_m(buf);

    for (pos = 0; pos < len; pos++) {
        // Retrieve the next symbol coding element.
        err = huf_symbol_mapping_get(self->mapping,
                buf[pos], &element);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        for (index = element->length; index > 0; index--) {
            // Fill the next bit of the encoded byte.
            huf_bit_write(&self->bit_writer,
                    element->coding[index - 1]);

            if (self->bit_writer.offset) {
                continue;
            }

            // If buffer is full, then dump it to the writer buffer.
            err = huf_bufio_write_uint8(self->bufio_writer,
                    self->bit_writer.bits);
            if (err != HUF_ERROR_SUCCESS) {
                routine_error_m(err);
            }

            huf_bit_read_writer_reset(&self->bit_writer);
        }
    }

    if (self->bit_writer.offset != 8) {
        err = huf_bufio_write_uint8(self->bufio_writer,
                self->bit_writer.bits);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }
    }

    routine_yield_m();
}


// Create a new instance of the Huffman encoder.
huf_error_t
huf_encoder_init(
        huf_encoder_t **self,
        const huf_config_t *config)
{
    routine_m();

    huf_encoder_t *self_ptr = NULL;
    huf_config_t *encoder_config = NULL;

    routine_param_m(self);
    routine_param_m(config);

    huf_error_t err = huf_malloc(void_pptr_m(&self_ptr),
            sizeof(huf_encoder_t), 1);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    *self = self_ptr;

    // Save the encoder configuration.
    err = huf_config_init(&encoder_config);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    memcpy(encoder_config, config, sizeof(*config));

    // If size of encoding chunk set to zero then length of the
    // data to encode will be treated as size of the chunk.
    if (!encoder_config->chunk_size) {
        encoder_config->chunk_size = encoder_config->length;
    }

    self_ptr->config = encoder_config;

    // Initialize read-writer instance.
    err = huf_read_writer_init(&self_ptr->read_writer,
            self_ptr->config->reader, config->writer);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    // Allocate memory for Huffman tree.
    err = huf_tree_init(&self_ptr->huffman_tree);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    err = huf_symbol_mapping_init(&self_ptr->mapping,
            HUF_ASCII_COUNT);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    // Allocate memory for the frequency histogram.
    err = huf_histogram_init(&self_ptr->histogram, 1,
            HUF_HISTOGRAM_LEN);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    // Create buffered writer instance. If writer buffer size
    // set to zero, the 64 KiB buffer will be used by default.
    err = huf_bufio_read_writer_init(&self_ptr->bufio_writer,
            self_ptr->read_writer,
            self_ptr->config->writer_buffer_size);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    // Create buffered reader instance. If reader buffer size
    // set to zero, the 64 KiB buffer will be used by default.
    err = huf_bufio_read_writer_init(&self_ptr->bufio_reader,
            self_ptr->read_writer,
            self_ptr->config->reader_buffer_size);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    routine_yield_m();
}


// Release memory occupied by the Huffman encoder.
huf_error_t
huf_encoder_free(huf_encoder_t **self)
{
    routine_m();

    huf_error_t err;
    huf_encoder_t *self_ptr;

    routine_param_m(self);

    self_ptr = *self;

    err = huf_tree_free(&self_ptr->huffman_tree);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    err = huf_bufio_read_writer_free(
            &self_ptr->bufio_writer);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    err = huf_bufio_read_writer_free(
            &self_ptr->bufio_reader);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    err = huf_read_writer_free(&self_ptr->read_writer);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    err = huf_histogram_free(&self_ptr->histogram);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    err = huf_symbol_mapping_free(&self_ptr->mapping);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    err = huf_config_free(&self_ptr->config);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    free(self_ptr);

    *self = NULL;

    routine_yield_m();
}


// Encode the data according to the provided
// configuration.
huf_error_t
huf_encode(const huf_config_t *config)
{
    routine_m();

    huf_error_t err;
    huf_encoder_t *self = NULL;

    uint8_t *buf = NULL;
    int16_t tree_head[HUF_BTREE_LEN] = {0};

    int16_t actual_tree_length = 0;
    size_t tree_length = 0;

    err = huf_encoder_init(&self, config);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    err = huf_malloc(void_pptr_m(&buf), sizeof(uint8_t),
            self->config->chunk_size);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    size_t left_to_read = self->config->length;
    size_t need_to_read;

    while (left_to_read > 0) {
        need_to_read = self->config->chunk_size;

        if (left_to_read < need_to_read) {
            need_to_read = left_to_read;
        }

        // Read the next chunk of data, that we are going to encode.
        err = huf_bufio_read(self->bufio_reader,
                buf, need_to_read);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        err = huf_histogram_populate(self->histogram,
                buf, need_to_read);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        err = __huf_create_tree_by_histogram(self);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        err = __huf_create_char_coding(self);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        // Write serialized tree into buffer.
        err = huf_tree_serialize(self->huffman_tree,
                tree_head, &tree_length);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        actual_tree_length = tree_length;

        // Write the size of the next chunk.
        err = huf_bufio_write(self->bufio_writer,
                &need_to_read, sizeof(need_to_read));
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        // Write the length of the serialized Huffman tree.
        err = huf_bufio_write(self->bufio_writer,
                &actual_tree_length, sizeof(actual_tree_length));
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        // Write the serialized tree itself.
        err = huf_bufio_write(self->bufio_writer, tree_head,
                tree_length * sizeof(int16_t));
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        // Reset the bit writer before encoging the next chunk of data.
        huf_bit_read_writer_reset(&self->bit_writer);

        // Write data
        err = __huf_encode_chunk(self, buf, need_to_read);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        left_to_read -= need_to_read;

        // If there is no more data to read, then skip reset of the histogram.
        if (!left_to_read) {
            continue;
        }

        err = huf_tree_reset(self->huffman_tree);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        err = huf_histogram_reset(self->histogram);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }

        err = huf_symbol_mapping_reset(self->mapping);
        if (err != HUF_ERROR_SUCCESS) {
            routine_error_m(err);
        }
    }

    // Flush buffer to the file.
    err = huf_bufio_read_writer_flush(self->bufio_writer);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    routine_ensure_m();

    huf_encoder_free(&self);
    free(buf);

    routine_defer_m();
}

