#include <string.h>

#include <unistd.h>

#include "crtl/bits/tree_huffman.h"



// Initialize a new instance of the read-write buffer
// with the specified size in bytes.
crtl_huf_error_t
crtl_huf_bufio_read_writer_init(
        crtl_huf_bufio_read_writer_t **self,
        crtl_huf_read_writer_t *read_writer,
        size_t capacity)
{
    __crtl_huf_routine_m();

    crtl_huf_bufio_read_writer_t *self_ptr = NULL;

    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(read_writer);

    crtl_huf_error_t err = crtl_huf_malloc(__crtl_void_pptr_m(&self_ptr),
            sizeof(crtl_huf_bufio_read_writer_t), 1);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    *self = self_ptr;

    err = crtl_huf_malloc(__crtl_void_pptr_m(&self_ptr->bytes),
            sizeof(uint8_t), capacity);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    // If zero value provided for capacity, then use
    // 64 KiB buffer by default.
    if (!capacity) {
        capacity = CRTL_HUF_64KIB_BUFFER;
    }

    self_ptr->capacity = capacity;
    self_ptr->read_writer = read_writer;

    __crtl_huf_routine_yield_m();
}


// Release memory occupied by the read-write buffer.
crtl_huf_error_t
crtl_huf_bufio_read_writer_free(
        crtl_huf_bufio_read_writer_t **self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    crtl_huf_bufio_read_writer_t *self_ptr = *self;

    free(self_ptr->bytes);
    free(self_ptr);

    *self = NULL;

    __crtl_huf_routine_yield_m();
}


// Flush the writer buffer.
crtl_huf_error_t
crtl_huf_bufio_read_writer_flush(
        crtl_huf_bufio_read_writer_t *self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    // If the buffer is empty, there is nothing to do then.
    if (self->length <= 0) {
        __crtl_huf_routine_success_m();
    }

    crtl_huf_error_t err = crtl_huf_write(self->read_writer->writer,
            self->bytes, self->length);
    // Re-raise a read-write error.
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    self->length = 0;
    self->have_been_processed += self->length;

    __crtl_huf_routine_yield_m();
}


// Flush the content of the writer buffer.
static crtl_huf_error_t
__crtl_huf_bufio_read_writer_flush(
        crtl_huf_bufio_read_writer_t *self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    // Flush buffer if it is full.
    if (self->length >= self->capacity) {
        crtl_huf_error_t err = crtl_huf_write(
                self->read_writer->writer,
                self->bytes, self->length);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        // Renew byte length
        self->have_been_processed += self->length;
        self->length = 0;
    }

    __crtl_huf_routine_yield_m();
}


// Write the specified amount of bytes starting from the
// provided pointer into the writer buffer. If the buffer
// will be filled during the copying of bytes, it could be
// flushed.
crtl_huf_error_t
crtl_huf_bufio_write(
        crtl_huf_bufio_read_writer_t *self,
        const void *buf,
        size_t len)
{
    __crtl_huf_routine_m();

    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(buf);

    const uint8_t *buf_ptr = buf;

    crtl_huf_error_t err = __crtl_huf_bufio_read_writer_flush(self);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    size_t available_to_write = self->capacity - self->length;

    // If there is a data in buffer, then copy data from
    // specified buffer and dump it to writer.
    if (self->length && len >= available_to_write) {
        memcpy(self->bytes + self->length,
                buf_ptr, available_to_write);

        // Next call could fail, so increase length of the buffer.
        self->length = self->capacity;

        err = crtl_huf_write(self->read_writer->writer,
                self->bytes, self->capacity);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        buf_ptr += available_to_write;
        len -= available_to_write;

        self->length = 0;
        self->have_been_processed += self->capacity;
    }

    // All other data dump to writer withot copying into buffer.
    while (len >= self->capacity) {
        err = crtl_huf_write(self->read_writer->writer,
                buf_ptr, self->capacity);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        buf_ptr += self->capacity;
        len -= self->capacity;

        self->have_been_processed += self->capacity;
    }

    memcpy(self->bytes + self->length, buf_ptr, len);

    self->length += len;
    self->have_been_processed += len;

    __crtl_huf_routine_yield_m();
}


// Read the specified amount of bytes from the reader buffer
// starting from the provided pointer.
crtl_huf_error_t
crtl_huf_bufio_read(
        crtl_huf_bufio_read_writer_t *self,
        void *buf, size_t len)
{
    __crtl_huf_routine_m();

    crtl_huf_error_t err;
    uint8_t *buf_ptr = buf;

    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(buf);

    size_t len_copy = len;

    // Get count of available in buffer bytes.
    size_t available_to_read = self->length - self->offset;

    // If there is a data in buffer, then copy it to
    // destination buffer.
    if (available_to_read > 0 && len > 0) {
        size_t bytes_to_copy = available_to_read;

        if (available_to_read > len) {
            bytes_to_copy = len;
        }

        // Copy as much bytes as available.
        memcpy(buf_ptr, self->bytes + self->offset, bytes_to_copy);

        // Recalculate the offset and update buffer pointer.
        self->offset += bytes_to_copy;
        buf_ptr += bytes_to_copy;

        // Update length value, so we could continue to fill the bytes.
        len -= bytes_to_copy;
    }

    // If request length was satisfied, then simply exit.
    if (len <= 0) {
        // Leave the routinetion with CRTL_HUF_ERROR_SUCCESS.
        __crtl_huf_routine_success_m();
    }

    // If there is still data required to read and it is larger
    // than buffer capacity, then just read it directly to the
    // destination buffer.
    if (len >= self->capacity) {
        err = crtl_huf_read(self->read_writer->reader, buf_ptr, &len);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        self->length = 0;
        self->offset = 0;

        // Leave the routinetion with CRTL_HUF_ERROR_SUCCESS.
        __crtl_huf_routine_success_m();
    }

    // Try to fill available buffer.
    self->length = self->capacity;

    // In case when buffer size is larger that requested
    // data. Read bytes into buffer first and then copy
    // bytes to the destination.
    err = crtl_huf_read(self->read_writer->reader,
            self->bytes, &self->length);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    memcpy(buf_ptr, self->bytes, len);
    self->offset = len;

    __crtl_huf_routine_ensure_m();

    // Update integral counter when the routine was
    // interrupted due to unexpected error.
    if (!__crtl_huf_routine_violation_m()) {
        self->have_been_processed += len_copy;
    }

    __crtl_huf_routine_defer_m();
}


// Read the 8-bits word from the reader buffer into the
// specified pointer.
crtl_huf_error_t
crtl_huf_bufio_read_uint8(
        crtl_huf_bufio_read_writer_t *self,
        uint8_t *byte)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(byte);

    crtl_huf_error_t err = crtl_huf_bufio_read(
            self, byte, sizeof(uint8_t));
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    __crtl_huf_routine_yield_m();
}


// Write the specified 8-bits word into the writer buffer.
crtl_huf_error_t
crtl_huf_bufio_write_uint8(
        crtl_huf_bufio_read_writer_t *self,
        uint8_t byte)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    // Flush buffer if it is full.
    crtl_huf_error_t err = __crtl_huf_bufio_read_writer_flush(self);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    // Put byte into the buffer.
    self->bytes[self->length] = byte;
    self->length++;

    __crtl_huf_routine_yield_m();
}


