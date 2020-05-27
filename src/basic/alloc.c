#include <malloc.h>

#include "crtl/bits/types_basic.h"
#include "crypto/attribute.h"

#include "crtl/alloc.h"


typedef struct {
  crtl_malloc_fn_t local_malloc;
  crtl_realloc_fn_t local_realloc;
  crtl_calloc_fn_t local_calloc;
  crtl_free_fn_t local_free;
} crtl_allocator_t;

static crtl_allocator_t _unused __crtl_allocator = {
  malloc,
  realloc,
  calloc,
  free,
};

  



void* crtl_malloc(size_t size) 
{
  if (size > 0)
    return __crtl_allocator.local_malloc(size);
  return NULL;
}

void crtl_free(void* ptr) 
{
  __crtl_allocator.local_free(ptr);
}

void* crtl_calloc(size_t count, size_t size) 
{
  return __crtl_allocator.local_calloc(count, size);
}

void* crtl_realloc(void* ptr, size_t size) 
{
  if (size > 0)
    return __crtl_allocator.local_realloc(ptr, size);
  crtl_free(ptr);
  return NULL;
}


int crtl_replace_allocator(crtl_malloc_fn_t malloc_func, crtl_realloc_fn_t realloc_func,
                              crtl_calloc_fn_t calloc_func, crtl_free_fn_t free_func) 
{
    if (malloc_func == NULL || realloc_func == NULL ||
        calloc_func == NULL || free_func == NULL) {
        return CRTL_ERROR;
    }

    __crtl_allocator.local_malloc = malloc_func;
    __crtl_allocator.local_realloc = realloc_func;
    __crtl_allocator.local_calloc = calloc_func;
    __crtl_allocator.local_free = free_func;

    return CRTL_SUCCESS;
}



/**
 *  分配内存 一维、二维、三维...
 *  
 */
/* allocate a 1-d array */
_api void *crtl_malloc1 (int n1, int size)
{
	void *p;

	if ((p=__crtl_allocator.local_malloc(n1*size))==NULL)
		return NULL;
	return p;
}
_api void *crtl_remalloc1(void *v, int n1, int size)
{
	void *p;

	if ((p=__crtl_allocator.local_realloc(v,n1*size))==NULL)
		return NULL;
	return p;
}

/* __crtl_allocator.local_free a 1-d array */
_api void crtl_mfree1 (void *p)
{
	__crtl_allocator.local_free(p);
}

/* allocate a 2-d array */
_api void **crtl_malloc2 (int n1, int n2, int size)
{
	int i2;
	void **p;

	if ((p=(void**)__crtl_allocator.local_malloc(n2*sizeof(void*)))==NULL) 
		return NULL;
	if ((p[0]=(void*)__crtl_allocator.local_malloc(n2*n1*size))==NULL) {
		__crtl_allocator.local_free(p);
		return NULL;
	}
	for (i2=0; i2<n2; i2++)
		p[i2] = (void*)p[0]+size*n1*i2;
	return p;
}

_api void **crtl_remalloc2(void **v, int n1, int n2, int size)
{
	int i2;
	void **p;

	if ((p=(void**)__crtl_allocator.local_realloc(v, n2*sizeof(void*)))==NULL) 
		return NULL;
	if ((p[0]=(void*)__crtl_allocator.local_realloc(v[0], n2*n1*size))==NULL) {
		__crtl_allocator.local_free(p);
		return NULL;
	}
	for (i2=0; i2<n2; i2++)
		p[i2] = (void*)p[0]+size*n1*i2;
	return p;
}

/* __crtl_allocator.local_free a 2-d array */
_api void crtl_mfree2 (void **p)
{
	__crtl_allocator.local_free(p[0]);
	__crtl_allocator.local_free(p);
}

_api void ***crtl_malloc3 (int n1, int n2, int n3, int size)
{
    int i3,i2;
    void ***p;

    if ((p=(void***)__crtl_allocator.local_malloc(n3*sizeof(void**)))==NULL)
        return NULL;
    if ((p[0]=(void**)__crtl_allocator.local_malloc(n3*n2*sizeof(void*)))==NULL) {
        __crtl_allocator.local_free(p);
        return NULL;
    }
    if ((p[0][0]=(void*)__crtl_allocator.local_malloc(n3*n2*n1*size))==NULL) {
        __crtl_allocator.local_free(p[0]);
        __crtl_allocator.local_free(p);
        return NULL;
    }

    for (i3=0; i3<n3; i3++) {
        p[i3] = p[0]+n2*i3;
        for (i2=0; i2<n2; i2++)
            p[i3][i2] = (void*)p[0][0]+size*n1*(i2+n2*i3);
    }
    return p;
}


_api void ***crtl_remalloc3 (void ***v, int n1, int n2, int n3, int size)
{
    int i3,i2;
    void ***p;

    if ((p=(void***)__crtl_allocator.local_realloc(v, n3*sizeof(void**)))==NULL)
        return NULL;
    if ((p[0]=(void**)__crtl_allocator.local_realloc(v[0], n3*n2*sizeof(void*)))==NULL) {
        __crtl_allocator.local_free(p);
        return NULL;
    }
    if ((p[0][0]=(void*)__crtl_allocator.local_realloc(v[0][0], n3*n2*n1*size))==NULL) {
        __crtl_allocator.local_free(p[0]);
        __crtl_allocator.local_free(p);
        return NULL;
    }

    for (i3=0; i3<n3; i3++) {
        p[i3] = p[0]+n2*i3;
        for (i2=0; i2<n2; i2++)
            p[i3][i2] = (void*)p[0][0]+size*n1*(i2+n2*i3);
    }
    return p;
}


_api void crtl_mfree3 (void ***p)
{
	__crtl_allocator.local_free(p[0][0]);
	__crtl_allocator.local_free(p[0]);
	__crtl_allocator.local_free(p);
}
/* allocate a 4-d array */
_api void ****crtl_malloc4 (int n1, int n2, int n3, int n4, int size)
{
	int i4,i3,i2;
	void ****p;

	if ((p=(void****)__crtl_allocator.local_malloc(n4*sizeof(void***)))==NULL)
		return NULL;
	if ((p[0]=(void***)__crtl_allocator.local_malloc(n4*n3*sizeof(void**)))==NULL) {
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0]=(void**)__crtl_allocator.local_malloc(n4*n3*n2*sizeof(void*)))==NULL) {
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0]=(void*)__crtl_allocator.local_malloc(n4*n3*n2*n1*size))==NULL) {
		__crtl_allocator.local_free(p[0][0]);
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	for (i4=0; i4<n4; i4++) {
		p[i4] = p[0]+i4*n3;
		for (i3=0; i3<n3; i3++) {
			p[i4][i3] = p[0][0]+n2*(i3+n3*i4);
			for (i2=0; i2<n2; i2++)
				p[i4][i3][i2] = (char*)p[0][0][0]+
						size*n1*(i2+n2*(i3+n3*i4));
		}
	}
	return p;
}


_api void ****crtl_remalloc4 (void ****v, int n1, int n2, int n3, int n4, int size)
{
	int i4,i3,i2;
	void ****p;

	if ((p=(void****)__crtl_allocator.local_realloc(v, n4*sizeof(void***)))==NULL)
		return NULL;
	if ((p[0]=(void***)__crtl_allocator.local_realloc(v[0], n4*n3*sizeof(void**)))==NULL) {
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0]=(void**)__crtl_allocator.local_realloc(v[0][0], n4*n3*n2*sizeof(void*)))==NULL) {
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0]=(void*)__crtl_allocator.local_realloc(v[0][0][0], n4*n3*n2*n1*size))==NULL) {
		__crtl_allocator.local_free(p[0][0]);
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	for (i4=0; i4<n4; i4++) {
		p[i4] = p[0]+i4*n3;
		for (i3=0; i3<n3; i3++) {
			p[i4][i3] = p[0][0]+n2*(i3+n3*i4);
			for (i2=0; i2<n2; i2++)
				p[i4][i3][i2] = (char*)p[0][0][0]+
						size*n1*(i2+n2*(i3+n3*i4));
		}
	}
	return p;
}


/* __crtl_allocator.local_free a 4-d array */
_api void crtl_mfree4 (void ****p)
{
	__crtl_allocator.local_free(p[0][0][0]);
	__crtl_allocator.local_free(p[0][0]);
	__crtl_allocator.local_free(p[0]);
	__crtl_allocator.local_free(p);
}

/* The following two functions were added by Zhaobo Meng, Jan. 1997*/
/* allocate a 5-d array */
_api void *****crtl_malloc5 (int n1, int n2, int n3, int n4, int n5, int size)
{
	int i5,i4,i3,i2;
	void *****p;

	if ((p=(void*****)__crtl_allocator.local_malloc(n5*sizeof(void****)))==NULL)
		return NULL;
	if ((p[0]=(void****)__crtl_allocator.local_malloc(n5*n4*sizeof(void***)))==NULL) {
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0]=(void***)__crtl_allocator.local_malloc(n5*n4*n3*sizeof(void**)))==NULL) {
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0]=(void**)__crtl_allocator.local_malloc(n5*n4*n3*n2*sizeof(void*)))==NULL) {
		__crtl_allocator.local_free(p[0][0]);
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0][0]=(void*)__crtl_allocator.local_malloc(n5*n4*n3*n2*n1*size))==NULL) {
		__crtl_allocator.local_free(p[0][0][0]);
		__crtl_allocator.local_free(p[0][0]);
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	for (i5=0; i5<n5; i5++) {
		p[i5] = p[0]+n4*i5;
		for (i4=0; i4<n4; i4++) {
			p[i5][i4] = p[0][0]+n3*(i4+n4*i5);
			for (i3=0; i3<n3; i3++) {
				p[i5][i4][i3] = p[0][0][0]+n2*(i3+n3*(i4+n4*i5));
				for (i2=0; i2<n2; i2++)
					p[i5][i4][i3][i2] = (char*)p[0][0][0][0]+
						size*n1*(i2+n2*(i3+n3*(i4+n4*i5)));
			}
		}
	}
	return p;
}


_api void *****crtl_remalloc5 (void *****v, int n1, int n2, int n3, int n4, int n5, int size)
{
	int i5,i4,i3,i2;
	void *****p;

	if ((p=(void*****)__crtl_allocator.local_realloc(v, n5*sizeof(void****)))==NULL)
		return NULL;
	if ((p[0]=(void****)__crtl_allocator.local_realloc(v[0], n5*n4*sizeof(void***)))==NULL) {
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0]=(void***)__crtl_allocator.local_realloc(v[0][0], n5*n4*n3*sizeof(void**)))==NULL) {
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0]=(void**)__crtl_allocator.local_realloc(v[0][0][0], n5*n4*n3*n2*sizeof(void*)))==NULL) {
		__crtl_allocator.local_free(p[0][0]);
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0][0]=(void*)__crtl_allocator.local_realloc(v[0][0][0][0], n5*n4*n3*n2*n1*size))==NULL) {
		__crtl_allocator.local_free(p[0][0][0]);
		__crtl_allocator.local_free(p[0][0]);
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	for (i5=0; i5<n5; i5++) {
		p[i5] = p[0]+n4*i5;
		for (i4=0; i4<n4; i4++) {
			p[i5][i4] = p[0][0]+n3*(i4+n4*i5);
			for (i3=0; i3<n3; i3++) {
				p[i5][i4][i3] = p[0][0][0]+n2*(i3+n3*(i4+n4*i5));
				for (i2=0; i2<n2; i2++)
					p[i5][i4][i3][i2] = (char*)p[0][0][0][0]+
						size*n1*(i2+n2*(i3+n3*(i4+n4*i5)));
			}
		}
	}
	return p;
}


/* __crtl_allocator.local_free a 5-d array */
_api void crtl_mfree5 (void *****p)
{
	__crtl_allocator.local_free(p[0][0][0][0]);
	__crtl_allocator.local_free(p[0][0][0]);
	__crtl_allocator.local_free(p[0][0]);
	__crtl_allocator.local_free(p[0]);
	__crtl_allocator.local_free(p);
}

/* The following two functions were added by Zhaobo Meng, Jan. 1997*/
/* allocate a 6-d array */
_api void ******crtl_malloc6 (int n1, int n2, int n3, int n4, int n5, int n6, int size)
{
	int i6,i5,i4,i3,i2;
	void ******p;

	if ((p=(void******)__crtl_allocator.local_malloc(n6*sizeof(void*****)))==NULL)
		return NULL;

	if ((p[0]=(void*****)__crtl_allocator.local_malloc(n6*n5*sizeof(void****)))==NULL) {
                __crtl_allocator.local_free(p);
		return NULL;
        }

	if ((p[0][0]=(void****)__crtl_allocator.local_malloc(n6*n5*n4*sizeof(void***)))==NULL) {
		__crtl_allocator.local_free(p[0]);
                __crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0]=(void***)__crtl_allocator.local_malloc(n6*n5*n4*n3*sizeof(void**)))==NULL) {
		__crtl_allocator.local_free(p[0][0]);
                __crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0][0]=(void**)__crtl_allocator.local_malloc(n6*n5*n4*n3*n2*sizeof(void*)))==NULL) {
	        __crtl_allocator.local_free(p[0][0][0]);
		__crtl_allocator.local_free(p[0][0]);
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0][0][0]=(void*)__crtl_allocator.local_malloc(n6*n5*n4*n3*n2*n1*size))==NULL) {
	        __crtl_allocator.local_free(p[0][0][0][0]);
		__crtl_allocator.local_free(p[0][0][0]);
		__crtl_allocator.local_free(p[0][0]);
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}

        for (i6=0; i6<n6; i6++) {
                p[i6] = p[0]+n5*i6;
	        for (i5=0; i5<n5; i5++) {
	                p[i6][i5] = p[0][0]+n4*(i5+n5*i6);
			for (i4=0; i4<n4; i4++) {
			        p[i6][i5][i4] = p[0][0][0]+n3*(i4+n4*(i5+n5*i6));
				for (i3=0; i3<n3; i3++) {
				        p[i6][i5][i4][i3] = p[0][0][0][0]
					      +n2*(i3+n3*(i4+n4*(i5+n5*i6)));
					for (i2=0; i2<n2; i2++)
					        p[i6][i5][i4][i3][i2] = 
						      (char*)p[0][0][0][0][0]+
				       size*n1*(i2+n2*(i3+n3*(i4+n4*(i5+n5*i6))));
			        }
		        }
	        }
        }
	return p;
}

_api void ******crtl_remalloc6 (void ******v, int n1, int n2, int n3, int n4, int n5, int n6, int size)
{
	int i6,i5,i4,i3,i2;
	void ******p;

	if ((p=(void******)__crtl_allocator.local_realloc(v, n6*sizeof(void*****)))==NULL)
		return NULL;

	if ((p[0]=(void*****)__crtl_allocator.local_realloc(v[0], n6*n5*sizeof(void****)))==NULL) {
                __crtl_allocator.local_free(p);
		return NULL;
        }

	if ((p[0][0]=(void****)__crtl_allocator.local_realloc(v[0][0], n6*n5*n4*sizeof(void***)))==NULL) {
		__crtl_allocator.local_free(p[0]);
                __crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0]=(void***)__crtl_allocator.local_realloc(v[0][0][0], n6*n5*n4*n3*sizeof(void**)))==NULL) {
		__crtl_allocator.local_free(p[0][0]);
                __crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0][0]=(void**)__crtl_allocator.local_realloc(v[0][0][0][0], n6*n5*n4*n3*n2*sizeof(void*)))==NULL) {
	        __crtl_allocator.local_free(p[0][0][0]);
		__crtl_allocator.local_free(p[0][0]);
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}
	if ((p[0][0][0][0][0]=(void*)__crtl_allocator.local_realloc(v[0][0][0][0][0], n6*n5*n4*n3*n2*n1*size))==NULL) {
	        __crtl_allocator.local_free(p[0][0][0][0]);
		__crtl_allocator.local_free(p[0][0][0]);
		__crtl_allocator.local_free(p[0][0]);
		__crtl_allocator.local_free(p[0]);
		__crtl_allocator.local_free(p);
		return NULL;
	}

        for (i6=0; i6<n6; i6++) {
                p[i6] = p[0]+n5*i6;
	        for (i5=0; i5<n5; i5++) {
	                p[i6][i5] = p[0][0]+n4*(i5+n5*i6);
			for (i4=0; i4<n4; i4++) {
			        p[i6][i5][i4] = p[0][0][0]+n3*(i4+n4*(i5+n5*i6));
				for (i3=0; i3<n3; i3++) {
				        p[i6][i5][i4][i3] = p[0][0][0][0]
					      +n2*(i3+n3*(i4+n4*(i5+n5*i6)));
					for (i2=0; i2<n2; i2++)
					        p[i6][i5][i4][i3][i2] = 
						      (char*)p[0][0][0][0][0]+
				       size*n1*(i2+n2*(i3+n3*(i4+n4*(i5+n5*i6))));
			        }
		        }
	        }
        }
	return p;
}

/* __crtl_allocator.local_free a 6-d array */
_api void crtl_mfree6 (void ******p)
{
    __crtl_allocator.local_free(p[0][0][0][0][0]);
	__crtl_allocator.local_free(p[0][0][0][0]);
	__crtl_allocator.local_free(p[0][0][0]);
	__crtl_allocator.local_free(p[0][0]);
	__crtl_allocator.local_free(p[0]);
	__crtl_allocator.local_free(p);
}



