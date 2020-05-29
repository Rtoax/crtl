/* SPDX-License-Identifier: GPL-2.0 */
/*
 * memory buffer pool support
 */
#ifndef _LINUX_MEMPOOL_H
#define _LINUX_MEMPOOL_H

#include <stdbool.h>
#include <sys/types.h>

#include <crtl/bits/mutex.h>
#include <crtl/bits/spinlock.h>


typedef void * (*mempool_alloc_t)(void *argv);
typedef void (*mempool_free_t)(void *element, void *argv);

typedef struct mempool_s {
	crtl_lock_mutex_t lock;
	int min_nr;
	int curr_nr;
	void **elements;

	void *pool_data;
	mempool_alloc_t alloc;
	mempool_free_t free;
    
	crtl_lock_spin_t wait;
} mempool_t;

inline bool mempool_initialized(mempool_t *pool);

void mempool_exit(mempool_t *pool);

int mempool_init(mempool_t *pool, int min_nr, mempool_alloc_t alloc_fn,
		 mempool_free_t free_fn, void *argv);


mempool_t *mempool_create(int min_nr, mempool_alloc_t alloc_fn,
			mempool_free_t free_fn, void *pool_data);


int mempool_resize(mempool_t *pool, int new_min_nr);
void mempool_destroy(mempool_t *pool);
void *mempool_alloc(mempool_t *pool) ;
void mempool_free(void *element, mempool_t *pool);


#endif /* _LINUX_MEMPOOL_H */

