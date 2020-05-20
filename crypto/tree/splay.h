#ifndef __CRTL_BITS_TREE_SPLAY_H
#define __CRTL_BITS_TREE_SPLAY_H 1

#include "crtl/easy/attribute.h"

/*-
 * copyright 2002 niels provos <provos@citi.umich.edu>
 * all rights reserved.
 *
 * redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * this software is provided by the author ``as is'' and any express or
 * implied warranties, including, but not limited to, the implied warranties
 * of merchantability and fitness for a particular purpose are disclaimed.
 * in no event shall the author be liable for any direct, indirect,
 * incidental, special, exemplary, or consequential damages (including, but
 * not limited to, procurement of substitute goods or services; loss of use,
 * data, or profits; or business interruption) however caused and on any
 * theory of liability, whether in contract, strict liability, or tort
 * (including negligence or otherwise) arising in any way out of the use of
 * this software, even if advised of the possibility of such damage.
 */

/*
 * this file defines data structures for different types of trees:
 * splay trees and red-black trees.
 *
 * a splay tree is a self-organizing data structure.  every operation
 * on the tree causes a splay to happen.  the splay moves the requested
 * node to the root of the tree and partly rebalances it.
 *
 * this has the benefit that request locality causes faster lookups as
 * the requested nodes move to the top of the tree.  on the other hand,
 * every lookup causes memory writes.
 *
 * the balance theorem bounds the total access time for m operations
 * and n inserts on an initially empty tree as o((m + n)lg n).  the
 * amortized cost for a sequence of m accesses to a splay tree is o(lg n);
 */

#define crtl_splay_head(name, type)                                                \
struct name {                                                                 \
  struct type *sph_root; /* root of the tree */                               \
}

#define crtl_splay_initializer(root)                                               \
  { NULL }

#define crtl_splay_init(root) do {                                                 \
  (root)->sph_root = NULL;                                                    \
} while (/*constcond*/ 0)

#define crtl_splay_entry(type)                                                     \
struct {                                                                      \
  struct type *spe_left;          /* left element */                          \
  struct type *spe_right;         /* right element */                         \
}

#define crtl_splay_left(elm, field)    (elm)->field.spe_left
#define crtl_splay_right(elm, field)   (elm)->field.spe_right
#define crtl_splay_root(head)          (head)->sph_root
#define crtl_splay_empty(head)         (crtl_splay_root(head) == NULL)

/* crtl_splay_rotate_{left,right} expect that tmp hold crtl_splay_{right,left} */
#define crtl_splay_rotate_right(head, tmp, field) do {                             \
  crtl_splay_left((head)->sph_root, field) = crtl_splay_right(tmp, field);              \
  crtl_splay_right(tmp, field) = (head)->sph_root;                                 \
  (head)->sph_root = tmp;                                                     \
} while (/*constcond*/ 0)

#define crtl_splay_rotate_left(head, tmp, field) do {                              \
  crtl_splay_right((head)->sph_root, field) = crtl_splay_left(tmp, field);              \
  crtl_splay_left(tmp, field) = (head)->sph_root;                                  \
  (head)->sph_root = tmp;                                                     \
} while (/*constcond*/ 0)

#define crtl_splay_linkleft(head, tmp, field) do {                                 \
  crtl_splay_left(tmp, field) = (head)->sph_root;                                  \
  tmp = (head)->sph_root;                                                     \
  (head)->sph_root = crtl_splay_left((head)->sph_root, field);                     \
} while (/*constcond*/ 0)

#define crtl_splay_linkright(head, tmp, field) do {                                \
  crtl_splay_right(tmp, field) = (head)->sph_root;                                 \
  tmp = (head)->sph_root;                                                     \
  (head)->sph_root = crtl_splay_right((head)->sph_root, field);                    \
} while (/*constcond*/ 0)

#define crtl_splay_assemble(head, node, left, right, field) do {                   \
  crtl_splay_right(left, field) = crtl_splay_left((head)->sph_root, field);             \
  crtl_splay_left(right, field) = crtl_splay_right((head)->sph_root, field);            \
  crtl_splay_left((head)->sph_root, field) = crtl_splay_right(node, field);             \
  crtl_splay_right((head)->sph_root, field) = crtl_splay_left(node, field);             \
} while (/*constcond*/ 0)

/* generates prototypes and inline functions */

#define crtl_splay_prototype(name, type, field, cmp)                               \
void name##_splay(struct name *, struct type *);                              \
void name##_crtl_splay_minmax(struct name *, int);                                 \
struct type *name##_crtl_splay_insert(struct name *, struct type *);               \
struct type *name##_crtl_splay_remove(struct name *, struct type *);               \
                                                                              \
/* finds the node with the same key as elm */                                 \
static __inline struct type *                                                 \
name##_crtl_splay_find(struct name *head, struct type *elm)                        \
{                                                                             \
  if (crtl_splay_empty(head))                                                      \
    return(NULL);                                                             \
  name##_splay(head, elm);                                                    \
  if ((cmp)(elm, (head)->sph_root) == 0)                                      \
    return (head->sph_root);                                                  \
  return (NULL);                                                              \
}                                                                             \
                                                                              \
static __inline struct type *                                                 \
name##_crtl_splay_next(struct name *head, struct type *elm)                        \
{                                                                             \
  name##_splay(head, elm);                                                    \
  if (crtl_splay_right(elm, field) != NULL) {                                      \
    elm = crtl_splay_right(elm, field);                                            \
    while (crtl_splay_left(elm, field) != NULL) {                                  \
      elm = crtl_splay_left(elm, field);                                           \
    }                                                                         \
  } else                                                                      \
    elm = NULL;                                                               \
  return (elm);                                                               \
}                                                                             \
                                                                              \
static __inline struct type *                                                 \
name##_crtl_splay_min_max(struct name *head, int val)                              \
{                                                                             \
  name##_crtl_splay_minmax(head, val);                                             \
  return (crtl_splay_root(head));                                                  \
}

/* main splay operation.
 * moves node close to the key of elm to top
 */
#define crtl_splay_generate(name, type, field, cmp)                                \
struct type *                                                                 \
name##_crtl_splay_insert(struct name *head, struct type *elm)                      \
{                                                                             \
    if (crtl_splay_empty(head)) {                                                  \
      crtl_splay_left(elm, field) = crtl_splay_right(elm, field) = NULL;                \
    } else {                                                                  \
      int __comp;                                                             \
      name##_splay(head, elm);                                                \
      __comp = (cmp)(elm, (head)->sph_root);                                  \
      if(__comp < 0) {                                                        \
        crtl_splay_left(elm, field) = crtl_splay_left((head)->sph_root, field);         \
        crtl_splay_right(elm, field) = (head)->sph_root;                           \
        crtl_splay_left((head)->sph_root, field) = NULL;                           \
      } else if (__comp > 0) {                                                \
        crtl_splay_right(elm, field) = crtl_splay_right((head)->sph_root, field);       \
        crtl_splay_left(elm, field) = (head)->sph_root;                            \
        crtl_splay_right((head)->sph_root, field) = NULL;                          \
      } else                                                                  \
        return ((head)->sph_root);                                            \
    }                                                                         \
    (head)->sph_root = (elm);                                                 \
    return (NULL);                                                            \
}                                                                             \
                                                                              \
struct type *                                                                 \
name##_crtl_splay_remove(struct name *head, struct type *elm)                      \
{                                                                             \
  struct type *__tmp;                                                         \
  if (crtl_splay_empty(head))                                                      \
    return (NULL);                                                            \
  name##_splay(head, elm);                                                    \
  if ((cmp)(elm, (head)->sph_root) == 0) {                                    \
    if (crtl_splay_left((head)->sph_root, field) == NULL) {                        \
      (head)->sph_root = crtl_splay_right((head)->sph_root, field);                \
    } else {                                                                  \
      __tmp = crtl_splay_right((head)->sph_root, field);                           \
      (head)->sph_root = crtl_splay_left((head)->sph_root, field);                 \
      name##_splay(head, elm);                                                \
      crtl_splay_right((head)->sph_root, field) = __tmp;                           \
    }                                                                         \
    return (elm);                                                             \
  }                                                                           \
  return (NULL);                                                              \
}                                                                             \
                                                                              \
void                                                                          \
name##_splay(struct name *head, struct type *elm)                             \
{                                                                             \
  struct type __node, *__left, *__right, *__tmp;                              \
  int __comp;                                                                 \
                                                                              \
  crtl_splay_left(&__node, field) = crtl_splay_right(&__node, field) = NULL;            \
  __left = __right = &__node;                                                 \
                                                                              \
  while ((__comp = (cmp)(elm, (head)->sph_root)) != 0) {                      \
    if (__comp < 0) {                                                         \
      __tmp = crtl_splay_left((head)->sph_root, field);                            \
      if (__tmp == NULL)                                                      \
        break;                                                                \
      if ((cmp)(elm, __tmp) < 0){                                             \
        crtl_splay_rotate_right(head, __tmp, field);                               \
        if (crtl_splay_left((head)->sph_root, field) == NULL)                      \
          break;                                                              \
      }                                                                       \
      crtl_splay_linkleft(head, __right, field);                                   \
    } else if (__comp > 0) {                                                  \
      __tmp = crtl_splay_right((head)->sph_root, field);                           \
      if (__tmp == NULL)                                                      \
        break;                                                                \
      if ((cmp)(elm, __tmp) > 0){                                             \
        crtl_splay_rotate_left(head, __tmp, field);                                \
        if (crtl_splay_right((head)->sph_root, field) == NULL)                     \
          break;                                                              \
      }                                                                       \
      crtl_splay_linkright(head, __left, field);                                   \
    }                                                                         \
  }                                                                           \
  crtl_splay_assemble(head, &__node, __left, __right, field);                      \
}                                                                             \
                                                                              \
/* splay with either the minimum or the maximum element                       \
 * used to find minimum or maximum element in tree.                           \
 */                                                                           \
void name##_crtl_splay_minmax(struct name *head, int __comp)                       \
{                                                                             \
  struct type __node, *__left, *__right, *__tmp;                              \
                                                                              \
  crtl_splay_left(&__node, field) = crtl_splay_right(&__node, field) = NULL;            \
  __left = __right = &__node;                                                 \
                                                                              \
  while (1) {                                                                 \
    if (__comp < 0) {                                                         \
      __tmp = crtl_splay_left((head)->sph_root, field);                            \
      if (__tmp == NULL)                                                      \
        break;                                                                \
      if (__comp < 0){                                                        \
        crtl_splay_rotate_right(head, __tmp, field);                               \
        if (crtl_splay_left((head)->sph_root, field) == NULL)                      \
          break;                                                              \
      }                                                                       \
      crtl_splay_linkleft(head, __right, field);                                   \
    } else if (__comp > 0) {                                                  \
      __tmp = crtl_splay_right((head)->sph_root, field);                           \
      if (__tmp == NULL)                                                      \
        break;                                                                \
      if (__comp > 0) {                                                       \
        crtl_splay_rotate_left(head, __tmp, field);                                \
        if (crtl_splay_right((head)->sph_root, field) == NULL)                     \
          break;                                                              \
      }                                                                       \
      crtl_splay_linkright(head, __left, field);                                   \
    }                                                                         \
  }                                                                           \
  crtl_splay_assemble(head, &__node, __left, __right, field);                      \
}

#define __CRTL_SPLAY_NEGINF  -1
#define __CRTL_SPLAY_INF     1

#define crtl_splay_insert(name, x, y)  name##_crtl_splay_insert(x, y)
#define crtl_splay_remove(name, x, y)  name##_crtl_splay_remove(x, y)
#define crtl_splay_find(name, x, y)    name##_crtl_splay_find(x, y)
#define crtl_splay_next(name, x, y)    name##_crtl_splay_next(x, y)
#define crtl_splay_min(name, x)        (crtl_splay_empty(x) ? NULL                      \
                                  : name##_crtl_splay_min_max(x, __CRTL_SPLAY_NEGINF))
#define crtl_splay_max(name, x)        (crtl_splay_empty(x) ? NULL                      \
                                  : name##_crtl_splay_min_max(x, __CRTL_SPLAY_INF))

#define crtl_splay_foreach(x, name, head)                                          \
  for ((x) = crtl_splay_min(name, head);                                           \
       (x) != NULL;                                                           \
       (x) = crtl_splay_next(name, head, x))



#endif /*<__CRTL_BITS_TREE_SPLAY_H>*/


