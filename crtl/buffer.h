#ifndef __CRTL_BITS_TYPES_BUFFER_H
#define __CRTL_BITS_TYPES_BUFFER_H 1

#include <sys/types.h>

/**
 * buffer handle
 */
typedef void *crtl_buffer_t;

/**
 * create buffer 
 * @param void
 * @return type of crtl_buffer_t
 */
crtl_buffer_t crtl_buffer_head_init(void);

/**
 * create buffer 
 * @param buffer: buffer type, created by crtl_buffer_head_init
 * @param size: buffer node size
 * @return buffer node address
 */
void *crtl_buffer_node_new(crtl_buffer_t buffer, size_t size);

/**
 * grow buffer node size
 * @param buffer: buffer node address
 * @param size: buffer node new size
 * @return buffer node address
 */
void *crtl_buffer_node_grow(void *buf, size_t size);

/**
 * destroy buffer node
 * @param buffer: buffer node address
 * @return void
 */
void crtl_buffer_free(crtl_buffer_t buffer);


#endif /*<__CRTL_BITS_TYPES_BUFFER_H>*/
