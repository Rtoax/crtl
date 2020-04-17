#include <unistd.h>

#include "crtl/bits/crtl_tree_huffman.h"


static const char*
__huf_error_map[] = {
    "Success",
    "Failed to allocate the memory block",
    "An invalid argument was specified to the function",
    "Failed on read/write operation",
    "Fatal failure",
    "Unknown failure"
};

// Initialize a new instance of the configuration.
huf_error_t
huf_config_init(huf_config_t **self)
{
    routine_m();
    routine_param_m(self);

    huf_error_t err = huf_malloc(void_pptr_m(self),
            sizeof(huf_config_t), 1);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    routine_yield_m();
}


// Release memory occupied by the configuration.
huf_error_t
huf_config_free(huf_config_t **self)
{
    routine_m();
    routine_param_m(self);

    free(*self);
    *self = NULL;

    routine_yield_m();
}




// Return string representation of the specified error.
const char*
huf_error_string(huf_error_t error)
{
    size_t huf_error_map_len = (sizeof(__huf_error_map) 
            / sizeof(*__huf_error_map));

    // Return "Uknown failure" if the specified error
    // values is not defined in the error list.
    if (error < 0 || error >= huf_error_map_len) {
        return __huf_error_map[huf_error_map_len-1];
    }

    // Otherwise simply return the string by the
    // position in the map.
    return __huf_error_map[error];
}



// Initialize a new instance of the read-writer.
huf_error_t
huf_read_writer_init(
        huf_read_writer_t **self,
        huf_reader_t reader,
        huf_writer_t writer)
{
    routine_m();

    huf_error_t err;
    huf_read_writer_t *self_ptr;

    routine_param_m(self);

    err = huf_malloc(void_pptr_m(self),
            sizeof(huf_read_writer_t), 1);
    if (err != HUF_ERROR_SUCCESS) {
        routine_error_m(err);
    }

    self_ptr = *self;

    self_ptr->reader = reader;
    self_ptr->writer = writer;

    routine_yield_m();
}


// Release memory occupied by the read-writer.
huf_error_t
huf_read_writer_free(huf_read_writer_t **self)
{
    routine_m();
    routine_param_m(self);

    free(*self);
    *self = NULL;

    routine_yield_m();
}


// Write the specified amount of byte from the buffer
// starting from the *buf* pointer.
huf_error_t
huf_write(huf_writer_t writer, const void *buf, size_t count)
{
    routine_m();
    routine_param_m(buf);

    size_t have_written = write(writer, buf, count);
    if (have_written < 0) {
        routine_error_m(HUF_ERROR_READ_WRITE);
    }

    routine_yield_m();
}


// Read the specified amount of bytes into the buffer
// starting from the *buf* pointer.
huf_error_t
huf_read(huf_reader_t reader, void *buf, size_t *count)
{
    routine_m();

    routine_param_m(buf);
    routine_param_m(count);

    size_t have_read = read(reader, buf, *count);
    if (have_read < 0) {
        *count = 0;
        routine_error_m(HUF_ERROR_READ_WRITE);
    }

    *count = have_read;
    routine_yield_m();
}



// Allocate the memory block of the specified size.
huf_error_t
huf_malloc(void** ptr, size_t size, size_t num)
{
    routine_m();
    routine_param_m(ptr);

    *ptr = calloc(num, size);
    if (!*ptr) {
        routine_error_m(HUF_ERROR_MEMORY_ALLOCATION);
    }

    routine_yield_m();
}

