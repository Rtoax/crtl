#include <crtl/crtl_list.h>



struct list_test {
	struct list_head list;
	int a;
#define LIST_TEST_INITIALIZER(v)   {{NULL, NULL}, v}
};

struct hlist_test {
	struct hlist_head list;
	int a;
#define HLIST_TEST_INITIALIZER(v)   {{NULL}, v}
};



int demo_list_1_add_del_swap_for_each()
{
    struct list_head lhead;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);
    
    list_add_tail(&lt1.list, &lhead);
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);
    list_add_tail(&lt4.list, &lhead);
    list_add_tail(&lt5.list, &lhead);

    list_del_init(&lt3.list);
	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    list_replace_init(&lt4.list, &lt3.list);
	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_swap(&lt1.list, &lt5.list);
	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    return 0;
}



int demo_list_2_move_and_move_tail()
{
    struct list_head lhead1, lhead2;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead1);
    INIT_LIST_HEAD(&lhead2);
    
    list_add_tail(&lt1.list, &lhead1);
    list_add_tail(&lt2.list, &lhead1);
    list_add_tail(&lt3.list, &lhead1);
    list_add_tail(&lt4.list, &lhead2);
    list_add_tail(&lt5.list, &lhead2);

    list_for_each_entry(iter, &lhead1, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_move(&lt1.list, &lhead2);

    list_for_each_entry(iter, &lhead1, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_move_tail(&lt2.list, &lhead2);

    list_for_each_entry(iter, &lhead1, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    return 0;
}


int demo_list_3_bulk_move()
{
    struct list_head lhead1, lhead2;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead1);
    INIT_LIST_HEAD(&lhead2);
    
    list_add_tail(&lt1.list, &lhead1);
    list_add_tail(&lt2.list, &lhead1);
    list_add_tail(&lt3.list, &lhead1);
    list_add_tail(&lt4.list, &lhead2);
    list_add_tail(&lt5.list, &lhead2);

    list_for_each_entry(iter, &lhead1, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_bulk_move_tail(&lhead2, &lt1.list, &lt2.list);

    list_for_each_entry(iter, &lhead1, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");


    return 0;
}


int demo_list_4_is_first_is_last_is_empty_is_singular()
{
    struct list_head lhead;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);

    if(list_empty(&lhead))
    {
        printf("is empty.\n");
    }
    if(list_empty_careful(&lhead))
    {
        printf("is list_empty_careful.\n");
    }
    
    list_add_tail(&lt1.list, &lhead);

    
    if(list_is_singular(&lhead))
    {
        printf("is list_is_singular.\n");
    }
    
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);
    list_add_tail(&lt4.list, &lhead);
    list_add_tail(&lt5.list, &lhead);

    
    if(list_empty(&lhead))
    {
        printf("is empty.\n");
    }

	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    if(list_is_first(&lt1.list, &lhead))
    {
        printf("is first.\n");
    }
    if(list_is_last(&lt1.list, &lhead))
    {
        printf("is last.\n");
    }
    if(list_is_first(&lt5.list, &lhead))
    {
        printf("is first.\n");
    }
    if(list_is_last(&lt5.list, &lhead))
    {
        printf("is last.\n");
    }

    return 0;
}



int demo_list_5_rotate()
{
    struct list_head lhead;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);
    
    list_add_tail(&lt1.list, &lhead);
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);
    list_add_tail(&lt4.list, &lhead);
    list_add_tail(&lt5.list, &lhead);

	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_rotate_left(&lhead);
    
    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    list_rotate_to_front(&lt4.list, &lhead);
    
    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
//    list_rotate_left(&lhead);
//
//
//    list_for_each_entry(iter, &lhead, list) {
//		printf("%d - ", iter->a);
//	}printf("\n");
    
    return 0;
}



int demo_list_6_cut()
{
    struct list_head lhead;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);
    
    list_add_tail(&lt1.list, &lhead);
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);
    list_add_tail(&lt4.list, &lhead);
    list_add_tail(&lt5.list, &lhead);

	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    struct list_head lhead2;
    
    list_cut_position(&lhead2, &lhead, &lt3.list);
//    1 - 2 - 3 - 4 - 5 - 
//    4 - 5 - 
//    1 - 2 - 3 - 

//    list_cut_before(&lhead2, &lhead, &lt3.list);
//    1 - 2 - 3 - 4 - 5 - 
//    3 - 4 - 5 - 
//    1 - 2 - 


    
    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    
    return 0;
}


int demo_list_7_splice()//粘贴
{
    struct list_head lhead;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);
    
    list_add_tail(&lt1.list, &lhead);
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);

    struct list_head lhead2;
    INIT_LIST_HEAD(&lhead2);
    
    list_add_tail(&lt4.list, &lhead2);
    list_add_tail(&lt5.list, &lhead2);
    
    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");


#if 1
    list_splice(&lhead2, &lhead);

//    list_splice_tail(&lhead2, &lhead);
    
    list_for_each_entry(iter, &lhead, list) {
        printf("%d - ", iter->a);
    }printf("\n");
#else

    list_splice_init(&lhead2, &lhead);

//    list_splice_tail_init(&lhead2, &lhead);
    
    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

#endif

    return 0;
}



int demo_list_8_for_each_and_entry()
{
    struct list_head lhead;
    int __a = 13;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    struct list_test lt6 = LIST_TEST_INITIALIZER(6);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);
    
    
    list_add_tail(&lt1.list, &lhead);
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);
    list_add_tail(&lt4.list, &lhead);
    list_add_tail(&lt5.list, &lhead);

	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    iter = list_first_entry(&lhead, struct list_test, list);
    printf("list_first_entry:   %d - \n", iter->a);    
    
    iter = list_last_entry(&lhead, struct list_test, list);
    printf("list_last_entry:    %d - \n", iter->a);

    
    iter = list_first_entry_or_null(&lhead, struct list_test, list);
    printf("list_first_entry_or_null:   %d - \n", iter->a);

    iter = list_next_entry(iter, list);
    printf("list_next_entry:   %d - \n", iter->a);

    
    iter = list_prev_entry(iter, list);
    printf("list_prev_entry:   %d - \n", iter->a);

    struct list_head *lh_iter;
    
    list_for_each(lh_iter, &lhead){
		printf("%p - ", lh_iter);
	}printf("\n");
    
    printf("list_for_each_prev\n");
    list_for_each_prev(lh_iter, &lhead){
		printf("%p - ", lh_iter);
	}printf("\n");

    struct list_head *lh_n;
    
    printf("list_for_each_safe\n");
    list_for_each_safe(lh_iter, lh_n, &lhead){
		printf("%p - ", lh_iter);
	}printf("\n");
    
    printf("list_for_each_prev_safe\n");
    list_for_each_prev_safe(lh_iter, lh_n, &lhead){
		printf("%p - ", lh_iter);
	}printf("\n");


    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_for_each_entry_reverse(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    iter = &lt3;
    list_prepare_entry(iter, &lhead, list); //TODO
	printf("list_prepare_entry:     %d\n", iter->a);
    
    list_for_each_entry_continue(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    list_for_each_entry_continue_reverse(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    iter = &lt3;
    printf("list_for_each_entry_from\n");
    list_for_each_entry_from(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    iter = &lt3;
    printf("list_for_each_entry_from_reverse\n");
    list_for_each_entry_from_reverse(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    
    struct list_test *iter_next;
    printf("list_for_each_entry_safe\n");
    list_for_each_entry_safe(iter,iter_next, &lhead, list) {
        if(list_is_last(&iter->list, &lhead))
            printf("%d - \n", iter->a);
		else printf("%d - %d\n", iter->a, iter_next->a);
	}printf("\n");
    
    printf("list_for_each_entry_safe\n");
    list_for_each_entry_safe(iter,iter_next, &lhead, list) {
        list_safe_reset_next(iter, iter_next, list);
		printf("%d - %d\n", iter->a, iter_next->a);
        list_safe_reset_next(iter, iter_next, list);
	}printf("\n");
    
    
    list_add(&lt6.list, &lt3.list);
    printf("list_for_each_entry_safe\n");
    list_for_each_entry_safe(iter,iter_next, &lhead, list) {
		printf("%d - ", iter->a);
        list_safe_reset_next(iter, iter_next, list);
	}printf("\n");
    
    list_del(&lt6.list);
    printf("list_for_each_entry_safe\n");
    list_for_each_entry_safe(iter,iter_next, &lhead, list) {
		printf("%d - ", iter->a);
        list_safe_reset_next(iter, iter_next, list);
	}printf("\n");
    

    iter = &lt3;
    printf("list_for_each_entry_safe_continue\n");
    list_for_each_entry_safe_continue(iter, iter_next, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    iter = &lt3;
    printf("list_for_each_entry_safe_from\n");
    list_for_each_entry_safe_from(iter, iter_next, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    printf("list_for_each_entry_safe_reverse\n");
    list_for_each_entry_safe_reverse(iter, iter_next, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    

    
    return 0;
}

static int list_sort_test_cmp(void *priv, struct list_head *a, struct list_head *b)
{
    struct list_test *t1 = container_of(a, struct list_test, list);
    struct list_test *t2 = container_of(b, struct list_test, list);
    return t1->a-t2->a;
}

void demo_list_9_list_sort()
{
    printf("sort:::::::::::::::::::::::\n");
    struct list_head lhead;
    int __a = 13;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    struct list_test lt6 = LIST_TEST_INITIALIZER(6);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);
    
    
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt4.list, &lhead);
    list_add_tail(&lt5.list, &lhead);
    list_add_tail(&lt1.list, &lhead);
    list_add_tail(&lt3.list, &lhead);

    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_sort(NULL, &lhead, &list_sort_test_cmp);
    
    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
}


int main()
{
    demo_list_1_add_del_swap_for_each();
    demo_list_2_move_and_move_tail();
    demo_list_3_bulk_move();
    demo_list_4_is_first_is_last_is_empty_is_singular();
    demo_list_5_rotate();
    demo_list_6_cut();
    demo_list_7_splice();
    demo_list_8_for_each_and_entry();
    demo_list_9_list_sort();


    return 0;
}
