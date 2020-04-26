#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"

#include "crtl/bits/crtl_types_basic.h"
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




// SPDX-License-Identifier: GPL-2.0
/*
 * A fast, small, non-recursive O(n log n) sort for the Linux kernel
 *
 * This performs n*log2(n) + 0.37*n + o(n) comparisons on average,
 * and 1.5*n*log2(n) + O(n) in the (very contrived) worst case.
 *
 * Glibc qsort() manages n*log2(n) - 1.26*n for random inputs (1.63*n
 * better) at the expense of stack usage and much larger code to avoid
 * quicksort's O(n^2) worst case.
 */

#define pr_fmt(fmt) ": " fmt

/**
 * is_aligned - is this pointer & size okay for word-wide copying?
 * @base: pointer to data
 * @size: size of each element
 * @align: required alignment (typically 4 or 8)
 *
 * Returns true if elements can be copied using word loads and stores.
 * The size must be a multiple of the alignment, and the base address must
 * be if we do not have CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS.
 *
 * For some reason, gcc doesn't know to optimize "if (a & mask || b & mask)"
 * to "if ((a | b) & mask)", so we do that by hand.
 */
inline static bool is_aligned(const void *base, size_t size, unsigned char align)
{
	unsigned char lsbits = (unsigned char)size;

	(void)base;
    
	return (lsbits & (align - 1)) == 0;
}

/**
 * swap_words_32 - swap two elements in 32-bit chunks
 * @a: pointer to the first element to swap
 * @b: pointer to the second element to swap
 * @n: element size (must be a multiple of 4)
 *
 * Exchange the two objects in memory.  This exploits base+index addressing,
 * which basically all CPUs have, to minimize loop overhead computations.
 *
 * For some reason, on x86 gcc 7.3.0 adds a redundant test of n at the
 * bottom of the loop, even though the zero flag is stil valid from the
 * subtract (since the intervening mov instructions don't alter the flags).
 * Gcc 8.1.0 doesn't have that problem.
 */
static void __sort_swap_words_32(void *a, void *b, size_t n)
{
	do {
		uint32_t t = *(uint32_t *)(a + (n -= 4));
		*(uint32_t *)(a + n) = *(uint32_t *)(b + n);
		*(uint32_t *)(b + n) = t;
	} while (n);
}

/**
 * swap_words_64 - swap two elements in 64-bit chunks
 * @a: pointer to the first element to swap
 * @b: pointer to the second element to swap
 * @n: element size (must be a multiple of 8)
 *
 * Exchange the two objects in memory.  This exploits base+index
 * addressing, which basically all CPUs have, to minimize loop overhead
 * computations.
 *
 * We'd like to use 64-bit loads if possible.  If they're not, emulating
 * one requires base+index+4 addressing which x86 has but most other
 * processors do not.  If CONFIG_64BIT, we definitely have 64-bit loads,
 * but it's possible to have 64-bit loads without 64-bit pointers (e.g.
 * x32 ABI).  Are there any cases the kernel needs to worry about?
 */
static void __sort_swap_words_64(void *a, void *b, size_t n)
{
	do {
#ifdef CONFIG_64BIT
		uint64_t t = *(uint64_t *)(a + (n -= 8));
		*(uint64_t *)(a + n) = *(uint64_t *)(b + n);
		*(uint64_t *)(b + n) = t;
#else
		/* Use two 32-bit transfers to avoid base+index+4 addressing */
		uint32_t t = *(uint32_t *)(a + (n -= 4));
		*(uint32_t *)(a + n) = *(uint32_t *)(b + n);
		*(uint32_t *)(b + n) = t;

		t = *(uint32_t *)(a + (n -= 4));
		*(uint32_t *)(a + n) = *(uint32_t *)(b + n);
		*(uint32_t *)(b + n) = t;
#endif
	} while (n);
}

/**
 * swap_bytes - swap two elements a byte at a time
 * @a: pointer to the first element to swap
 * @b: pointer to the second element to swap
 * @n: element size
 *
 * This is the fallback if alignment doesn't allow using larger chunks.
 */
static void __sort_swap_bytes(void *a, void *b, size_t n)
{
	do {
		char t = ((char *)a)[--n];
		((char *)a)[n] = ((char *)b)[n];
		((char *)b)[n] = t;
	} while (n);
}

typedef void (*swap_func_t)(void *a, void *b, int size);

/*
 * The values are arbitrary as long as they can't be confused with
 * a pointer, but small integers make for the smallest compare
 * instructions.
 */
#define SWAP_WORDS_64 (swap_func_t)0
#define SWAP_WORDS_32 (swap_func_t)1
#define SWAP_BYTES    (swap_func_t)2

/*
 * The function pointer is last to make tail calls most efficient if the
 * compiler decides not to inline this function.
 */
static void __sort_do_swap(void *a, void *b, size_t size, swap_func_t swap_func)
{
	if (swap_func == SWAP_WORDS_64)
		__sort_swap_words_64(a, b, size);
	else if (swap_func == SWAP_WORDS_32)
		__sort_swap_words_32(a, b, size);
	else if (swap_func == SWAP_BYTES)
		__sort_swap_bytes(a, b, size);
	else
		swap_func(a, b, (int)size);
}

/**
 * parent - given the offset of the child, find the offset of the parent.
 * @i: the offset of the heap element whose parent is sought.  Non-zero.
 * @lsbit: a precomputed 1-bit mask, equal to "size & -size"
 * @size: size of each element
 *
 * In terms of array indexes, the parent of element j = @i/@size is simply
 * (j-1)/2.  But when working in byte offsets, we can't use implicit
 * truncation of integer divides.
 *
 * Fortunately, we only need one bit of the quotient, not the full divide.
 * @size has a least significant bit.  That bit will be clear if @i is
 * an even multiple of @size, and set if it's an odd multiple.
 *
 * Logically, we're doing "if (i & lsbit) i -= size;", but since the
 * branch is unpredictable, it's done with a bit of clever branch-free
 * code instead.
 */
inline static size_t __sort_parent(size_t i, unsigned int lsbit, size_t size)
{
	i -= size;
	i -= size & -(i & lsbit);
	return i / 2;
}

/**
 * sort - sort an array of elements
 * @base: pointer to data to sort
 * @num: number of elements
 * @size: size of each element
 * @cmp_func: pointer to comparison function
 * @swap_func: pointer to swap function or NULL
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
void crtl_sort(void *base, size_t num, size_t size, int (*cmp_func)(const void *, const void *), void (*swap_func)(void *, void *, int size))
{
	/* pre-scale counters for performance */
	size_t n = num * size, a = (num/2) * size;
	const unsigned int lsbit = size & -size;  /* Used to find parent */

	if (!a)		/* num < 2 || size == 0 */
		return;

	if (!swap_func) {
		if (is_aligned(base, size, 8))
			swap_func = SWAP_WORDS_64;
		else if (is_aligned(base, size, 4))
			swap_func = SWAP_WORDS_32;
		else
			swap_func = SWAP_BYTES;
	}

	/*
	 * Loop invariants:
	 * 1. elements [a,n) satisfy the heap property (compare greater than
	 *    all of their children),
	 * 2. elements [n,num*size) are sorted, and
	 * 3. a <= b <= c <= d <= n (whenever they are valid).
	 */
	for (;;) {
		size_t b, c, d;

		if (a)			/* Building heap: sift down --a */
			a -= size;
		else if (n -= size)	/* Sorting: Extract root to --n */
			__sort_do_swap(base, base + n, size, swap_func);
		else			/* Sort complete */
			break;

		/*
		 * Sift element at "a" down into heap.  This is the
		 * "bottom-up" variant, which significantly reduces
		 * calls to cmp_func(): we find the sift-down path all
		 * the way to the leaves (one compare per level), then
		 * backtrack to find where to insert the target element.
		 *
		 * Because elements tend to sift down close to the leaves,
		 * this uses fewer compares than doing two per level
		 * on the way down.  (A bit more than half as many on
		 * average, 3/4 worst-case.)
		 */
		for (b = a; c = 2*b + size, (d = c + size) < n;)
			b = cmp_func(base + c, base + d) >= 0 ? c : d;
		if (d == n)	/* Special case last leaf with no sibling */
			b = c;

		/* Now backtrack from "b" to the correct location for "a" */
		while (b != a && cmp_func(base + a, base + b) >= 0)
			b = __sort_parent(b, lsbit, size);
		c = b;			/* Where "a" belongs */
		while (b != a) {	/* Shift it into place */
			b = __sort_parent(b, lsbit, size);
			__sort_do_swap(base + b, base + c, size, swap_func);
		}
	}
}

