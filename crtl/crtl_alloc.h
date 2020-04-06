#ifndef __CRTL_ALLOC_H
#define __CRTL_ALLOC_H 1

#include <malloc.h>

/**
 *  分配内存 一维、二维、三维...，分配后可以通过数组形式访问
 *  int **I = crtl_malloc2(2,3, sizeof(int));
 *  I[1][2] = 12;
 */
/**
 *  分配一维数组
 */
void *crtl_malloc1 (int n1, int size);
void *crtl_remalloc1(void *v, int n1, int size);
void **crtl_malloc2 (int n1, int n2, int size);
void ***crtl_malloc3 (int n1, int n2, int n3, int size);
void ****crtl_malloc4 (int n1, int n2, int n3, int n4, int size);
void *****crtl_malloc5 (int n1, int n2, int n3, int n4, int n5, int size);
void ******crtl_malloc6 (int n1, int n2, int n3, int n4, int n5, int n6, int size);

/**
 *  释放内存 一维、二维、三维...
 *  
 */
void crtl_mfree6 (void ******p);
void crtl_mfree5 (void *****p);
void crtl_mfree4 (void ****p);
void crtl_mfree3 (void ***p);
void crtl_mfree2 (void **p);
void crtl_mfree1 (void *p);


#endif /*<__CRTL_ALLOC_H>*/

