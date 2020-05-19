#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "crtl/bits/crtl_hashtab.h"
#include "crtl/log.h"



_api struct crtl_hashtab_struct *crtl_hashtab_create(uint32_t (*hash_value)(struct crtl_hashtab_struct *h, const void *key),
			       int (*keycmp)(struct crtl_hashtab_struct *h, const void *key1, const void *key2),
			       uint32_t size)
{
	struct crtl_hashtab_struct *p;
	uint32_t i;

	//p = kzalloc(sizeof(*p), GFP_KERNEL);
	p = (struct crtl_hashtab_struct *)malloc(sizeof(struct crtl_hashtab_struct));
	if (p == NULL)
    {
        crtl_print_err("null pointer error.\n");
        return p;
    }

	p->size = size;
	p->nel = 0;
    if(hash_value)
	    p->hash_value = hash_value;
    else
        p->hash_value = __hash_value;

    if(keycmp)
	    p->keycmp = keycmp;
    else
        p->keycmp = __keycmp;
    
	//p->htable = kmalloc(sizeof(*(p->htable)) * size, GFP_KERNEL);
	p->htable = malloc(sizeof(*(p->htable)) * size);

    if (p->htable == NULL) 
    {
		//kfree(p);
		free(p);
		return NULL;
	}

	for (i = 0; i < size; i++)
		p->htable[i] = NULL;

	return p;
}

_api int crtl_hashtab_insert(struct crtl_hashtab_struct *h, void *key, void *datum)
{
	uint32_t hvalue;
	struct crtl_hashtab_node *prev, *cur, *newnode;

	if (!h || h->nel == crtl_hashtab_MAX_NODES)
    {
        crtl_print_err("!h || h->nel == crtl_hashtab_MAX_NODES(%x)\n", crtl_hashtab_MAX_NODES);
        return CRTL_ERROR;
    }

	hvalue = h->hash_value(h, key);
    crtl_print_debug("hash value = %d\n", hvalue);
    
	prev = NULL;
	cur = h->htable[hvalue];
	while (cur && h->keycmp(h, key, cur->key) > 0) 
    {
		prev = cur;
		cur = cur->next;
	}

	if (cur && (h->keycmp(h, key, cur->key) == 0))
    {
        crtl_print_err("cur && (h->keycmp(h, key, cur->key)(=%d) == 0)\n", h->keycmp(h, key, cur->key));
        return CRTL_ERROR;
    }
    
	//newnode = kzalloc(sizeof(*newnode), GFP_KERNEL);
	newnode = malloc(sizeof(struct crtl_hashtab_node));
	if (newnode == NULL)
    {
        crtl_print_err("null pointer error.\n");
        return CRTL_ERROR;
    }
		
	newnode->key = key;
	newnode->datum = datum;
    
	if (prev) 
    {
		newnode->next = prev->next;
		prev->next = newnode;
	} 
    else 
    {
		newnode->next = h->htable[hvalue];
		h->htable[hvalue] = newnode;
	}

	h->nel++;
	return CRTL_SUCCESS;
}

_api void *crtl_hashtab_search(struct crtl_hashtab_struct *h, const void *key)
{
	uint32_t hvalue;
	struct crtl_hashtab_node *cur;

	if (!h)
    {
        crtl_print_err("null hash table.\n");
        return NULL;
    }

	hvalue = h->hash_value(h, key);
	cur = h->htable[hvalue];
	while (cur && h->keycmp(h, key, cur->key) > 0)
		cur = cur->next;

	if (cur == NULL || (h->keycmp(h, key, cur->key) != 0))
		return NULL;

	return cur->datum;
}

_api void crtl_hashtab_destroy(struct crtl_hashtab_struct *h)
{
	uint32_t i;
	struct crtl_hashtab_node *cur, *temp;

	if (!h)
    {
        crtl_print_err("null hash table.\n");
        return;
    }
    crtl_print_debug("\n");
	for (i = 0; i < h->size; i++) 
    {
		cur = h->htable[i];
		while (cur) 
        {
			temp = cur;
			cur = cur->next;
			//kfree(temp);
			free(temp);
		}
		h->htable[i] = NULL;
	}
    crtl_print_debug("\n");
    
	//kfree(h->htable);
	free(h->htable);
	h->htable = NULL;
    
    crtl_print_debug("\n");
    
	//kfree(h);
	free(h);
    h = NULL;

    
    if(h == NULL)crtl_print_debug("\n");
}

_api int crtl_hashtab_map(struct crtl_hashtab_struct *h, int (*apply)(void *k, void *d, void *args), void *args)
{
	uint32_t i;
	int ret;
	struct crtl_hashtab_node *cur;
    
    crtl_print_debug("\n");
	if (!h)
    {
        crtl_print_err("null hash table.\n");
        return CRTL_ERROR;
    }
	crtl_print_debug("\n");

	for (i = 0; i < h->size; i++) 
    {
		cur = h->htable[i];
		while (cur) 
        {
			ret = apply(cur->key, cur->datum, args);
			if (ret != CRTL_SUCCESS)
            {
                crtl_print_err("apply return error(%d)\n", ret);
                return ret;
            }
				
			cur = cur->next;
		}
	}
	return CRTL_SUCCESS;
}


_api void crtl_hashtab_stat(struct crtl_hashtab_struct *h, struct crtl_hashtab_info *info)
{
    if (!h)
    {
        crtl_print_err("null hash table.\n");
        return;
    }
    if (!info)
    {
        crtl_print_err("null info table.\n");
        return;
    }
    
	uint32_t i, chain_len, slots_used, max_chain_len;
	struct crtl_hashtab_node *cur;

	slots_used = 0;
	max_chain_len = 0;
	for (slots_used = max_chain_len = i = 0; i < h->size; i++) 
    {
		cur = h->htable[i];
		if (cur) 
        {
			slots_used++;
			chain_len = 0;
			while (cur) 
            {
				chain_len++;
				cur = cur->next;
			}

			if (chain_len > max_chain_len)
				max_chain_len = chain_len;
		}
	}

	info->slots_used = slots_used;
	info->max_chain_len = max_chain_len;
}

_hidden uint32_t __hash_value(struct crtl_hashtab_struct *h, const void *key)
{
    const char *p, *keyp;
	unsigned int size;
	unsigned int val;

	val = 0;
	keyp = key;
	size = strlen(keyp);
    //crtl_print_debug("size = %d\n",size);
	for (p = keyp; (p - keyp) < size; p++)
		val = (val << 4 | (val >> (8*sizeof(unsigned int)-4))) ^ (*p);
	return val & (h->size - 1);
}

_hidden int __keycmp(struct crtl_hashtab_struct *h, const void *key1, const void *key2)
{
    const char *keyp1, *keyp2;

	keyp1 = key1;
	keyp2 = key2;
	return strcmp(keyp1, keyp2);
}
