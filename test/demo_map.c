#include <crtl/bits/crtl_map.h>
#include <crtl/crtl_log.h>


void demo_map_test1()
{
#if 0
    map_int_t m;
    map_init(&m);
    map_set(&m, "testkey", 123);
    map_set(&m, "testkey4", 12412);
    map_set(&m, "testk1ey4", 1412);
    map_set(&m, "testk2ey4", 124312);
    int *val = map_get(&m, "testkey");
    if (val) {
      printf("value: %d\n", *val);
    } else {
      printf("value not found\n");
    }
    map_deinit(&m);
    //Contained Type  | Type name
    //----------------|----------------------------------
    //void*           | map_void_t
    //char*           | map_str_t
    //int             | map_int_t
    //char            | map_char_t
    //float           | map_float_t
    //double          | map_double_t
    /* Creates the type uint_map_t for storing unsigned ints */
    typedef map_t(unsigned int) uint_map_t;
    /* Typedefs the struct `fp_map_t` as a container for type FILE* */
    typedef map_t(FILE*) fp_map_t;
    const char *key;
    map_iter_t iter = map_iter(&m);

    while ((key = map_next(&m, &iter))) {
      printf("%s -> %d", key, *map_get(&m, key));
    }
#endif    
}

int main()
{
    
    demo_map_test1();
    

    return 0;
}


