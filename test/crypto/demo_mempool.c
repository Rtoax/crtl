#include <malloc.h>
#include "crypto/memory/mempool.h"
#include <crtl/log.h>


struct mempooltest {
    int id;
};

void* myalloc_t(void *pool_data)
{
    static int num = 0;
    crtl_print_info("my malloc: %d\n", ++num);
    malloc(sizeof(struct mempooltest));
}
void myfree_t(void *element, void *pool_data)
{
    static int num = 0;
    crtl_print_info("my free: %d\n", ++num);
    free(element);
}


void demo_crypto_mempool_test()
{
    mempool_t *mp1 = mempool_create(10, myalloc_t, myfree_t, NULL);
    
    struct mempooltest *t1 = mempool_alloc(mp1);
    
    mempool_resize(mp1, 12);


//    mempool_t mp2;
    
//    mempool_init(&mp2, 10, myalloc_t, myfree_t, NULL);
//    struct mempooltest *t2 = mempool_alloc(&mp2);
    
//    mempool_destroy(mp1);

}

int main()
{
    demo_crypto_mempool_test();
    
    
    
    return 0;
}

