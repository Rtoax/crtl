
#ifndef VCAS
#undef VCAS
#define VCAS(ptr, oldval, newold) __sync_val_compare_and_swap(ptr, oldval, newold)
#endif

#ifndef CAS
#undef CAS
#define CAS(loc, old_value, new_value) __sync_bool_compare_and_swap((void **)loc, old_value, new_value)
#endif
