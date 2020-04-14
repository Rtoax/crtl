#ifndef __CRTL_BITS_NOTIFIER_H
#define __CRTL_BITS_NOTIFIER_H 1


#include "crtl/bits/crtl_list.h"

typedef struct list_head crtl_notifier_t;

struct crtl_notifier_block;


typedef	int (*crtl_notifier_fn_t)(struct crtl_notifier_block *nb, 
                                    unsigned long action, void *data);


struct crtl_notifier_block {
	crtl_notifier_fn_t notifier_call;
	int priority; 
    struct list_head list;
};


void crtl_notifier_chain_init(crtl_notifier_t *chain_head);
void crtl_notifier_register(crtl_notifier_t *chain, crtl_notifier_fn_t fn, int priority);
void crtl_notifier_unregister(crtl_notifier_t *chain, crtl_notifier_fn_t fn);
void crtl_notifier_call_chain(crtl_notifier_t *chain, unsigned long action, void *data);
void crtl_notifier_chain_destroy(crtl_notifier_t *chain);




#endif /*<__CRTL_BITS_NOTIFIER_H>*/
