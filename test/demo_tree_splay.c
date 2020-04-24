#include "crtl/crtl_tree.h"
#include "crtl/crtl_log.h"


struct test_data {
    int a;
    crtl_splay_entry(test_data) field;
};
static int test_date_cmp(const struct test_data *d1, const struct test_data *d2)
{
    return d1->a - d2->a;
}



crtl_splay_head(tree_head, test_data) tree_root = crtl_splay_initializer(tree_head);

crtl_splay_prototype(tree_head, test_data, field, test_date_cmp);
crtl_splay_generate(tree_head, test_data, field, test_date_cmp);



struct test_data my_data[10] = {
    {1},{2},{6},{7},{8},{9},{3},{4},{5}
};





static int demo_crtl_splay_tree_test1()
{
    crtl_splay_init(&tree_root);

    int i;
    for(i=0;i<10;i++)
    crtl_splay_insert(tree_head, &tree_root, &my_data[i]);

    struct test_data *find = crtl_splay_find(tree_head, &tree_root, &my_data[2]);
    find = crtl_splay_find(tree_head, &tree_root, &my_data[2]);
    find = crtl_splay_find(tree_head, &tree_root, &my_data[2]);
    find = crtl_splay_find(tree_head, &tree_root, &my_data[2]);
    find = crtl_splay_find(tree_head, &tree_root, &my_data[2]);
    find = crtl_splay_find(tree_head, &tree_root, &my_data[2]);
    printf("-----------------------\n");
    printf("%d\n", find->a);
    
    printf("-----------------------\n");
    crtl_splay_foreach(find, tree_head, &tree_root) {
        printf("%d\n", find->a);
    }
    
    printf("-----------------------\n");
    find = NULL;
    find = crtl_splay_remove(tree_head, &tree_root, &my_data[2]);
    printf("%d\n", find->a);
    
    printf("-----------------------\n");
    crtl_splay_foreach(find, tree_head, &tree_root) {
        printf("%d\n", find->a);
    }
}




int main(void)
{
    demo_crtl_splay_tree_test1();


    return 0;
}

