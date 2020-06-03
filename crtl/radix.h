#include <stdint.h>
#ifndef ART_H
#define ART_H

#ifdef __cplusplus
extern "C" {
#endif

#define NODE4   1
#define NODE16  2
#define NODE48  3
#define NODE256 4

#define MAX_PREFIX_LEN 10

#if defined(__GNUC__) && !defined(__clang__)
# if __STDC_VERSION__ >= 199901L && 402 == (__GNUC__ * 100 + __GNUC_MINOR__)
/*
 * GCC 4.2.2's C99 inline keyword support is pretty broken; avoid. Introduced in
 * GCC 4.2.something, fixed in 4.3.0. So checking for specific major.minor of
 * 4.2 is fine.
 */
#  define BROKEN_GCC_C99_INLINE
# endif
#endif

typedef int(*crtl_radix_callback)(void *data, const unsigned char *key, uint32_t key_len, void *value);

/**
 * This struct is included as part
 * of all the various node sizes
 */
typedef struct {
    uint8_t type;
    uint8_t num_children;
    uint32_t partial_len;
    unsigned char partial[MAX_PREFIX_LEN];
} crtl_radix_node;

/**
 * Small node with only 4 children
 */
typedef struct {
    crtl_radix_node n;
    unsigned char keys[4];
    crtl_radix_node *children[4];
} crtl_radix_node4;

/**
 * Node with 16 children
 */
typedef struct {
    crtl_radix_node n;
    unsigned char keys[16];
    crtl_radix_node *children[16];
} crtl_radix_node16;

/**
 * Node with 48 children, but
 * a full 256 byte field.
 */
typedef struct {
    crtl_radix_node n;
    unsigned char keys[256];
    crtl_radix_node *children[48];
} crtl_radix_node48;

/**
 * Full node with 256 children
 */
typedef struct {
    crtl_radix_node n;
    crtl_radix_node *children[256];
} crtl_radix_node256;

/**
 * Represents a leaf. These are
 * of arbitrary size, as they include the key.
 */
typedef struct {
    void *value;
    uint32_t key_len;
    unsigned char key[];
} crtl_radix_leaf;

/**
 * Main struct, points to root.
 */
typedef struct {
    crtl_radix_node *root;
    uint64_t size;
} crtl_radix_tree;

/**
 * Initializes an ART tree
 * @return 0 on success.
 */
int crtl_radix_tree_init(crtl_radix_tree *t);

/**
 * DEPRECATED
 * Initializes an ART tree
 * @return 0 on success.
 */
#define init_crtl_radix_tree(...) crtl_radix_tree_init(__VA_ARGS__)

/**
 * Destroys an ART tree
 * @return 0 on success.
 */
int crtl_radix_tree_destroy(crtl_radix_tree *t);

/**
 * DEPRECATED
 * Initializes an ART tree
 * @return 0 on success.
 */
#define destroy_crtl_radix_tree(...) crtl_radix_tree_destroy(__VA_ARGS__)

/**
 * Returns the size of the ART tree.
 */
#ifdef BROKEN_GCC_C99_INLINE
# define crtl_radix_size(t) ((t)->size)
#else
inline uint64_t crtl_radix_size(crtl_radix_tree *t) {
    return t->size;
}
#endif

/**
 * Inserts a new value into the ART tree
 * @arg t The tree
 * @arg key The key
 * @arg key_len The length of the key
 * @arg value Opaque value.
 * @return NULL if the item was newly inserted, otherwise
 * the old value pointer is returned.
 */
void* crtl_radix_insert(crtl_radix_tree *t, const unsigned char *key, int key_len, void *value);

/**
 * Deletes a value from the ART tree
 * @arg t The tree
 * @arg key The key
 * @arg key_len The length of the key
 * @return NULL if the item was not found, otherwise
 * the value pointer is returned.
 */
void* crtl_radix_delete(crtl_radix_tree *t, const unsigned char *key, int key_len);

/**
 * Searches for a value in the ART tree
 * @arg t The tree
 * @arg key The key
 * @arg key_len The length of the key
 * @return NULL if the item was not found, otherwise
 * the value pointer is returned.
 */
void* crtl_radix_search(const crtl_radix_tree *t, const unsigned char *key, int key_len);

/**
 * Returns the minimum valued leaf
 * @return The minimum leaf or NULL
 */
crtl_radix_leaf* crtl_radix_minimum(crtl_radix_tree *t);

/**
 * Returns the maximum valued leaf
 * @return The maximum leaf or NULL
 */
crtl_radix_leaf* crtl_radix_maximum(crtl_radix_tree *t);

/**
 * Iterates through the entries pairs in the map,
 * invoking a callback for each. The call back gets a
 * key, value for each and returns an integer stop value.
 * If the callback returns non-zero, then the iteration stops.
 * @arg t The tree to iterate over
 * @arg cb The callback function to invoke
 * @arg data Opaque handle passed to the callback
 * @return 0 on success, or the return of the callback.
 */
int crtl_radix_iter(crtl_radix_tree *t, crtl_radix_callback cb, void *data);

/**
 * Iterates through the entries pairs in the map,
 * invoking a callback for each that matches a given prefix.
 * The call back gets a key, value for each and returns an integer stop value.
 * If the callback returns non-zero, then the iteration stops.
 * @arg t The tree to iterate over
 * @arg prefix The prefix of keys to read
 * @arg prefix_len The length of the prefix
 * @arg cb The callback function to invoke
 * @arg data Opaque handle passed to the callback
 * @return 0 on success, or the return of the callback.
 */
int crtl_radix_iter_prefix(crtl_radix_tree *t, const unsigned char *prefix, int prefix_len, crtl_radix_callback cb, void *data);

#ifdef __cplusplus
}
#endif

#endif

