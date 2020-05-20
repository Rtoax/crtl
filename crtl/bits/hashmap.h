#ifndef __CRTL_BITS_CRTL_HASHMAP_H
#define __CRTL_BITS_CRTL_HASHMAP_H 1



/* SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause) */

/*
 * Generic non-thread safe hash map implementation.
 *
 * Copyright (c) 2019 Facebook
 */
#ifndef __LIBBPF_CRTL_HASHMAP_H
#define __LIBBPF_CRTL_HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <bits/wordsize.h>

static inline size_t crtl_hash_bits(size_t h, int bits)
{
	/* shuffle bits and return requested number of upper bits */
	return (h * 11400714819323198485llu) >> (__WORDSIZE - bits);
}

typedef size_t (*crtl_hashmap_hash_fn)(const void *key, void *ctx);
typedef bool (*crtl_hashmap_equal_fn)(const void *key1, const void *key2, void *ctx);

struct crtl_hashmap_entry {
	const void *key;
	void *value;
	struct crtl_hashmap_entry *next;
};

struct crtl_hashmap_struct {
	crtl_hashmap_hash_fn hash_fn;
	crtl_hashmap_equal_fn equal_fn;
	void *ctx;

	struct crtl_hashmap_entry **buckets;
	size_t cap;
	size_t cap_bits;
	size_t sz;
};

#define CRTL_HASHMAP_INIT(hash_fn, equal_fn, ctx) {	\
	.hash_fn = (hash_fn),			\
	.equal_fn = (equal_fn),			\
	.ctx = (ctx),				\
	.buckets = NULL,			\
	.cap = 0,				\
	.cap_bits = 0,				\
	.sz = 0,				\
}

void crtl_hashmap_init(struct crtl_hashmap_struct *map, crtl_hashmap_hash_fn hash_fn,
		   crtl_hashmap_equal_fn equal_fn, void *ctx);
struct crtl_hashmap_struct *crtl_hashmap_new(crtl_hashmap_hash_fn hash_fn,
			     crtl_hashmap_equal_fn equal_fn,
			     void *ctx);
void crtl_hashmap_clear(struct crtl_hashmap_struct *map);
void crtl_hashmap_free(struct crtl_hashmap_struct *map);

size_t crtl_hashmap_size(const struct crtl_hashmap_struct *map);
size_t crtl_hashmap_capacity(const struct crtl_hashmap_struct *map);

/*
 * crtl_hashmap_struct insertion strategy:
 * - CRTL_HASHMAP_ADD - only add key/value if key doesn't exist yet;
 * - CRTL_HASHMAP_SET - add key/value pair if key doesn't exist yet; otherwise,
 *   update value;
 * - CRTL_HASHMAP_UPDATE - update value, if key already exists; otherwise, do
 *   nothing and return -ENOENT;
 * - CRTL_HASHMAP_APPEND - always add key/value pair, even if key already exists.
 *   This turns crtl_hashmap_struct into a multimap by allowing multiple values to be
 *   associated with the same key. Most useful read API for such crtl_hashmap_struct is
 *   crtl_hashmap_for_each_key_entry() iteration. If crtl_hashmap_find() is still
 *   used, it will return last inserted key/value entry (first in a bucket
 *   chain).
 */
enum crtl_hashmap_insert_strategy {
	CRTL_HASHMAP_ADD,
	CRTL_HASHMAP_SET,
	CRTL_HASHMAP_UPDATE,
	CRTL_HASHMAP_APPEND,
};

/*
 * crtl_hashmap_insert() adds key/value entry w/ various semantics, depending on
 * provided strategy value. If a given key/value pair replaced already
 * existing key/value pair, both old key and old value will be returned
 * through old_key and old_value to allow calling code do proper memory
 * management.
 */
int crtl_hashmap_insert(struct crtl_hashmap_struct *map, const void *key, void *value,
		    enum crtl_hashmap_insert_strategy strategy,
		    const void **old_key, void **old_value);

static inline int crtl_hashmap_add(struct crtl_hashmap_struct *map,
			       const void *key, void *value)
{
	return crtl_hashmap_insert(map, key, value, CRTL_HASHMAP_ADD, NULL, NULL);
}

static inline int crtl_hashmap_set(struct crtl_hashmap_struct *map,
			       const void *key, void *value,
			       const void **old_key, void **old_value)
{
	return crtl_hashmap_insert(map, key, value, CRTL_HASHMAP_SET,
			       old_key, old_value);
}

static inline int crtl_hashmap_update(struct crtl_hashmap_struct *map,
				  const void *key, void *value,
				  const void **old_key, void **old_value)
{
	return crtl_hashmap_insert(map, key, value, CRTL_HASHMAP_UPDATE,
			       old_key, old_value);
}

static inline int crtl_hashmap_append(struct crtl_hashmap_struct *map,
				  const void *key, void *value)
{
	return crtl_hashmap_insert(map, key, value, CRTL_HASHMAP_APPEND, NULL, NULL);
}

bool crtl_hashmap_delete(struct crtl_hashmap_struct *map, const void *key,
		     const void **old_key, void **old_value);

bool crtl_hashmap_find(const struct crtl_hashmap_struct *map, const void *key, void **value);

/*
 * crtl_hashmap_for_each_entry - iterate over all entries in crtl_hashmap_struct
 * @map: crtl_hashmap_struct to iterate
 * @cur: struct crtl_hashmap_entry * used as a loop cursor
 * @bkt: integer used as a bucket loop cursor
 */
#define crtl_hashmap_for_each_entry(map, cur, bkt)				    \
	for (bkt = 0; bkt < map->cap; bkt++)				    \
		for (cur = map->buckets[bkt]; cur; cur = cur->next)

/*
 * crtl_hashmap_for_each_entry_safe - iterate over all entries in crtl_hashmap_struct, safe
 * against removals
 * @map: crtl_hashmap_struct to iterate
 * @cur: struct crtl_hashmap_entry * used as a loop cursor
 * @tmp: struct crtl_hashmap_entry * used as a temporary next cursor storage
 * @bkt: integer used as a bucket loop cursor
 */
#define crtl_hashmap_for_each_entry_safe(map, cur, tmp, bkt)		    \
	for (bkt = 0; bkt < map->cap; bkt++)				    \
		for (cur = map->buckets[bkt];				    \
		     cur && ({tmp = cur->next; true; });		    \
		     cur = tmp)

/*
 * crtl_hashmap_for_each_key_entry - iterate over entries associated with given key
 * @map: crtl_hashmap_struct to iterate
 * @cur: struct crtl_hashmap_entry * used as a loop cursor
 * @key: key to iterate entries for
 */
#define crtl_hashmap_for_each_key_entry(map, cur, _key)			    \
	for (cur = ({ size_t bkt = crtl_hash_bits(map->hash_fn((_key), map->ctx),\
					     map->cap_bits);		    \
		     map->buckets ? map->buckets[bkt] : NULL; });	    \
	     cur;							    \
	     cur = cur->next)						    \
		if (map->equal_fn(cur->key, (_key), map->ctx))

#define crtl_hashmap_for_each_key_entry_safe(map, cur, tmp, _key)		    \
	for (cur = ({ size_t bkt = crtl_hash_bits(map->hash_fn((_key), map->ctx),\
					     map->cap_bits);		    \
		     cur = map->buckets ? map->buckets[bkt] : NULL; });	    \
	     cur && ({ tmp = cur->next; true; });			    \
	     cur = tmp)							    \
		if (map->equal_fn(cur->key, (_key), map->ctx))

#endif /* __LIBBPF_CRTL_HASHMAP_H */

#endif /*<__CRTL_BITS_CRTL_HASHMAP_H>*/

