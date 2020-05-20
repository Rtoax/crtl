
#ifndef xchg
#define xchg(ptr,v) ((__typeof__(*(ptr)))__xchg((unsigned long) \
                                         (v),(ptr),sizeof(*(ptr))))
 
static inline unsigned long __xchg(unsigned long x, volatile void * ptr, int size)
{
	unsigned long flags, tmp;

	(void)(flags);

	switch (size) {
	case 1:
		tmp = *(unsigned char *)ptr;
		*(unsigned char *)ptr = x;
		x = tmp;
		break;
	case 2:
		tmp = *(unsigned short *)ptr;
		*(unsigned short *)ptr = x;
		x = tmp;
		break;
	case 4:
		tmp = *(unsigned int *)ptr;
		*(unsigned int *)ptr = x;
		x = tmp;
		break;
	case 8:
		tmp = *(unsigned long *)ptr;
		*(unsigned long *)ptr = x;
		x = tmp;
		break;
	default:
		break;
	}

	(void)(flags);
	return x;
}
#endif //xchg


#ifndef cmpxchg
#define cmpxchg(ptr, o, n)		    \
({									\
	__typeof__(*(ptr)) __o = (o);					\
	__typeof__(*(ptr)) __n = (n);					\
	(__typeof__(*(ptr))) __sync_val_compare_and_swap((ptr),__o,__n);\
})
#endif

