#include <string.h>
#include <unistd.h>

#include "crtl/bits/tree_huffman.h"


// Initialize a new instance of the frequency histogram.
crtl_huf_error_t crtl_huf_histogram_init(crtl_huf_histogram_t **self,   size_t iota, size_t length)
{
    __crtl_huf_routine_m();

    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(iota);
    __crtl_huf_routine_param_m(length);

    crtl_huf_error_t err = crtl_huf_malloc(__crtl_void_pptr_m(self),
            sizeof(crtl_huf_histogram_t), 1);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    crtl_huf_histogram_t *self_ptr = *self;

    err = crtl_huf_malloc(__crtl_void_pptr_m(&self_ptr->frequencies),
            sizeof(uint64_t), length);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    self_ptr->iota = iota;
    self_ptr->length = length;
    self_ptr->start = -1;

    __crtl_huf_routine_yield_m();
}


// Release memory occupied by the frequency histogram.
crtl_huf_error_t
crtl_huf_histogram_free(crtl_huf_histogram_t **self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    crtl_huf_histogram_t *self_ptr = *self;

    free(self_ptr->frequencies);
    free(self_ptr);

    *self = NULL;

    __crtl_huf_routine_yield_m();
}


// Reset all collected statistics.
crtl_huf_error_t
crtl_huf_histogram_reset(crtl_huf_histogram_t *self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    memset(self->frequencies, 0,
            sizeof(uint64_t) * self->length);
    self->start = -1;

    __crtl_huf_routine_yield_m();
}


// Increase the appropriate element of the frequencies
// chart by one if the element was found in the specified
// buffer.
crtl_huf_error_t
crtl_huf_histogram_populate(
        crtl_huf_histogram_t *self,
        void *buf, size_t len)
{
    __crtl_huf_routine_m();

    uint8_t *buf_ptr = buf;
    uint8_t *buf_end = buf_ptr + len;

    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(buf);

    // Calculate frequencies of the symbols.
    while (buf_ptr + self->iota <= buf_end) {
        // Reset the destination variable.
        uint64_t element = 0;

        // Read the next element into 64 bit variable.
        memcpy(&element, buf_ptr, self->iota);

        // Shift buffer offset.
        buf_ptr += self->iota;

        self->frequencies[element] += 1;

        if (element < self->start || self->start == -1) {
            self->start = element;
        }
    }

    __crtl_huf_routine_yield_m();
}

