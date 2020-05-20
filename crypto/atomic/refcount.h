#ifndef __CRYPTO_BITS_REFCOUNT_H
#define __CRYPTO_BITS_REFCOUNT_H 1

#include <limits.h>
#include <stdio.h>

#include <crypto/atomic/atomic.h>


/**
 * struct refcount_t - variant of atomic_t specialized for reference counts
 * @refs: atomic_t counter field
 *
 * The counter saturates at REFCOUNT_SATURATED and will not move once
 * there. This avoids wrapping the counter and causing 'spurious'
 * use-after-free bugs.
 */
typedef struct crtl_refcount_struct {
	crtl_atomic_t refs;
} crtl_refcount_t;

#define CRTL_REFCOUNT_INIT(n)	{ .refs = CRTL_ATOMIC_INITIALIZER(n), }
#define CRTL_REFCOUNT_MAX		INT_MAX
#define CRTL_REFCOUNT_SATURATED	(INT_MIN / 2)

enum crtl_refcount_saturation_type {
	CRTL_REFCOUNT_ADD_NOT_ZERO_OVF,
	CRTL_REFCOUNT_ADD_OVF,
	CRTL_REFCOUNT_ADD_UAF,
	CRTL_REFCOUNT_SUB_UAF,
	CRTL_REFCOUNT_DEC_LEAK,
};

#define REFCOUNT_WARN(fmt) printf(fmt"\n")


void crtl_refcount_warn_saturate(crtl_refcount_t *r, enum crtl_refcount_saturation_type t);

/**
 * refcount_set - set a refcount's value
 * @r: the refcount
 * @n: value to which the refcount will be set
 */
static inline void crtl_refcount_set(crtl_refcount_t *r, int n)
{
	crtl_atomic_set(&r->refs, n);
}

/**
 * refcount_read - get a refcount's value
 * @r: the refcount
 *
 * Return: the refcount's value
 */
static inline unsigned int crtl_refcount_read(const crtl_refcount_t *r)
{
	return crtl_atomic_read(&r->refs);
}

/**
 * refcount_add_not_zero - add a value to a refcount unless it is 0
 * @i: the value to add to the refcount
 * @r: the refcount
 *
 * Will saturate at REFCOUNT_SATURATED and WARN.
 *
 * Provides no memory ordering, it is assumed the caller has guaranteed the
 * object memory to be stable (RCU, etc.). It does provide a control dependency
 * and thereby orders future stores. See the comment on top.
 *
 * Use of this function is not recommended for the normal reference counting
 * use case in which references are taken and released one at a time.  In these
 * cases, refcount_inc(), or one of its variants, should instead be used to
 * increment a reference count.
 *
 * Return: false if the passed refcount is 0, true otherwise
 */
static inline bool crtl_refcount_add_not_zero(int i, crtl_refcount_t *r)
{
#if 1
	int old = crtl_refcount_read(r);
	do {
		if (!old)
			break;
	} while (!crtl_atomic_try_cmpxchg_relaxed(&r->refs, &old, old + i));

	if (unlikely(old < 0 || old + i < 0))
		crtl_refcount_warn_saturate(r, CRTL_REFCOUNT_ADD_NOT_ZERO_OVF);

	return old;
#else
    return crtl_atomic_set(&r->refs, i);
#endif
}

/**
 * refcount_add - add a value to a refcount
 * @i: the value to add to the refcount
 * @r: the refcount
 *
 * Similar to atomic_add(), but will saturate at REFCOUNT_SATURATED and WARN.
 *
 * Provides no memory ordering, it is assumed the caller has guaranteed the
 * object memory to be stable (RCU, etc.). It does provide a control dependency
 * and thereby orders future stores. See the comment on top.
 *
 * Use of this function is not recommended for the normal reference counting
 * use case in which references are taken and released one at a time.  In these
 * cases, refcount_inc(), or one of its variants, should instead be used to
 * increment a reference count.
 */
static inline void crtl_refcount_add(int i, crtl_refcount_t *r)
{
#if 0
	int old = atomic_fetch_add_relaxed(i, &r->refs); //原子访存添加轻松

	if (unlikely(!old))
		crtl_refcount_warn_saturate(r, CRTL_REFCOUNT_ADD_UAF);
	else if (unlikely(old < 0 || old + i < 0))
		crtl_refcount_warn_saturate(r, CRTL_REFCOUNT_ADD_OVF);
#else
    crtl_refcount_add_not_zero(i, r);
#endif
}

/**
 * refcount_inc_not_zero - increment a refcount unless it is 0
 * @r: the refcount to increment
 *
 * Similar to atomic_inc_not_zero(), but will saturate at REFCOUNT_SATURATED
 * and WARN.
 *
 * Provides no memory ordering, it is assumed the caller has guaranteed the
 * object memory to be stable (RCU, etc.). It does provide a control dependency
 * and thereby orders future stores. See the comment on top.
 *
 * Return: true if the increment was successful, false otherwise
 */
static inline bool crtl_refcount_inc_not_zero(crtl_refcount_t *r)
{
	return crtl_refcount_add_not_zero(1, r);
}

/**
 * refcount_inc - increment a refcount
 * @r: the refcount to increment
 *
 * Similar to atomic_inc(), but will saturate at REFCOUNT_SATURATED and WARN.
 *
 * Provides no memory ordering, it is assumed the caller already has a
 * reference on the object.
 *
 * Will WARN if the refcount is 0, as this represents a possible use-after-free
 * condition.
 */
static inline void crtl_refcount_inc(crtl_refcount_t *r)
{
	crtl_refcount_add(1, r);
}


#endif /*<__CRYPTO_BITS_REFCOUNT_H>*/

