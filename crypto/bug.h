#ifndef __CRTL_CRYPTO_BUG_H
#define __CRTL_CRYPTO_BUG_H 1

#include <crtl/assert.h>
#include <crypto/expect.h>

#define BUG() crtl_assert(0)

#define BUG_ON(x) crtl_assert(!(x))

#define WARN_ON(condition) ({                                           \
	int __ret_warn_on = !!(condition);                              \
	unlikely(__ret_warn_on);                                        \
})

#define WARN_ON_ONCE(condition) ({                              \
	int __ret_warn_on = !!(condition);                      \
	if (unlikely(__ret_warn_on))                            \
		crtl_assert(0);                                      \
	unlikely(__ret_warn_on);                                \
})

#endif /*<__CRTL_CRYPTO_BUG_H>*/
