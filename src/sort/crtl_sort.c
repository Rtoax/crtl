#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"
#include "crtl/crtl_types.h"
#include "crtl/crtl_alloc.h"
#include "crtl/crtl_string.h"
#include "crtl/easy/macro.h"



void crtl_sort_cocktailshaker(void *base, size_t num, size_t size, int (*cmp)(const void *, const void *))
{
    crtl_byte *pbBase = (crtl_byte *)base;
    size_t i;
    int swapped;

    if(num) {
        do {
            swapped = false;
            for(i = 0; i < num - 2; i++)
                if(cmp(pbBase + (i + 1) * size, pbBase + i * size) > 0) {
                    crtl_memswap(pbBase + (i + 1) * size, pbBase + i * size, size);
                    swapped = true;
                }

            if(!swapped) break;

            swapped = false;
            for(i = num - 2; i > 0; i--)
                if(cmp(pbBase + (i + 1) * size, pbBase + i * size) > 0) {
                    crtl_memswap(pbBase + (i + 1) * size, pbBase + i * size, size);
                    swapped = true;
                }
        } while(swapped);
    }
}


void crtl_sort_heap(void *base, size_t num,size_t size, int (*cmp)(const void *, const void *))
{
    crtl_byte *pbBase = (crtl_byte *)base;
    int i = (num / 2 - 1) * size;
    int n = num * size;
    int c, r;

    while(i >= 0) {
        for(r = i; r * 2 + size < n; r = c) {
            c = r * 2 + size;
            if(c < n - size && cmp(pbBase + c, pbBase + c + size) >= 0) c += size;
            if(cmp(pbBase + r, pbBase + c) < 0) break;
            crtl_memswap(pbBase + r, pbBase + c, size);
        }
        i -= size;
    }

    for(i = n - size; i > 0; i -= size) {
        crtl_memswap(pbBase, pbBase + i, size);

        for(r = 0; r * 2 + size < i; r = c) {
            c = r * 2 + size;
            if(c < i - size && cmp(pbBase + c, pbBase + c + size) >= 0) c += size;
            if(cmp(pbBase + r, pbBase + c) < 0) break;
            crtl_memswap(pbBase + r, pbBase + c, size);
        }
    }
}



void crtl_sort_insertion(void *base, size_t num, size_t size, int (*cmp)(const void *, const void *))
{
    crtl_byte *pbBase = (crtl_byte *)base;
    size_t i, j;

    if(num)
        for(i = 1; i < num; i++)
            for(j = i; j > 0 && cmp(pbBase + j * size, pbBase + (j - 1) * size) > 0; j--)
                crtl_memswap(pbBase + j * size, pbBase + (j - 1) * size, size);
}


void crtl_sort_qsort3way(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *))
{
    crtl_byte *ptr = (crtl_byte *)base;

    while(n > 1) {
        int i = 1, lt = 0, gt = n;
        while(i < gt) {
            int c = cmp(ptr + lt * size, ptr + i * size);
            if(c < 0) {
                crtl_memswap(ptr + lt * size, ptr + i * size, size);
                lt++;
                i++;
            } else if(c > 0) {
                gt--;
                crtl_memswap(ptr + i * size, ptr + gt * size, size);
            } else {
                i++;
            }
        }

        if(lt < n - gt) {
            crtl_sort_qsort3way(ptr, lt, size, cmp);
            ptr += gt * size;
            n -= gt;
        } else {
            crtl_sort_qsort3way(ptr + gt * size, n - gt, size, cmp);
            n = lt;
        }
    }
}



static size_t __partition(void *base, size_t lo,size_t hi,size_t size,int (*cmp)(const void *, const void *))
{
    crtl_byte *pbBase = (crtl_byte *)base;
    crtl_byte *v = (crtl_byte *)base;

    size_t i = lo;
    size_t j = hi + 1;

    //log_debug("lo = %ld, hi = %ld\n", lo, hi);

    while(true) 
    {
        while(cmp(pbBase + (++i) * size, v + lo * size) > 0)
            if(i == hi) break;
        while(cmp(v + lo * size, pbBase + (--j) * size) > 0)
            if(j == lo) break;
        if(i >= j) break;
        
        crtl_memswap(pbBase + i * size, pbBase + j * size, size);
    }
    crtl_memswap(pbBase + lo * size, pbBase + j * size, size);
    //log_debug("\n");
    
    return j;
}

void crtl_sort_qsortH(void *base,size_t lo,size_t hi,size_t size,int (*cmp)(const void *, const void *))
{
    size_t p;

    if((long)hi <= (long)lo)
    {
        return;
    }
    else 
    {
        p = __partition(base, lo, hi, size, cmp);
        crtl_sort_qsortH(base, lo, p - 1, size, cmp);
        crtl_sort_qsortH(base, (p + 1), hi, size, cmp);
    }
}



static size_t __LomutoPartition(void *base,size_t lo,size_t hi,size_t size,int (*cmp)(const void *, const void *))
{
  crtl_byte *ptr = (crtl_byte *)base;
  crtl_byte *p = ptr + hi * size;

  int i = lo - 1;
  size_t j;

  for(j = lo; j < hi; j++)
    if(cmp(ptr + j * size, p) > 0)
      crtl_memswap(ptr + (++i) * size, ptr + j * size, size);

  crtl_memswap(ptr + (i + 1) * size, ptr + j * size, size);

  return i + 1;
}

void crtl_sort_qsortL(void *base,size_t lo,size_t hi,size_t size,int (*cmp)(const void *, const void *))
{
    size_t p;

    if((long)hi <= (long)lo)
        return;
    else {
        p = __LomutoPartition(base, lo, hi, size, cmp);
        crtl_sort_qsortL(base, lo, p - 1, size, cmp);
        crtl_sort_qsortL(base, (p + 1), hi, size, cmp);
    }
}


void crtl_sort_selection(void *base,size_t num,size_t size,int (*cmp)(const void *, const void *))
{
    crtl_byte *pvBase = (crtl_byte *)base;
    size_t i, j;
    size_t m;

    if(num) {
        for(i = 0; i < num; i++) {
            m = i;

            for(j = i + 1; j < num; j++)
                if(cmp(pvBase + j * size, pvBase + m * size) > 0) 
                    m = j;

            crtl_memswap(pvBase + i * size, pvBase + m * size, size);
        }
    }
}


void crtl_sort_shell(void *base,size_t num,size_t size,int (*cmp)(const void *, const void *))
{
    crtl_byte *pbBase = (crtl_byte *)base;
    size_t i, j;
    size_t h = 1;

    if(num) {
        while(h < num / 3)
            h = 3 * h + 1;

        while(h >= 1) {
            for(i = 1; i < num; i++)
                for(j = i; j > 0 && cmp(pbBase + j * size, pbBase + (j - 1) * size) > 0; j--)
                    crtl_memswap(pbBase + j * size, pbBase + (j - 1) * size, size);
            h /= 3;
        }
    }
}

