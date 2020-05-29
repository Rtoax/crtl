// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/mm/mempool.c
 *
 *  memory buffer pool support. Such pools are mostly used
 *  for guaranteed, deadlock-free memory allocations during
 *  extreme VM load.
 *
 *  started by Ingo Molnar, Copyright (C) 2001
 *  debugging by David Rientjes, Copyright (C) 2015
 */
#include <malloc.h>

#include "crtl/mempool.h"

#include <crypto/bug.h>
#include <crypto/asm/barrier.h>
#include <crypto/attribute.h>
#include <crypto/overflow.h>

#include "crypto/memory/mempool.h"


static inline void check_element(mempool_t *pool, void *element){}
static inline void poison_element(mempool_t *pool, void *element){}

static int mempool_init_node(mempool_t *pool, int min_nr, mempool_alloc_t alloc_fn,
		      mempool_free_t free_fn, void *pool_data,int node_id);
static mempool_t *mempool_create_node(int min_nr, mempool_alloc_t alloc_fn,
			mempool_free_t free_fn, void *pool_data,int nid);


static void add_element(mempool_t *pool, void *element)
{
	BUG_ON(pool->curr_nr >= pool->min_nr);
	poison_element(pool, element);
	pool->elements[pool->curr_nr++] = element;
}

static void *remove_element(mempool_t *pool)
{
	void *element = pool->elements[--pool->curr_nr];

	BUG_ON(pool->curr_nr < 0);
	check_element(pool, element);
	return element;
}

inline bool mempool_initialized(mempool_t *pool)
{
	return pool->elements != NULL;
}


/**
 * mempool_exit - exit a mempool initialized with mempool_init()
 * @pool:      pointer to the memory pool which was initialized with
 *             mempool_init().
 *
 * Free all reserved elements in @pool and @pool itself.  This function
 * only sleeps if the free_fn() function sleeps.
 *
 * May be called on a zeroed but uninitialized mempool (i.e. allocated with
 * kzalloc()).
 */
void mempool_exit(mempool_t *pool)
{
	while (pool->curr_nr) {
		void *element = remove_element(pool);
		pool->free(element, pool->pool_data);
	}
	free(pool->elements);
	pool->elements = NULL;
}

/**
 * mempool_destroy - deallocate a memory pool
 * @pool:      pointer to the memory pool which was allocated via
 *             mempool_create().
 *
 * Free all reserved elements in @pool and @pool itself.  This function
 * only sleeps if the free_fn() function sleeps.
 */
void mempool_destroy(mempool_t *pool)
{
	if (unlikely(!pool))
		return;

	mempool_exit(pool);
	free(pool);
}

static inline void *kmalloc_array_node(size_t n, size_t size, int node)
{
	size_t bytes;

	if (unlikely(check_mul_overflow(n, size, &bytes)))
		return NULL;
	if (__builtin_constant_p(n) && __builtin_constant_p(size))
		return malloc(bytes);
	return malloc(bytes);
}

int mempool_init_node(mempool_t *pool, int min_nr, mempool_alloc_t alloc_fn,
		      mempool_free_t free_fn, void *pool_data,int node_id)
{
	crtl_mutex_init(&pool->lock, NULL);
	pool->min_nr	= min_nr;
	pool->pool_data = pool_data;
	pool->alloc	= alloc_fn;
	pool->free	= free_fn;
	crtl_spin_init_shared(&pool->wait);

	pool->elements = kmalloc_array_node(min_nr, sizeof(void *), node_id);
	if (!pool->elements)
		return -ENOMEM;

	/*
	 * First pre-allocate the guaranteed number of buffers.
	 */
	while (pool->curr_nr < pool->min_nr) {
		void *element;
        
		element = pool->alloc(pool->pool_data);
		if (unlikely(!element)) {
			mempool_exit(pool);
			return -ENOMEM;
		}
		add_element(pool, element);
	}

	return 0;
}

/**
 * mempool_init - initialize a memory pool
 * @pool:      pointer to the memory pool that should be initialized
 * @min_nr:    the minimum number of elements guaranteed to be
 *             allocated for this pool.
 * @alloc_fn:  user-defined element-allocation function.
 * @free_fn:   user-defined element-freeing function.
 * @pool_data: optional private data available to the user-defined functions.
 *
 * Like mempool_create(), but initializes the pool in (i.e. embedded in another
 * structure).
 *
 * Return: %0 on success, negative error code otherwise.
 */
int mempool_init(mempool_t *pool, int min_nr, mempool_alloc_t alloc_fn,
		 mempool_free_t free_fn, void *pool_data)
{
	return mempool_init_node(pool, min_nr, alloc_fn, free_fn, pool_data, -1);
}

/**
 * mempool_create - create a memory pool
 * @min_nr:    the minimum number of elements guaranteed to be
 *             allocated for this pool.
 * @alloc_fn:  user-defined element-allocation function.
 * @free_fn:   user-defined element-freeing function.
 * @pool_data: optional private data available to the user-defined functions.
 *
 * this function creates and allocates a guaranteed size, preallocated
 * memory pool. The pool can be used from the mempool_alloc() and mempool_free()
 * functions. This function might sleep. Both the alloc_fn() and the free_fn()
 * functions might sleep - as long as the mempool_alloc() function is not called
 * from IRQ contexts.
 *
 * Return: pointer to the created memory pool object or %NULL on error.
 */
mempool_t *mempool_create(int min_nr, mempool_alloc_t alloc_fn,
				            mempool_free_t free_fn, void *pool_data)
{
	return mempool_create_node(min_nr,alloc_fn,free_fn, pool_data, -1);
}
                            
/**
 * kzalloc_node - allocate zeroed memory from a particular memory node.
 * @size: how many bytes of memory are required.
 * @flags: the type of memory to allocate (see kmalloc).
 * @node: memory node from which to allocate
 */
static inline void *kzalloc_node(size_t size, int node)
{
    return malloc(size);
}

mempool_t *mempool_create_node(int min_nr, mempool_alloc_t alloc_fn,
                			       mempool_free_t free_fn, void *pool_data,int node_id)
{
	mempool_t *pool;

	pool = kzalloc_node(sizeof(*pool), node_id);
	if (!pool)
		return NULL;
    
	if (mempool_init_node(pool, min_nr, alloc_fn, free_fn, pool_data,node_id)) {
		free(pool);
		return NULL;
	}

	return pool;
}
static inline void *kmalloc_array(unsigned n, size_t s)
{
   return malloc(n * s);
}

/**
 * mempool_resize - resize an existing memory pool
 * @pool:       pointer to the memory pool which was allocated via
 *              mempool_create().
 * @new_min_nr: the new minimum number of elements guaranteed to be
 *              allocated for this pool.
 *
 * This function shrinks/grows the pool. In the case of growing,
 * it cannot be guaranteed that the pool will be grown to the new
 * size immediately, but new mempool_free() calls will refill it.
 * This function may sleep.
 *
 * Note, the caller must guarantee that no mempool_destroy is called
 * while this function is running. mempool_alloc() & mempool_free()
 * might be called (eg. from IRQ contexts) while this function executes.
 *
 * Return: %0 on success, negative error code otherwise.
 */
int mempool_resize(mempool_t *pool, int new_min_nr)
{
	void *element;
	void **new_elements;

	BUG_ON(new_min_nr <= 0);

	crtl_mutex_lock(&pool->lock, 0,0,0,0);
	if (new_min_nr <= pool->min_nr) {
		while (new_min_nr < pool->curr_nr) {
			element = remove_element(pool);
			crtl_mutex_unlock(&pool->lock);
			pool->free(element, pool->pool_data);
			crtl_mutex_lock(&pool->lock, 0,0,0,0);
		}
		pool->min_nr = new_min_nr;
		goto out_unlock;
	}
	crtl_mutex_unlock(&pool->lock);

	/* Grow the pool */
	new_elements = kmalloc_array(new_min_nr, sizeof(*new_elements));
	if (!new_elements)
		return -ENOMEM;

	crtl_mutex_lock(&pool->lock, 0,0,0,0);
	if (unlikely(new_min_nr <= pool->min_nr)) {
		/* Raced, other resize will do our work */
		crtl_mutex_unlock(&pool->lock);
		free(new_elements);
		goto out;
	}
	memcpy(new_elements, pool->elements,
			pool->curr_nr * sizeof(*new_elements));
	free(pool->elements);
	pool->elements = new_elements;
	pool->min_nr = new_min_nr;

	while (pool->curr_nr < pool->min_nr) {
		crtl_mutex_unlock(&pool->lock);
		element = pool->alloc(pool->pool_data);
		if (!element)
			goto out;
		crtl_mutex_lock(&pool->lock, 0,0,0,0);
		if (pool->curr_nr < pool->min_nr) {
			add_element(pool, element);
		} else {
			crtl_mutex_unlock(&pool->lock);
			pool->free(element, pool->pool_data);	/* Raced */
			goto out;
		}
	}
out_unlock:
	crtl_mutex_unlock(&pool->lock);
out:
	return 0;
}

/**
 * mempool_alloc - allocate an element from a specific memory pool
 * @pool:      pointer to the memory pool which was allocated via
 *             mempool_create().
 * @gfp_mask:  the usual allocation bitmask.
 *
 * this function only sleeps if the alloc_fn() function sleeps or
 * returns NULL. Note that due to preallocation, this function
 * *never* fails when called from process contexts. (it might
 * fail if called from an IRQ context.)
 * Note: using __GFP_ZERO is not supported.
 *
 * Return: pointer to the allocated element or %NULL on error.
 */
void *mempool_alloc(mempool_t *pool)
{
	void *element;
	crtl_lock_spin_t wait;

repeat_alloc:

	element = pool->alloc(pool->pool_data);
	if (likely(element != NULL))
		return element;

	crtl_mutex_lock(&pool->lock, 0,0,0,0);
	if (likely(pool->curr_nr)) {
		element = remove_element(pool);
		crtl_mutex_unlock(&pool->lock);
		/* paired with rmb in mempool_free(), read comment there */
		smp_wmb();
		return element;
	}

	/* Let's wait for someone else to return an element to @pool */
	crtl_spin_init_shared(&wait);
	crtl_spin_lock(&pool->wait, 0);

	crtl_mutex_unlock(&pool->lock);


	crtl_spin_unlock(&pool->wait);
	goto repeat_alloc;
}

/**
 * mempool_free - return an element to the pool.
 * @element:   pool element pointer.
 * @pool:      pointer to the memory pool which was allocated via
 *             mempool_create().
 *
 * this function only sleeps if the free_fn() function sleeps.
 */
void mempool_free(void *element, mempool_t *pool)
{
	if (unlikely(element == NULL))
		return;

	/*
	 * Paired with the wmb in mempool_alloc().  The preceding read is
	 * for @element and the following @pool->curr_nr.  This ensures
	 * that the visible value of @pool->curr_nr is from after the
	 * allocation of @element.  This is necessary for fringe cases
	 * where @element was passed to this task without going through
	 * barriers.
	 *
	 * For example, assume @p is %NULL at the beginning and one task
	 * performs "p = mempool_alloc(...);" while another task is doing
	 * "while (!p) cpu_relax(); mempool_free(p, ...);".  This function
	 * may end up using curr_nr value which is from before allocation
	 * of @p without the following rmb.
	 */
	smp_rmb();

	/*
	 * For correctness, we need a test which is guaranteed to trigger
	 * if curr_nr + #allocated == min_nr.  Testing curr_nr < min_nr
	 * without locking achieves that and refilling as soon as possible
	 * is desirable.
	 *
	 * Because curr_nr visible here is always a value after the
	 * allocation of @element, any task which decremented curr_nr below
	 * min_nr is guaranteed to see curr_nr < min_nr unless curr_nr gets
	 * incremented to min_nr afterwards.  If curr_nr gets incremented
	 * to min_nr after the allocation of @element, the elements
	 * allocated after that are subject to the same guarantee.
	 *
	 * Waiters happen iff curr_nr is 0 and the above guarantee also
	 * ensures that there will be frees which return elements to the
	 * pool waking up the waiters.
	 */
	if (unlikely(pool->curr_nr < pool->min_nr)) {
		crtl_mutex_lock(&pool->lock, 0,0,0,0);
		if (likely(pool->curr_nr < pool->min_nr)) {
			add_element(pool, element);
			crtl_mutex_unlock(&pool->lock);
			crtl_spin_unlock(&pool->wait);
			return;
		}
		crtl_mutex_unlock(&pool->lock);
	}
	pool->free(element, pool->pool_data);
}


