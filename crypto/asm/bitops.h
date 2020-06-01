
#ifndef BOOT_BITOPS_H
#define BOOT_BITOPS_H
#define _LINUX_BITOPS_H		/* Inhibit inclusion of <linux/bitops.h> */

#include <sys/types.h>

#ifdef __GCC_ASM_FLAG_OUTPUTS__
# define CC_SET(c) "\n\t/* output condition code " #c "*/\n"
# define CC_OUT(c) "=@cc" #c
#else
# define CC_SET(c) "\n\tset" #c " %[_cc_" #c "]\n"
# define CC_OUT(c) [_cc_ ## c] "=qm"
#endif

static inline bool constant_test_bit(int nr, const void *addr)
{
	const unsigned int *p = (const unsigned int *)addr;
	return ((1UL << (nr & 31)) & (p[nr >> 5])) != 0;
}
static inline bool variable_test_bit(int nr, const void *addr)
{
	bool v;
	const unsigned int *p = (const unsigned int *)addr;

	asm("btl %2,%1" CC_SET(c) : CC_OUT(c) (v) : "m" (*p), "Ir" (nr));
	return v;
}

#define test_bit(nr,addr) (__builtin_constant_p((nr)) ? constant_test_bit((nr),(addr)) : variable_test_bit((nr),(addr)))

static inline void set_bit(int nr, void *addr)
{
	asm("btsl %1,%0" : "+m" (*(unsigned int *)addr) : "Ir" (nr));
}

#endif /* BOOT_BITOPS_H */

