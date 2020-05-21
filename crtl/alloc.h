#ifndef __CRTL_ALLOC_H
#define __CRTL_ALLOC_H 1

#include <malloc.h>


typedef void* (*crtl_malloc_fn_t)(size_t size);
typedef void* (*crtl_realloc_fn_t)(void* ptr, size_t size);
typedef void* (*crtl_calloc_fn_t)(size_t count, size_t size);
typedef void (*crtl_free_fn_t)(void* ptr);


/**
 * malloc hook 
 * @param size: size of memory
 * @return address of memory
 */
void* crtl_malloc(size_t size);

/**
 * free hook
 * @param ptr: memory to free
 * @return void
 */
void crtl_free(void* ptr);

/**
 * calloc hook
 * @param count: number of size
 * @param size: size of memory chunk
 * @return address of memory
 */
void* crtl_calloc(size_t count, size_t size);

/**
 * realloc hook
 * @param ptr: address of memory
 * @param size: resize size
 * @return address of memory
 */
void* crtl_realloc(void* ptr, size_t size);

/**
 * replace of memory allocator
 * @param malloc_func:  replace malloc
 * @param realloc_func: replace realloc
 * @param calloc_func:  replace calloc
 * @param free_func:    replace free
 * @return CRTL_ERROR or CRTL_SUCCESS
 */
int crtl_replace_allocator(crtl_malloc_fn_t malloc_func, crtl_realloc_fn_t realloc_func,
                              crtl_calloc_fn_t calloc_func, crtl_free_fn_t free_func);


/**
 *  分配内存 一维、二维、三维...，分配后可以通过数组形式访问
 *  int **I = crtl_malloc2(2,3, sizeof(int));
 *  I[1][2] = 12;
 */
/**
 *  分配一维数组
 */
/**
 * alloc memory of multi array
 * @param n1:   size of index of first dimension
 * @param n2:   size of index of second dimension
 * @param n3:   size of index of third dimension
 * @param n4:   size of index of fouth dimension
 * @param n5:   size of index of fifth dimension
 * @param n6:   size of index of sixth dimension
 * @return address of memory
 */
void *crtl_malloc1 (int n1, int size);
void **crtl_malloc2 (int n1, int n2, int size);
void ***crtl_malloc3 (int n1, int n2, int n3, int size);
void ****crtl_malloc4 (int n1, int n2, int n3, int n4, int size);
void *****crtl_malloc5 (int n1, int n2, int n3, int n4, int n5, int size);
void ******crtl_malloc6 (int n1, int n2, int n3, int n4, int n5, int n6, int size);


/**
 * remalloc memory of multi array
 * @param v:   address of memory
 * @param n1:   size of index of first dimension
 * @param n2:   size of index of second dimension
 * @param n3:   size of index of third dimension
 * @param n4:   size of index of fouth dimension
 * @param n5:   size of index of fifth dimension
 * @param n6:   size of index of sixth dimension
 * @return address of memory
 */
void *crtl_remalloc1(void *v, int n1, int size);
void **crtl_remalloc2(void **v, int n1, int n2, int size);
void ***crtl_remalloc3 (void ***v, int n1, int n2, int n3, int size);
void ****crtl_remalloc4 (void ****v, int n1, int n2, int n3, int n4, int size);
void *****crtl_remalloc5 (void *****v, int n1, int n2, int n3, int n4, int n5, int size);
void ******crtl_remalloc6 (void ******v, int n1, int n2, int n3, int n4, int n5, int n6, int size);


/**
 *  释放内存 一维、二维、三维...
 *  
 */
/**
 * free memory of multi array
 * @param p:   address of memory
 * @return void
 */
void crtl_mfree6 (void ******p);
void crtl_mfree5 (void *****p);
void crtl_mfree4 (void ****p);
void crtl_mfree3 (void ***p);
void crtl_mfree2 (void **p);
void crtl_mfree1 (void *p);


#endif /*<__CRTL_ALLOC_H>*/


