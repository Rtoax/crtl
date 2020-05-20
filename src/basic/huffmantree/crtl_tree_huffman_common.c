#include <unistd.h>

#include "crtl/bits/tree_huffman.h"

#include "crtl/easy/attribute.h"

static const char* __crtl_huf_error_map[] = {
    "Success",
    "Failed to allocate the memory block",
    "An invalid argument was specified to the function",
    "Failed on read/write operation",
    "Fatal failure",
    "Unknown failure"
};

// Initialize a new instance of the configuration.
crtl_huf_error_t crtl_huf_config_init(crtl_huf_config_t **self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    crtl_huf_error_t err = crtl_huf_malloc(__crtl_void_pptr_m(self),
            sizeof(crtl_huf_config_t), 1);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    __crtl_huf_routine_yield_m();
}


// Release memory occupied by the configuration.
crtl_huf_error_t crtl_huf_config_free(crtl_huf_config_t **self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    free(*self);
    *self = NULL;

    __crtl_huf_routine_yield_m();
}




// Return string representation of the specified error.
const char* crtl_huf_error_string(crtl_huf_error_t error)
{
    size_t crtl_huf_error_map_len = (sizeof(__crtl_huf_error_map) 
            / sizeof(*__crtl_huf_error_map));

    // Return "Uknown failure" if the specified error
    // values is not defined in the error list.
    if (error < 0 || error >= crtl_huf_error_map_len) {
        return __crtl_huf_error_map[crtl_huf_error_map_len-1];
    }

    // Otherwise simply return the string by the
    // position in the map.
    return __crtl_huf_error_map[error];
}



// Initialize a new instance of the read-writer.
crtl_huf_error_t crtl_huf_read_writer_init(
        crtl_huf_read_writer_t **self,
        crtl_huf_reader_t reader,
        crtl_huf_writer_t writer)
{
    __crtl_huf_routine_m();

    crtl_huf_error_t err;
    crtl_huf_read_writer_t *self_ptr;

    __crtl_huf_routine_param_m(self);

    err = crtl_huf_malloc(__crtl_void_pptr_m(self),
            sizeof(crtl_huf_read_writer_t), 1);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    self_ptr = *self;

    self_ptr->reader = reader;
    self_ptr->writer = writer;

    __crtl_huf_routine_yield_m();
}


// Release memory occupied by the read-writer.
crtl_huf_error_t crtl_huf_read_writer_free(crtl_huf_read_writer_t **self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    free(*self);
    *self = NULL;

    __crtl_huf_routine_yield_m();
}


// Write the specified amount of byte from the buffer
// starting from the *buf* pointer.
crtl_huf_error_t crtl_huf_write(crtl_huf_writer_t writer, const void *buf, size_t count)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(buf);

    size_t have_written = write(writer, buf, count);
    if (have_written < 0) {
        __crtl_huf_routine_error_m(CRTL_HUF_ERROR_READ_WRITE);
    }

    __crtl_huf_routine_yield_m();
}


// Read the specified amount of bytes into the buffer
// starting from the *buf* pointer.
crtl_huf_error_t crtl_huf_read(crtl_huf_reader_t reader, void *buf, size_t *count)
{
    __crtl_huf_routine_m();

    __crtl_huf_routine_param_m(buf);
    __crtl_huf_routine_param_m(count);

    size_t have_read = read(reader, buf, *count);
    if (have_read < 0) {
        *count = 0;
        __crtl_huf_routine_error_m(CRTL_HUF_ERROR_READ_WRITE);
    }

    *count = have_read;
    __crtl_huf_routine_yield_m();
}



// Allocate the memory block of the specified size.
crtl_huf_error_t crtl_huf_malloc(void** ptr, size_t size, size_t num)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(ptr);

    *ptr = calloc(num, size);
    if (!*ptr) {
        __crtl_huf_routine_error_m(CRTL_HUF_ERROR_MEMORY_ALLOCATION);
    }

    __crtl_huf_routine_yield_m();
}

