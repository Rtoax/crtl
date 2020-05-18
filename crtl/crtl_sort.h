#ifndef __CRTL_SORT_H
#define __CRTL_SORT_H 1



#include "crtl/bits/crtl_types_basic.h"

#include "crtl/easy/attribute.h"



/**
 * sort - sort an array of elements with some algorithm
 * @param base: pointer to data to sort
 * @param num: number of elements
 * @param size: size of each element
 * @param cmp: pointer to comparison function
 * @return void
 */
_api void crtl_sort_cocktailshaker(void *base, size_t num, size_t size, int (*cmp)(const void *, const void *));
_api void crtl_sort_heap(void *base, size_t num,size_t size, int (*cmp)(const void *, const void *));
_api void crtl_sort_insertion(void *base, size_t num, size_t size, int (*cmp)(const void *, const void *));
_api void crtl_sort_qsort3way(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *));
_api void crtl_sort_qsortH(void *base,size_t lo,size_t hi,size_t size,int (*cmp)(const void *, const void *));
_api void crtl_sort_qsortL(void *base,size_t lo,size_t hi,size_t size,int (*cmp)(const void *, const void *));
_api void crtl_sort_selection(void *base,size_t num,size_t size,int (*cmp)(const void *, const void *));
_api void crtl_sort_shell(void *base,size_t num,size_t size,int (*cmp)(const void *, const void *));


/**
 * sort - sort an array of elements
 * @param base: pointer to data to sort
 * @param num: number of elements
 * @param size: size of each element
 * @param cmp_func: pointer to comparison function
 * @param swap_func: pointer to swap function or NULL
 * @return void
 *
 * This function does a heapsort on the given array.  You may provide
 * a swap_func function if you need to do something more than a memory
 * copy (e.g. fix up pointers or auxiliary data), but the built-in swap
 * avoids a slow retpoline and so is significantly faster.
 *
 * Sorting time is O(n log n) both on average and worst-case. While
 * quicksort is slightly faster on average, it suffers from exploitable
 * O(n*n) worst-case behavior and extra memory requirements that make
 * it less suitable for kernel use.
 */
_api void crtl_sort(void *base, size_t num, size_t size, int (*cmp_func)(const void *, const void *), 
                                                    void (*swap_func)(void *, void *, int size));





#endif /*<__CRTL_SORT_H>*/
