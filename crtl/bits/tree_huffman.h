#ifndef __CRTL_BITS_TREE_HUFFMAN_H
#define __CRTL_BITS_TREE_HUFFMAN_H 1

/**
 *  哈弗曼树： 带有权值的二叉树
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "crtl/easy/attribute.h"


#define CRTL_HUF_1KIB_BUFFER 1024

#define CRTL_HUF_64KIB_BUFFER 65536

#define CRTL_HUF_128KIB_BUFFER 131072

#define CRTL_HUF_256KIB_BUFFER 262144

#define CRTL_HUF_512KIB_BUFFER 524288

#define CRTL_HUF_1MIB_BUFFER 1048576


// The count of ASCII symbols
#define CRTL_HUF_ASCII_COUNT 256

// Maximum length of the 2-byte serialized Huffman tree.
#define CRTL_HUF_BTREE_LEN 1024

// Length of the symbols frequency histogram.
#define CRTL_HUF_HISTOGRAM_LEN 512

// All leaves of the Huffman tree will be marked with that value.
#define CRTL_HUF_LEAF_NODE -1


// Enumeration of the possible errors.
typedef enum {
    // The call completed successfully.
    CRTL_HUF_ERROR_SUCCESS,

    // Failed to allocate the memory block.
    CRTL_HUF_ERROR_MEMORY_ALLOCATION,

    // The specified argument is invalid,
    // e.g. the pointer is equal to nil.
    CRTL_HUF_ERROR_INVALID_ARGUMENT,

    // The failure caused by input/output error.
    CRTL_HUF_ERROR_READ_WRITE,

    // Failure caused by Unrecoverable error.
    CRTL_HUF_ERROR_FATAL,
} crtl_huf_error_t;


// crtl_huf_writer_t is a writer abstraction.
typedef int crtl_huf_writer_t;

// crtl_huf_reader_r is a reader abstraction.
typedef int crtl_huf_reader_t;

// crtl_huf_read_writer_t groups reader and
// writer abstractions.
typedef struct __crtl_huf_read_writer {
    // A reader instance.
    crtl_huf_reader_t reader;
    // A writer instance.
    crtl_huf_writer_t writer;
} crtl_huf_read_writer_t;



// crtl_huf_bufio_read_writer_t represents read/writer buffer.
typedef struct __crtl_huf_bufio_read_writer {
    // Byte sized read-write buffer.
    uint8_t *bytes;

    // Current read position.
    size_t offset;

    // Available size in bytes of the buffer.
    size_t capacity;

    // Current length of the buffer. Will be increased on write
    // operations until this value will reach buffer capacity.
    size_t length;

    // Count of bytes have been processed by read-writer.
    uint64_t have_been_processed;

    // Read-Writer instance.
    crtl_huf_read_writer_t *read_writer;
} crtl_huf_bufio_read_writer_t;


// crtl_huf_bit_read_writer_t represents read/writer bit buffer.
typedef struct __crtl_huf_bit_read_writer {
    // Bit sized read/write buffer.
    uint8_t bits;
    // Current offset for bit buffer.
    uint8_t offset;
} crtl_huf_bit_read_writer_t;



typedef struct __crtl_huf_encoder_config {
    // Count of the reader bytes to encode. This is the only
    // mandatory parameter, if set to zero then no data will
    // be compressed.
    uint64_t length;

    // Size of the encoding chunk. If set to zero then
    // length of the data to encode will be treated as
    // size of the chunk.
    uint64_t chunk_size;

    // Size of the reader buffer in bytes. If set to zero
    // then will be defaulted to 64 KiB.
    size_t reader_buffer_size;

    // Size of the writer buffer in bytes. If set to zero
    // then will be defaulted to 64 KiB.
    size_t writer_buffer_size;

    // Instance of the reader which will be used as
    // a provider of the input data.
    crtl_huf_reader_t reader;

    // Instance of the writer which will be used as
    // a consumer of the Huffman-encoded data.
    crtl_huf_writer_t writer;
} crtl_huf_config_t;


// A Huffman decoding context.
typedef struct __crtl_huf_decoder crtl_huf_decoder_t;

// A Huffman encoding context.
typedef struct __crtl_huf_encoder crtl_huf_encoder_t;

// A frequency histogram.
typedef struct __crtl_huf_histogram {
    // Frequency chart.
    uint64_t *frequencies;

    // Size in bytes of the each element. So value for the
    // very next value will be read from incoming data with a
    // shift in iota bytes. That value should be less than 8,
    // which means only 64 bit elements are allowed.
    size_t iota;

    // Count of the frequency histogram elements. Each
    // element length is the count of iota bytes.
    size_t length;

    // First position of non-zero value. This is could
    // be useful while iterating through frequencies to
    // skip empty values.
    size_t start;
} crtl_huf_histogram_t;

// An element of the symbol mapping.
typedef struct __crtl_huf_symbol_mapping_element {
    // Length of the symbol coding.
    size_t length;

    // Binary symbol coding.
    uint8_t *coding;
} crtl_huf_symbol_mapping_element_t;

// A symbol mapping.
typedef struct __crtl_huf_symbol_mapping {
    // Count of the elements in the mapping.
    size_t length;

    // Array of the symbols encodings.
    crtl_huf_symbol_mapping_element_t **symbols;
} crtl_huf_symbol_mapping_t;

// A node of the Huffman tree.
typedef struct __crtl_huf_node {
    // The weight of the node.
    int16_t index;

    // The pointer to the parent node.
    struct __crtl_huf_node *parent;

    // The pointer to the left child node.
    struct __crtl_huf_node *left;

    // The pointer to the right child node.
    struct __crtl_huf_node *right;
} crtl_huf_node_t;

// A Huffman tree.
typedef struct __crtl_huf_tree {
    // List of Huffman tree leaves.
    crtl_huf_node_t **leaves;

    // Root element of the Huffman tree.
    crtl_huf_node_t *root;
} crtl_huf_tree_t;


#define __crtl_void_pptr_m(pointer) ((void**)(pointer))

// Initialize the function routine.
#define __crtl_huf_routine_m() \
    crtl_huf_error_t __error = CRTL_HUF_ERROR_SUCCESS \


// Set the routine finalization label.
#define __crtl_huf_routine_ensure_m() \
    ensure: \


// Leave the function routine.
#define __crtl_huf_routine_defer_m() \
    do { \
        return __error; \
    } while(0) \


// Leave the function routine without finalization.
#define __crtl_huf_routine_yield_m() \
    do { \
        __crtl_huf_routine_ensure_m(); \
        return __error; \
    } while (0) \


// Validate that provided parameter is not equal to the nil.
#define __crtl_huf_routine_param_m(param) \
    do { \
        if ((param) == 0) { \
            __error = CRTL_HUF_ERROR_INVALID_ARGUMENT; \
            goto ensure; \
        } \
    } while (0) \


// Validate that specified parameter is satisfying
// criteria: low < value < high, and throw error otherwise.
#define __crtl_huf_routine_inrange_m(value, low, high) \
    do { \
        if ((value) < (low) || (value) > (high)) { \
            __error = CRTL_HUF_ERROR_INVALID_ARGUMENT; \
            goto ensure; \
        } \
    } while(0) \


// Leave the function with the provided error.
#define __crtl_huf_routine_error_m(error) \
    do { \
        __error = (error); \
        goto ensure; \
    } while (0) \


// Leave the function with CRTL_HUF_ERROR_SUCCESS.
#define __crtl_huf_routine_success_m() \
    __crtl_huf_routine_error_m(CRTL_HUF_ERROR_SUCCESS) \


// Return true when the routine interrupted with
// an error.
#define __crtl_huf_routine_violation_m()\
    (__error != CRTL_HUF_ERROR_SUCCESS) \





// Return string representation of the specified error.
const char* crtl_huf_error_string(crtl_huf_error_t error);


// Initialize a new instance of the read-writer.
crtl_huf_error_t crtl_huf_read_writer_init(crtl_huf_read_writer_t **self,
                                              crtl_huf_reader_t reader,
                                              crtl_huf_writer_t writer);


// Release memory occupied by the read-writer.
crtl_huf_error_t crtl_huf_read_writer_free(crtl_huf_read_writer_t **self);


// Write the specified amount of byte from the buffer
// starting from the *buf* pointer.
crtl_huf_error_t crtl_huf_write(crtl_huf_writer_t writer, const void *buf, size_t count);


// Read the specified amount of bytes into the buffer
// starting from the *buf* pointer.
crtl_huf_error_t crtl_huf_read(crtl_huf_reader_t reader, void *buf, size_t *count);


// Allocate the memory block of the specified size.
crtl_huf_error_t crtl_huf_malloc(void** ptr, size_t size, size_t num);




// Write the first bit of the specified word into the
// bit buffer.
static inline void _unused crtl_huf_bit_write(crtl_huf_bit_read_writer_t *self, uint8_t bit) {
    self->offset -= self->offset ? 1 : 0;
    self->bits |= (bit & 1) << self->offset;
}


// Reset the content of the buffer.
static inline void _unused crtl_huf_bit_read_writer_reset(crtl_huf_bit_read_writer_t *self) {
    self->bits = 0;
    self->offset = 8;
}


// Initialize a new instance of the read-write buffer
// with the specified size in bytes.
crtl_huf_error_t crtl_huf_bufio_read_writer_init(crtl_huf_bufio_read_writer_t **self,
                                                     crtl_huf_read_writer_t *read_writer,
                                                     size_t size);


// Release memory occupied by the read-write buffer.
crtl_huf_error_t crtl_huf_bufio_read_writer_free(crtl_huf_bufio_read_writer_t **self);


// Flush the writer buffer.
crtl_huf_error_t crtl_huf_bufio_read_writer_flush(crtl_huf_bufio_read_writer_t *self);


// Write the specified amount of bytes starting from the
// provided pointer into the writer buffer. If the buffer
// will be filled during the copying of bytes, it could be
// flushed.
crtl_huf_error_t crtl_huf_bufio_write(crtl_huf_bufio_read_writer_t *self, const void *buf, size_t size);


// Read the specified amount of bytes from the reader buffer
// starting from the provided pointer.
crtl_huf_error_t crtl_huf_bufio_read(crtl_huf_bufio_read_writer_t *self, void *buf, size_t size);


// Read the 8-bits word from the reader buffer into the
// specified pointer.
crtl_huf_error_t crtl_huf_bufio_read_uint8(crtl_huf_bufio_read_writer_t *self, uint8_t *byte);


// Write the specified 8-bits word into the writer buffer.
crtl_huf_error_t crtl_huf_bufio_write_uint8(crtl_huf_bufio_read_writer_t *self, uint8_t byte);


// Initialize a new instance of the configuration.
crtl_huf_error_t crtl_huf_config_init(crtl_huf_config_t **self);


// Release memory occupied by the configuration.
crtl_huf_error_t crtl_huf_config_free(crtl_huf_config_t **self);


// Initialize a new instance of the Huffman-decoder.
crtl_huf_error_t crtl_huf_decoder_init(crtl_huf_decoder_t **self, const crtl_huf_config_t *config);


// Release memory occupied by the Huffman-decoder.
crtl_huf_error_t crtl_huf_decoder_free(crtl_huf_decoder_t **self);


// Decodes the data according to the provided
// configuration.
crtl_huf_error_t crtl_huf_decode(const crtl_huf_config_t *config);


// Create a new instance of the Huffman encoder.
crtl_huf_error_t crtl_huf_encoder_init(crtl_huf_encoder_t **self, const crtl_huf_config_t *config);


// Release memory occupied by the Huffman encoder.
crtl_huf_error_t crtl_huf_encoder_free(crtl_huf_encoder_t **self);


// Encode the data according to the provided
// configuration.
crtl_huf_error_t crtl_huf_encode(const crtl_huf_config_t *config);



// Initialize a new instance of the frequency histogram.
crtl_huf_error_t crtl_huf_histogram_init(crtl_huf_histogram_t **self, size_t iota, size_t length);


// Release memory occupied by the frequency histogram.
crtl_huf_error_t crtl_huf_histogram_free(crtl_huf_histogram_t **self);


// Reset all collected statistics.
crtl_huf_error_t crtl_huf_histogram_reset(crtl_huf_histogram_t *self);


// Increase the appropriate element of the frequencies
// chart by one if the element was found in the specified
// buffer.
crtl_huf_error_t crtl_huf_histogram_populate(crtl_huf_histogram_t *self, void *buf, size_t len);






// Initialize a new instance of the symbol
// mapping element.
crtl_huf_error_t crtl_huf_symbol_mapping_element_init(    crtl_huf_symbol_mapping_element_t **self,
                                                           const uint8_t *coding,
                                                           size_t length);


// Release memory occupied by the symbol
// mapping element.
crtl_huf_error_t crtl_huf_symbol_mapping_element_free(    crtl_huf_symbol_mapping_element_t **self);





// Initialize a new instance of the symbol mapping.
crtl_huf_error_t crtl_huf_symbol_mapping_init( crtl_huf_symbol_mapping_t **self, size_t length);


// Release memory occupied by the symbol mapping.
crtl_huf_error_t crtl_huf_symbol_mapping_free(crtl_huf_symbol_mapping_t **self);


// Insert an element into the symbol mapping by
// the specified position.
crtl_huf_error_t crtl_huf_symbol_mapping_insert(crtl_huf_symbol_mapping_t *self,
                                                    size_t position,
                                                    crtl_huf_symbol_mapping_element_t *element);


// Retrieve values of the symbol mapping element
// by the specified position.
crtl_huf_error_t crtl_huf_symbol_mapping_get(crtl_huf_symbol_mapping_t *self,
                                                 size_t position,
                                                 crtl_huf_symbol_mapping_element_t **element);


// Reset the memory occupied by the symbol mapping.
crtl_huf_error_t crtl_huf_symbol_mapping_reset(crtl_huf_symbol_mapping_t *self);



// Write the path to the root node of the Huffman
// tree starting from the specified node into the
// buffer.
crtl_huf_error_t crtl_huf_node_to_string(const crtl_huf_node_t *self, uint8_t *buf, size_t *len);


// Initialize a new instance of the Huffman tree.
crtl_huf_error_t crtl_huf_tree_init(crtl_huf_tree_t **self);


// Release memory occupied by the Huffman tree.
crtl_huf_error_t crtl_huf_tree_free(crtl_huf_tree_t **self);


// Release the memory occupied by the Huffman
// tree nodes.
crtl_huf_error_t crtl_huf_tree_reset(crtl_huf_tree_t *self);


// De-serialize the Huffman tree into the
// provided buffer.
crtl_huf_error_t crtl_huf_tree_deserialize(crtl_huf_tree_t *self, const int16_t *buf, size_t len);


// Serialize the Huffman tree from the
// provided buffer.
crtl_huf_error_t crtl_huf_tree_serialize(crtl_huf_tree_t *self, int16_t *buf, size_t *len);





#endif /*<__CRTL_BITS_TREE_HUFFMAN_H>*/
