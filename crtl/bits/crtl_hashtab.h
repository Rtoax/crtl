#ifndef __CRTL_BITS_HASHTAB_H
#define __CRTL_BITS_HASHTAB_H 1

#include <stdint.h>
#include <string.h>
#include <malloc.h>


#include "crtl/bits/types_basic.h"

#include "crtl/easy/attribute.h"

/**
 * File: crtl_hashtab_struct.h   security\selinux\ss 2509    2011/3/15   
 *
 * A hash table (crtl_hashtab_struct) maintains associations between
 * key values and datum values.  The type of the key values
 * and the type of the datum values is arbitrary.  The
 * functions for hash computation and key comparison are
 * provided by the creator of the table.
 *
 * Author : Stephen Smalley, <sds@epoch.ncsc.mil>
 */
#ifndef _SS_HASHTAB_H_
#define _SS_HASHTAB_H_

#define crtl_hashtab_MAX_NODES	0xffffffff

struct crtl_hashtab_node {
	void *key;
	void *datum;
	struct crtl_hashtab_node *next;
};

struct crtl_hashtab_struct {
	struct crtl_hashtab_node **htable;	/* hash table */
	uint32_t size;			/* number of slots in hash table */
	uint32_t nel;			/* number of elements in hash table */
	uint32_t (*hash_value)(struct crtl_hashtab_struct *h, const void *key);
					/* hash function */
	int (*keycmp)(struct crtl_hashtab_struct *h, const void *key1, const void *key2);
					/* key comparison function */
};

struct crtl_hashtab_info {
	uint32_t slots_used;
	uint32_t max_chain_len;
};

/*
 * Creates a new hash table with the specified characteristics.
 *
 * Returns NULL if insufficent space is available or
 * the new hash table otherwise.
 */
_api struct crtl_hashtab_struct *crtl_hashtab_create(uint32_t (*hash_value)(struct crtl_hashtab_struct *h, const void *key),
			       int (*keycmp)(struct crtl_hashtab_struct *h, const void *key1, const void *key2),
			       uint32_t size);

/*
 * Inserts the specified (key, datum) pair into the specified hash table.
 *
 * Returns -ENOMEM on memory allocation error,
 * -EEXIST if there is already an entry with the same key,
 * -EINVAL for general errors or
  0 otherwise.
 */
_api int crtl_hashtab_insert(struct crtl_hashtab_struct *h, void *k, void *d);

/*
 * Searches for the entry with the specified key in the hash table.
 *
 * Returns NULL if no entry has the specified key or
 * the datum of the entry otherwise.
 */
_api void *crtl_hashtab_search(struct crtl_hashtab_struct *h, const void *k);

/*
 * Destroys the specified hash table.
 */
_api void crtl_hashtab_destroy(struct crtl_hashtab_struct *h);

/*
 * Applies the specified apply function to (key,datum,args)
 * for each entry in the specified hash table.
 *
 * The order in which the function is applied to the entries
 * is dependent upon the internal structure of the hash table.
 *
 * If apply returns a non-zero status, then crtl_hashtab_map will cease
 * iterating through the hash table and will propagate the error
 * return to its caller.
 */
_api int crtl_hashtab_map(struct crtl_hashtab_struct *h, int (*apply)(void *k, void *d, void *args), void *args);

/* Fill info with some hash table statistics */
_api void crtl_hashtab_stat(struct crtl_hashtab_struct *h, struct crtl_hashtab_info *info);

#endif	/* _SS_HASHTAB_H_ */


_hidden uint32_t __hash_value(struct crtl_hashtab_struct *h, const void *key);
_hidden int __keycmp(struct crtl_hashtab_struct *h, const void *key1, const void *key2);




#endif /*<__CRTL_BITS_HASHTAB_H>*/

