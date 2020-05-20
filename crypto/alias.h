
#ifndef weak_alias
#define weak_alias(name, aliasname) extern typeof (name) aliasname __attribute__ ((weak, alias(#name)))
#endif

#ifndef weak_hidden_alias
#define weak_hidden_alias(name, aliasname) extern __typeof (name) aliasname __attribute__ ((weak, alias (#name), __visibility__ ("hidden")));
#endif

#ifndef strong_alias
#define strong_alias(name, aliasname)  extern __typeof (name) aliasname __attribute__ ((alias (#name)));
#endif

