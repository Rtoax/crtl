#ifndef __CRTL_SORT_H
#define __CRTL_SORT_H 1



#include "crtl/bits/crtl_types_basic.h"


void crtl_sort_cocktailshaker(void *base, size_t num, size_t size, int (*cmp)(const void *, const void *));
void crtl_sort_heap(void *base, size_t num,size_t size, int (*cmp)(const void *, const void *));
void crtl_sort_insertion(void *base, size_t num, size_t size, int (*cmp)(const void *, const void *));
void crtl_sort_qsort3way(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *));
void crtl_sort_qsortH(void *base,size_t lo,size_t hi,size_t size,int (*cmp)(const void *, const void *));
void crtl_sort_qsortL(void *base,size_t lo,size_t hi,size_t size,int (*cmp)(const void *, const void *));
void crtl_sort_selection(void *base,size_t num,size_t size,int (*cmp)(const void *, const void *));
void crtl_sort_shell(void *base,size_t num,size_t size,int (*cmp)(const void *, const void *));
void crtl_sort(void *base, size_t num, size_t size, int (*cmp_func)(const void *, const void *), 
                                                    void (*swap_func)(void *, void *, int size));





#endif /*<__CRTL_SORT_H>*/
