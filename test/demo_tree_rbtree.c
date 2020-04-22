
#include <crtl/crtl_tree.h>
#include <crtl/crtl_log.h>



struct mytype {
	struct rb_node my_node;
	int num;
};

struct mytype *my_search(struct rb_root *root, int num)
{
	struct rb_node *node = root->rb_node;
	while(node)
	{
		struct mytype *data = container_of(node, struct mytype, my_node);
		if(num < data->num)
		{
			node = node->rb_left;
		}
		else if(num > data->num)
		{
			node = node->rb_right;
		}
		else
			return data;
	}
	return NULL;
}

int my_insert(struct rb_root *root, struct mytype *data)
{
	struct rb_node **tmp = &(root->rb_node), *parent = NULL;
	while(*tmp)
	{
		struct mytype *this = container_of(*tmp, struct mytype, my_node);
		parent = *tmp;
		if(data->num < this->num)
		{
			tmp = &((*tmp)->rb_left);
		}
		else if(data->num > this->num)
		{
			tmp = &((*tmp)->rb_right);
		}
		else 
		{
			return -1;
		}
	}
	rb_link_node(&data->my_node, parent, tmp);
	rb_insert_color(&data->my_node, root);
	return 0;
}

void my_delete(struct rb_root *root, int num)
{
    struct mytype *data = my_search(root, num);
    if(!data)
    {
        crtl_print_debug("Not found %d.\n", num);
        return;
    }
    rb_erase(&data->my_node, root);
    free(data);
}

void print_rbtree(struct rb_root *tree)
{
    struct rb_node *node;
    for(node=rb_first(tree); node; node=rb_next(node))
    {
        crtl_print_debug("%2d \n", rb_entry(node, struct mytype, my_node)->num);
    }
    crtl_print_debug("\n");
}

void demo_rbtree_original()
{
    struct rb_root mytree = RB_ROOT;
    int i, ret, num, nums[] = {2,6,9,8,3,4,7,1,12,1,5,6,9,83,6, 4,87,65,45,85,21,36,64,74,75};
    struct mytype *tmp;

    for(i=0;i<sizeof(nums)/sizeof(nums[0]);i++)
    {
        tmp = malloc(sizeof(struct mytype));
        if(!tmp)
            crtl_print_debug("Allocate dynamic memory error.\n");

        num = nums[i];
        tmp->num = num;
        
        ret = my_insert(&mytree, tmp);
        if(ret <0)
        {
            crtl_print_debug("The %d already exist.\n", tmp->num);
            free(tmp);
        }
    }
    print_rbtree(&mytree);

    my_delete(&mytree, 3);
    
    print_rbtree(&mytree);
}


struct structA{
    int a;
};
int demo_crtl_rbtree_cmp(const void *d1, const void *d2)
{
    struct structA *a1 = (struct structA *)d1;
    struct structA *a2 = (struct structA *)d2;
    if(a1->a > a2->a) return CRTL_GT;
    else if(a1->a == a2->a) return CRTL_EQ;
    else if(a1->a < a2->a) return CRTL_LT;

    return CRTL_EQ;
}
int demo_crtl_rbtree_display(const void *data)
{
    struct structA *a1 = (struct structA *)data;
    return printf("%d\n", a1->a);
}

struct structA aaa[] = {{71},{64},{5},{7},{12}};

void demo_crtl_rbtree()
{
    crtl_rbtree_t rbt = crtl_rbtree_init(&demo_crtl_rbtree_cmp, &demo_crtl_rbtree_display);

    crtl_print_debug(">>nnode %d.\n", crtl_rbtree_nnode(rbt));
    crtl_print_debug(">>nnode %d. is empty %s\n", 
                                crtl_rbtree_nnode(rbt), crtl_rbtree_is_empty(rbt)==CRTL_SUCCESS?"YES":"NO");

    int i;
    for(i=0;i<sizeof(aaa)/sizeof(aaa[0]); i++)
        crtl_rbtree_insert(rbt, &aaa[i], sizeof(struct structA));

    crtl_print_debug(">>nnode %d.\n", crtl_rbtree_nnode(rbt));
    crtl_print_debug(">>nnode %d. is empty %s\n", 
                                crtl_rbtree_nnode(rbt), crtl_rbtree_is_empty(rbt)==CRTL_SUCCESS?"YES":"NO");


    crtl_rbtree_node_t *node = NULL;
    for(node=crtl_rbtree_getfirst(rbt); node; node = crtl_rbtree_getnext(node))
    {
        struct structA *a1 = (struct structA *)(node->data);
        rbt->display(a1);
    }
    crtl_print_debug(">>nnode %d.\n", crtl_rbtree_nnode(rbt));
    crtl_print_debug(">>nnode %d. is empty %s\n", 
                                crtl_rbtree_nnode(rbt), crtl_rbtree_is_empty(rbt)==CRTL_SUCCESS?"YES":"NO");
    crtl_rbtree_delete(rbt, &aaa[3]);
    crtl_print_debug(">>nnode %d.\n", crtl_rbtree_nnode(rbt));
    crtl_rbtree_delete(rbt, &aaa[2]);
    crtl_print_debug(">>nnode %d.\n", crtl_rbtree_nnode(rbt));
    crtl_print_debug(">>nnode %d. is empty %s\n", 
                                crtl_rbtree_nnode(rbt), crtl_rbtree_is_empty(rbt)==CRTL_SUCCESS?"YES":"NO");
    
    node = NULL;
    for(node=crtl_rbtree_getfirst(rbt); node; node = crtl_rbtree_getnext(node))
    {
        struct structA *a1 = CRTL_RBTREE_DATA(node);
        rbt->display(a1);
    }
    crtl_print_debug(">>nnode %d.\n", crtl_rbtree_nnode(rbt));

    
    crtl_print_debug("Use crtl_rbtree_iterator_t\n");
    
    struct structA *a1 = NULL;
    crtl_rbtree_iterator_t iter = crtl_rbtree_iterator(rbt);
    for(a1 = iter->first(iter); a1; a1 = iter->next(iter))
    {
        rbt->display(a1);
    }
    for(a1 = iter->prev(iter); a1; a1 = iter->prev(iter))
    {
        rbt->display(a1);
    }
    for(a1 = iter->next(iter); a1; a1 = iter->next(iter))
    {
        rbt->display(a1);
    }
    
    crtl_rbtree_destroy(rbt);
}


int main()
{
//    demo_rbtree_original();

    demo_crtl_rbtree();

    return 0;
}

