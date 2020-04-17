#ifndef __CRTL_BITS_TREE_HUFFMAN_H
#define __CRTL_BITS_TREE_HUFFMAN_H 1


#ifndef INCLUDE_huffman_sys_h__
#define INCLUDE_huffman_sys_h__


#include <stdio.h>

#define void_pptr_m(pointer) ((void**)(pointer))


// Initialize the function routine.
#define routine_m() \
    huf_error_t __error = HUF_ERROR_SUCCESS \


// Set the routine finalization label.
#define routine_ensure_m() \
    ensure: \


// Leave the function routine.
#define routine_defer_m() \
    do { \
        return __error; \
    } while(0) \


// Leave the function routine without finalization.
#define routine_yield_m() \
    do { \
        routine_ensure_m(); \
        return __error; \
    } while (0) \


// Validate that provided parameter is not equal to the nil.
#define routine_param_m(param) \
    do { \
        if ((param) == 0) { \
            __error = HUF_ERROR_INVALID_ARGUMENT; \
            goto ensure; \
        } \
    } while (0) \


// Validate that specified parameter is satisfying
// criteria: low < value < high, and throw error otherwise.
#define routine_inrange_m(value, low, high) \
    do { \
        if ((value) < (low) || (value) > (high)) { \
            __error = HUF_ERROR_INVALID_ARGUMENT; \
            goto ensure; \
        } \
    } while(0) \


// Leave the function with the provided error.
#define routine_error_m(error) \
    do { \
        __error = (error); \
        goto ensure; \
    } while (0) \


// Leave the function with HUF_ERROR_SUCCESS.
#define routine_success_m() \
    routine_error_m(HUF_ERROR_SUCCESS) \


// Return true when the routine interrupted with
// an error.
#define routine_violation_m()\
    (__error != HUF_ERROR_SUCCESS) \


#endif // INCLUDE_huffman_sys_h__


#ifndef INCLUDE_huffman_common_h__
#define INCLUDE_huffman_common_h__

#include <stdint.h>
#include <stdlib.h>


#define HUF_1KIB_BUFFER 1024

#define HUF_64KIB_BUFFER 65536

#define HUF_128KIB_BUFFER 131072

#define HUF_256KIB_BUFFER 262144

#define HUF_512KIB_BUFFER 524288

#define HUF_1MIB_BUFFER 1048576


#endif // INCLUDE_huffman_common_h__


#ifndef INCLUDE_huffman_errors_h__
#define INCLUDE_huffman_errors_h__


// Enumeration of the possible errors.
typedef enum {
    // The call completed successfully.
    HUF_ERROR_SUCCESS,

    // Failed to allocate the memory block.
    HUF_ERROR_MEMORY_ALLOCATION,

    // The specified argument is invalid,
    // e.g. the pointer is equal to nil.
    HUF_ERROR_INVALID_ARGUMENT,

    // The failure caused by input/output error.
    HUF_ERROR_READ_WRITE,

    // Failure caused by Unrecoverable error.
    HUF_ERROR_FATAL,
} huf_error_t;


// Return string representation of the specified error.
const char* huf_error_string(huf_error_t error);


#endif // INCLUDE_huffman_errors_h__

#ifndef INCLUDE_huffman_io_h__
#define INCLUDE_huffman_io_h__


// huf_writer_t is a writer abstraction.
typedef int huf_writer_t;


// huf_reader_r is a reader abstraction.
typedef int huf_reader_t;


// huf_read_writer_t groups reader and
// writer abstractions.
typedef struct __huf_read_writer {
    // A reader instance.
    huf_reader_t reader;

    // A writer instance.
    huf_writer_t writer;
} huf_read_writer_t;


// Initialize a new instance of the read-writer.
huf_error_t
huf_read_writer_init(
        huf_read_writer_t **self,
        huf_reader_t reader,
        huf_writer_t writer);


// Release memory occupied by the read-writer.
huf_error_t
huf_read_writer_free(
        huf_read_writer_t **self);


// Write the specified amount of byte from the buffer
// starting from the *buf* pointer.
huf_error_t
huf_write(huf_writer_t writer, const void *buf, size_t count);


// Read the specified amount of bytes into the buffer
// starting from the *buf* pointer.
huf_error_t
huf_read(huf_reader_t reader, void *buf, size_t *count);


#endif // INCLUDE_huffman_io_h__



#ifndef INCLUDE_huffman_malloc_h__
#define INCLUDE_huffman_malloc_h__


// Allocate the memory block of the specified size.
huf_error_t
huf_malloc(void** ptr, size_t size, size_t num);


#endif // INCLUDE_huffman_malloc_h__

#ifndef INCLUDE_huffman_bufio_h__
#define INCLUDE_huffman_bufio_h__


// huf_bufio_read_writer_t represents read/writer buffer.
typedef struct __huf_bufio_read_writer {
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
    huf_read_writer_t *read_writer;
} huf_bufio_read_writer_t;


// huf_bit_read_writer_t represents read/writer bit buffer.
typedef struct __huf_bit_read_writer {
    // Bit sized read/write buffer.
    uint8_t bits;

    // Current offset for bit buffer.
    uint8_t offset;
} huf_bit_read_writer_t;


// Write the first bit of the specified word into the
// bit buffer.
static inline void
huf_bit_write(huf_bit_read_writer_t *self, uint8_t bit)
{
    self->offset -= self->offset ? 1 : 0;
    self->bits |= (bit & 1) << self->offset;
}


// Reset the content of the buffer.
static inline void
huf_bit_read_writer_reset(huf_bit_read_writer_t *self)
{
    self->bits = 0;
    self->offset = 8;
}


// Initialize a new instance of the read-write buffer
// with the specified size in bytes.
huf_error_t
huf_bufio_read_writer_init(
        huf_bufio_read_writer_t **self,
        huf_read_writer_t *read_writer,
        size_t size);


// Release memory occupied by the read-write buffer.
huf_error_t
huf_bufio_read_writer_free(
        huf_bufio_read_writer_t **self);


// Flush the writer buffer.
huf_error_t
huf_bufio_read_writer_flush(
        huf_bufio_read_writer_t *self);


// Write the specified amount of bytes starting from the
// provided pointer into the writer buffer. If the buffer
// will be filled during the copying of bytes, it could be
// flushed.
huf_error_t
huf_bufio_write(
        huf_bufio_read_writer_t *self,
        const void *buf, size_t size);


// Read the specified amount of bytes from the reader buffer
// starting from the provided pointer.
huf_error_t
huf_bufio_read(
        huf_bufio_read_writer_t *self,
        void *buf, size_t size);


// Read the 8-bits word from the reader buffer into the
// specified pointer.
huf_error_t
huf_bufio_read_uint8(
        huf_bufio_read_writer_t *self,
        uint8_t *byte);


// Write the specified 8-bits word into the writer buffer.
huf_error_t
huf_bufio_write_uint8(
        huf_bufio_read_writer_t *self,
        uint8_t byte);


#endif // INCLUDE_huffman_bufio_h__

#ifndef INCLUDE_huffman_config_h__
#define INCLUDE_huffman_config_h__


typedef struct __huf_encoder_config {
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
    huf_reader_t reader;

    // Instance of the writer which will be used as
    // a consumer of the Huffman-encoded data.
    huf_writer_t writer;
} huf_config_t;


// Initialize a new instance of the configuration.
huf_error_t
huf_config_init(huf_config_t **self);


// Release memory occupied by the configuration.
huf_error_t
huf_config_free(huf_config_t **self);


#endif // INCLUDE_huffman_config_h__

#ifndef INCLUDE_huffman_decoder_h__
#define INCLUDE_huffman_decoder_h__



// A Huffman decoding context.
typedef struct __huf_decoder huf_decoder_t;


// Initialize a new instance of the Huffman-decoder.
huf_error_t
huf_decoder_init(
        huf_decoder_t **self,
        const huf_config_t *config);


// Release memory occupied by the Huffman-decoder.
huf_error_t
huf_decoder_free(huf_decoder_t **self);


// Decodes the data according to the provided
// configuration.
huf_error_t
huf_decode(const huf_config_t *config);


#endif // INCLUDE_huffman_decoder_h__

#ifndef INCLUDE_huffman_encoder_h__
#define INCLUDE_huffman_encoder_h__


// A Huffman encoding context.
typedef struct __huf_encoder huf_encoder_t;


// Create a new instance of the Huffman encoder.
huf_error_t
huf_encoder_init(
        huf_encoder_t **self,
        const huf_config_t *config);


// Release memory occupied by the Huffman encoder.
huf_error_t
huf_encoder_free(huf_encoder_t **self);


// Encode the data according to the provided
// configuration.
huf_error_t
huf_encode(const huf_config_t *config);


#endif // INCLUDE_huffman_encoder_h__


#ifndef INCLUDE_huffman_histogram_h__
#define INCLUDE_huffman_histogram_h__



// A frequency histogram.
typedef struct __huf_histogram {
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
} huf_histogram_t;


// Initialize a new instance of the frequency histogram.
huf_error_t
huf_histogram_init(
        huf_histogram_t **self,
        size_t iota, size_t length);


// Release memory occupied by the frequency histogram.
huf_error_t
huf_histogram_free(huf_histogram_t **self);


// Reset all collected statistics.
huf_error_t
huf_histogram_reset(huf_histogram_t *self);


// Increase the appropriate element of the frequencies
// chart by one if the element was found in the specified
// buffer.
huf_error_t
huf_histogram_populate(
        huf_histogram_t *self,
        void *buf, size_t len);


#endif // INCLUDE_huffman_histogram_h__

#ifndef INCLUDE_huffman_symbol_h__
#define INCLUDE_huffman_symbol_h__


// An element of the symbol mapping.
typedef struct __huf_symbol_mapping_element {
    // Length of the symbol coding.
    size_t length;

    // Binary symbol coding.
    uint8_t *coding;
} huf_symbol_mapping_element_t;


// Initialize a new instance of the symbol
// mapping element.
huf_error_t
huf_symbol_mapping_element_init(
        huf_symbol_mapping_element_t **self,
        const uint8_t *coding,
        size_t length);


// Release memory occupied by the symbol
// mapping element.
huf_error_t
huf_symbol_mapping_element_free(
        huf_symbol_mapping_element_t **self);


// A symbol mapping.
typedef struct __huf_symbol_mapping {
    // Count of the elements in the mapping.
    size_t length;

    // Array of the symbols encodings.
    huf_symbol_mapping_element_t **symbols;
} huf_symbol_mapping_t;


// Initialize a new instance of the symbol mapping.
huf_error_t
huf_symbol_mapping_init(
        huf_symbol_mapping_t **self,
        size_t length);


// Release memory occupied by the symbol mapping.
huf_error_t
huf_symbol_mapping_free(
        huf_symbol_mapping_t **self);


// Insert an element into the symbol mapping by
// the specified position.
huf_error_t
huf_symbol_mapping_insert(
        huf_symbol_mapping_t *self,
        size_t position,
        huf_symbol_mapping_element_t *element);


// Retrieve values of the symbol mapping element
// by the specified position.
huf_error_t
huf_symbol_mapping_get(
        huf_symbol_mapping_t *self,
        size_t position,
        huf_symbol_mapping_element_t **element);


// Reset the memory occupied by the symbol mapping.
huf_error_t
huf_symbol_mapping_reset(
        huf_symbol_mapping_t *self);


#endif // INCLUDE_huffman_symbol_h__


#ifndef INCLUDE_huffman_tree_h__
#define INCLUDE_huffman_tree_h__


// The count of ASCII symbols
#define HUF_ASCII_COUNT 256

// Maximum length of the 2-byte serialized Huffman tree.
#define HUF_BTREE_LEN 1024

// Length of the symbols frequency histogram.
#define HUF_HISTOGRAM_LEN 512

// All leaves of the Huffman tree will be marked with that value.
#define HUF_LEAF_NODE -1


// A node of the Huffman tree.
typedef struct __huf_node {
    // The weight of the node.
    int16_t index;

    // The pointer to the parent node.
    struct __huf_node *parent;

    // The pointer to the left child node.
    struct __huf_node *left;

    // The pointer to the right child node.
    struct __huf_node *right;
} huf_node_t;


// Write the path to the root node of the Huffman
// tree starting from the specified node into the
// buffer.
huf_error_t
huf_node_to_string(
        const huf_node_t *self,
        uint8_t *buf, size_t *len);


// A Huffman tree.
typedef struct __huf_tree {
    // List of Huffman tree leaves.
    huf_node_t **leaves;

    // Root element of the Huffman tree.
    huf_node_t *root;
} huf_tree_t;


// Initialize a new instance of the Huffman tree.
huf_error_t
huf_tree_init(huf_tree_t **self);


// Release memory occupied by the Huffman tree.
huf_error_t
huf_tree_free(huf_tree_t **self);


// Release the memory occupied by the Huffman
// tree nodes.
huf_error_t
huf_tree_reset(huf_tree_t *self);


// De-serialize the Huffman tree into the
// provided buffer.
huf_error_t
huf_tree_deserialize(
        huf_tree_t *self,
        const int16_t *buf, size_t len);


// Serialize the Huffman tree from the
// provided buffer.
huf_error_t
huf_tree_serialize(
        huf_tree_t *self,
        int16_t *buf, size_t *len);


#endif // INCLUDE_huffman_tree_h__



#endif /*<__CRTL_BITS_TREE_HUFFMAN_H>*/
