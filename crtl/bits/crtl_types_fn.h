#ifndef __CRTL_BITS_TYPES_FN_H
#define __CRTL_BITS_TYPES_FN_H 1



typedef int (*crtl_cmp_fn_t)(const void *, const void *);
typedef void (*crtl_unary_fn_t)(const void*, void*);
typedef void (*crtl_binary_fn_t)(const void*, const void*, void*);
typedef void (*crtl_swap_fn_t)(void *a, void *b, int size);

typedef void* (*crtl_malloc_fn_t)(size_t size);
typedef void* (*crtl_realloc_fn_t)(void* ptr, size_t size);
typedef void* (*crtl_calloc_fn_t)(size_t count, size_t size);
typedef void (*crtl_free_fn_t)(void* ptr);



#endif /*<__CRTL_BITS_TYPES_FN_H>*/

