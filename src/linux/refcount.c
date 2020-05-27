#include "crypto/atomic/refcount.h"


void crtl_refcount_warn_saturate(crtl_refcount_t *r, enum crtl_refcount_saturation_type t)
{
	crtl_refcount_set(r, CRTL_REFCOUNT_SATURATED);

	switch (t) {
	case CRTL_REFCOUNT_ADD_NOT_ZERO_OVF:
		REFCOUNT_WARN("saturated; leaking memory");
		break;
	case CRTL_REFCOUNT_ADD_OVF:
		REFCOUNT_WARN("saturated; leaking memory");
		break;
	case CRTL_REFCOUNT_ADD_UAF:
		REFCOUNT_WARN("addition on 0; use-after-free");
		break;
	case CRTL_REFCOUNT_SUB_UAF:
		REFCOUNT_WARN("underflow; use-after-free");
		break;
	case CRTL_REFCOUNT_DEC_LEAK:
		REFCOUNT_WARN("decrement hit 0; leaking memory");
		break;
	default:
		REFCOUNT_WARN("unknown saturation event!?");
	}
}

