#include <string.h>
#include <unistd.h>

#include "crtl/bits/crtl_tree_huffman.h"


// Initialize a new instance of the symbol
// mapping element.
crtl_huf_error_t
crtl_huf_symbol_mapping_element_init(
        crtl_huf_symbol_mapping_element_t **self,
        const uint8_t *coding,
        size_t length)
{
    __crtl_huf_routine_m();

    crtl_huf_error_t err;
    crtl_huf_symbol_mapping_element_t *self_ptr;

    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(coding);

    err = crtl_huf_malloc(__crtl_void_pptr_m(self),
            sizeof(crtl_huf_symbol_mapping_element_t), 1);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    self_ptr = *self;

    err = crtl_huf_malloc(__crtl_void_pptr_m(&self_ptr->coding),
            sizeof(uint8_t), length + 1);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    self_ptr->length = length;
    memcpy(self_ptr->coding, coding, length);

    __crtl_huf_routine_yield_m();
}


// Release memory occupied by the symbol
// mapping element.
crtl_huf_error_t
crtl_huf_symbol_mapping_element_free(
        crtl_huf_symbol_mapping_element_t **self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    crtl_huf_symbol_mapping_element_t *self_ptr = *self;

    free(self_ptr->coding);
    free(self_ptr);

    *self = NULL;

    __crtl_huf_routine_yield_m();
}


// Initialize a new instance of the symbol mapping.
crtl_huf_error_t
crtl_huf_symbol_mapping_init(
        crtl_huf_symbol_mapping_t **self,
        size_t length)
{
    __crtl_huf_routine_m();

    crtl_huf_error_t err;
    crtl_huf_symbol_mapping_t *self_ptr;

    __crtl_huf_routine_param_m(self);

    err = crtl_huf_malloc(__crtl_void_pptr_m(self),
            sizeof(crtl_huf_symbol_mapping_t), 1);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    self_ptr = *self;

    err = crtl_huf_malloc(__crtl_void_pptr_m(&self_ptr->symbols),
            sizeof(crtl_huf_symbol_mapping_element_t*), length);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    self_ptr->length = length;

    __crtl_huf_routine_yield_m();
}


// Reset memory occipied by the symbol mapping.
static crtl_huf_error_t
__crtl_huf_symbol_mapping_free(
        crtl_huf_symbol_mapping_t *self)
{
    __crtl_huf_routine_m();

    crtl_huf_error_t err;
    crtl_huf_symbol_mapping_element_t *element;

    size_t index;

    __crtl_huf_routine_param_m(self);

    for (index = 0; index < self->length; index++) {
        element = self->symbols[index];

        if (!element) {
            continue;
        }

        err = crtl_huf_symbol_mapping_element_free(&element);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }

        self->symbols[index] = NULL;
    }

    __crtl_huf_routine_yield_m();
}


// Release memory occupied by the symbol mapping.
crtl_huf_error_t
crtl_huf_symbol_mapping_free(
        crtl_huf_symbol_mapping_t **self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    crtl_huf_symbol_mapping_t *self_ptr = *self;

    crtl_huf_error_t err = __crtl_huf_symbol_mapping_free(self_ptr);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    free(self_ptr->symbols);
    free(self_ptr);

    *self = NULL;

    __crtl_huf_routine_yield_m();
}


// Insert an element into the symbol mapping by
// the specified position.
crtl_huf_error_t
crtl_huf_symbol_mapping_insert(
        crtl_huf_symbol_mapping_t *self,
        size_t position,
        crtl_huf_symbol_mapping_element_t *element)
{
    __crtl_huf_routine_m();

    crtl_huf_error_t err;
    crtl_huf_symbol_mapping_element_t *previous_element;

    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(element);

    __crtl_huf_routine_inrange_m(position, 0, self->length - 1);

    previous_element = self->symbols[position];

    if (previous_element) {
        err = crtl_huf_symbol_mapping_element_free(
                &previous_element);
        if (err != CRTL_HUF_ERROR_SUCCESS) {
            __crtl_huf_routine_error_m(err);
        }
    }

    self->symbols[position] = element;

    __crtl_huf_routine_yield_m();
}


// Retrieve values of the symbol mapping element
// by the specified position.
crtl_huf_error_t
crtl_huf_symbol_mapping_get(
        crtl_huf_symbol_mapping_t *self,
        size_t position,
        crtl_huf_symbol_mapping_element_t **element)
{
    __crtl_huf_routine_m();

    __crtl_huf_routine_param_m(self);
    __crtl_huf_routine_param_m(element);

    __crtl_huf_routine_inrange_m(position, 0, self->length - 1);
    *element = self->symbols[position];

    __crtl_huf_routine_yield_m();
}


// Reset the memory occupied by the symbol mapping.
crtl_huf_error_t
crtl_huf_symbol_mapping_reset(crtl_huf_symbol_mapping_t *self)
{
    __crtl_huf_routine_m();
    __crtl_huf_routine_param_m(self);

    crtl_huf_error_t err = __crtl_huf_symbol_mapping_free(self);
    if (err != CRTL_HUF_ERROR_SUCCESS) {
        __crtl_huf_routine_error_m(err);
    }

    __crtl_huf_routine_yield_m();
}


