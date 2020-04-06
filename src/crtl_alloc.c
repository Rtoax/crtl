#include <malloc.h>
#include "crtl/crtl_alloc.h"

/**
 *  分配内存 一维、二维、三维...
 *  
 */
/* allocate a 1-d array */
void *crtl_malloc1 (int n1, int size)
{
	void *p;

	if ((p=malloc(n1*size))==NULL)
		return NULL;
	return p;
}
void *crtl_remalloc1(void *v, int n1, int size)
{
	void *p;

	if ((p=realloc(v,n1*size))==NULL)
		return NULL;
	return p;
}

/* free a 1-d array */
void crtl_mfree1 (void *p)
{
	free(p);
}

/* allocate a 2-d array */
void **crtl_malloc2 (int n1, int n2, int size)
{
	int i2;
	void **p;

	if ((p=(void**)malloc(n2*sizeof(void*)))==NULL) 
		return NULL;
	if ((p[0]=(void*)malloc(n2*n1*size))==NULL) {
		free(p);
		return NULL;
	}
	for (i2=0; i2<n2; i2++)
		p[i2] = (void*)p[0]+size*n1*i2;
	return p;
}

/* free a 2-d array */
void crtl_mfree2 (void **p)
{
	free(p[0]);
	free(p);
}

void ***crtl_malloc3 (int n1, int n2, int n3, int size)
{
    int i3,i2;
    void ***p;

    if ((p=(void***)malloc(n3*sizeof(void**)))==NULL)
        return NULL;
    if ((p[0]=(void**)malloc(n3*n2*sizeof(void*)))==NULL) {
        free(p);
        return NULL;
    }
    if ((p[0][0]=(void*)malloc(n3*n2*n1*size))==NULL) {
        free(p[0]);
        free(p);
        return NULL;
    }

    for (i3=0; i3<n3; i3++) {
        p[i3] = p[0]+n2*i3;
        for (i2=0; i2<n2; i2++)
            p[i3][i2] = (void*)p[0][0]+size*n1*(i2+n2*i3);
    }
    return p;
}
void crtl_mfree3 (void ***p)
{
	free(p[0][0]);
	free(p[0]);
	free(p);
}
/* allocate a 4-d array */
void ****crtl_malloc4 (int n1, int n2, int n3, int n4, int size)
{
	int i4,i3,i2;
	void ****p;

	if ((p=(void****)malloc(n4*sizeof(void***)))==NULL)
		return NULL;
	if ((p[0]=(void***)malloc(n4*n3*sizeof(void**)))==NULL) {
		free(p);
		return NULL;
	}
	if ((p[0][0]=(void**)malloc(n4*n3*n2*sizeof(void*)))==NULL) {
		free(p[0]);
		free(p);
		return NULL;
	}
	if ((p[0][0][0]=(void*)malloc(n4*n3*n2*n1*size))==NULL) {
		free(p[0][0]);
		free(p[0]);
		free(p);
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
/* free a 4-d array */
void crtl_mfree4 (void ****p)
{
	free(p[0][0][0]);
	free(p[0][0]);
	free(p[0]);
	free(p);
}

/* The following two functions were added by Zhaobo Meng, Jan. 1997*/
/* allocate a 5-d array */
void *****crtl_malloc5 (int n1, int n2, int n3, int n4, int n5, int size)
{
	int i5,i4,i3,i2;
	void *****p;

	if ((p=(void*****)malloc(n5*sizeof(void****)))==NULL)
		return NULL;
	if ((p[0]=(void****)malloc(n5*n4*sizeof(void***)))==NULL) {
		free(p);
		return NULL;
	}
	if ((p[0][0]=(void***)malloc(n5*n4*n3*sizeof(void**)))==NULL) {
		free(p[0]);
		free(p);
		return NULL;
	}
	if ((p[0][0][0]=(void**)malloc(n5*n4*n3*n2*sizeof(void*)))==NULL) {
		free(p[0][0]);
		free(p[0]);
		free(p);
		return NULL;
	}
	if ((p[0][0][0][0]=(void*)malloc(n5*n4*n3*n2*n1*size))==NULL) {
		free(p[0][0][0]);
		free(p[0][0]);
		free(p[0]);
		free(p);
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

/* free a 5-d array */
void crtl_mfree5 (void *****p)
{
	free(p[0][0][0][0]);
	free(p[0][0][0]);
	free(p[0][0]);
	free(p[0]);
	free(p);
}

/* The following two functions were added by Zhaobo Meng, Jan. 1997*/
/* allocate a 6-d array */
void ******crtl_malloc6 (int n1, int n2, int n3, int n4, int n5, int n6, int size)
{
	int i6,i5,i4,i3,i2;
	void ******p;

	if ((p=(void******)malloc(n6*sizeof(void*****)))==NULL)
		return NULL;

	if ((p[0]=(void*****)malloc(n6*n5*sizeof(void****)))==NULL) {
                free(p);
		return NULL;
        }

	if ((p[0][0]=(void****)malloc(n6*n5*n4*sizeof(void***)))==NULL) {
		free(p[0]);
                free(p);
		return NULL;
	}
	if ((p[0][0][0]=(void***)malloc(n6*n5*n4*n3*sizeof(void**)))==NULL) {
		free(p[0][0]);
                free(p[0]);
		free(p);
		return NULL;
	}
	if ((p[0][0][0][0]=(void**)malloc(n6*n5*n4*n3*n2*sizeof(void*)))==NULL) {
	        free(p[0][0][0]);
		free(p[0][0]);
		free(p[0]);
		free(p);
		return NULL;
	}
	if ((p[0][0][0][0][0]=(void*)malloc(n6*n5*n4*n3*n2*n1*size))==NULL) {
	        free(p[0][0][0][0]);
		free(p[0][0][0]);
		free(p[0][0]);
		free(p[0]);
		free(p);
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

/* free a 6-d array */
void crtl_mfree6 (void ******p)
{
    free(p[0][0][0][0][0]);
	free(p[0][0][0][0]);
	free(p[0][0][0]);
	free(p[0][0]);
	free(p[0]);
	free(p);
}



