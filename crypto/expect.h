

#ifndef likely
#define likely(exp) __builtin_expect(!!(exp), 1)
#endif 

#ifndef unlikely
#define unlikely(exp) __builtin_expect(!!(exp), 0)
#endif

