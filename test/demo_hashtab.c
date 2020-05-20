#include "crtl/bits/hashtab.h"
#include "crtl/log.h"



/**********************************************************************************************************************/

int rt_crtl_hashtab_test_display(void *k, void *d, void *args)
{
    printf("%s: %s\n", (char*)k, (char*)d);


    return CRTL_SUCCESS;
}

void demo_crtl_crtl_hashtab__test(int argc, char *argv[])
{
    int size = 135;
    struct crtl_hashtab_struct *ht = crtl_hashtab_create(NULL, NULL, size);
    struct crtl_hashtab_info info;
    
    crtl_print_debug("Init \n");
    char *key = NULL; 
    char *dat = NULL; 

    int i;
    for(i=0;i<size;i++)
    {
        key = malloc(10);
        dat = malloc(20);
        memset(key, 0, 10);
        memset(dat, 0, 20);
        sprintf(key, "%d", i);
        sprintf(dat, "rongtao%d", i);
        crtl_hashtab_insert(ht, key, dat);
        
        crtl_hashtab_stat(ht, &info);
        crtl_print_debug("Insert: %s = %s, clen=%2d, used=%2d\n", key, dat, info.max_chain_len, info.slots_used);
    }

    char search[10] = {0};
    for(i=0;i<size;i+=3)
    {
        sprintf(search, "%d", i);
        crtl_print_notice("Search %s = %s\n", search, (char*)crtl_hashtab_search(ht, search));
    }
    
    crtl_hashtab_map(ht, rt_crtl_hashtab_test_display, NULL);

    crtl_hashtab_destroy(ht);

    ht = NULL;
    
    crtl_hashtab_map(ht, rt_crtl_hashtab_test_display, NULL);

    
}


int main(int argc, char *argv[])
{
    demo_crtl_crtl_hashtab__test(argc, argv);
    
    return 0;
}

