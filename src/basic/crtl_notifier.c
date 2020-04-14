#include "crtl/bits/crtl_notifier.h"
#include "crtl/crtl_assert.h"
#include "crtl/crtl_log.h"
#include "crtl/easy/macro.h"
#include "crtl/easy/attribute.h"


_api void crtl_notifier_chain_init(crtl_notifier_t *chain_head)
{
    INIT_LIST_HEAD(chain_head);
}

_api void crtl_notifier_register(crtl_notifier_t *chain, crtl_notifier_fn_t fn, int priority)
{
    if(unlikely(!chain) || unlikely(!fn)) {
        crtl_assert(0);
        return;
    }
    
    struct crtl_notifier_block *new_blk = malloc(sizeof(struct crtl_notifier_block));
    crtl_assert(new_blk);
    
    new_blk->list.next = NULL;
    new_blk->list.prev = NULL;
    new_blk->notifier_call = fn;
    new_blk->priority = priority;
    
    if(list_empty(chain)) {
        list_add(&new_blk->list, chain);
    } else {
        struct crtl_notifier_block *iter_next, *iter;
        list_for_each_entry_safe(iter,iter_next, chain, list) {
            
            if(iter->priority <= new_blk->priority) {
                if(list_is_last(&iter->list, chain)) {
                    list_add(&new_blk->list, &iter->list);
                    break;
                } else {
                    if(iter_next->priority >= new_blk->priority) {
                        list_add(&new_blk->list, &iter->list);
                        break;
                    }
                }
                
            } else {
            
                list_add(&new_blk->list, chain);
                break;
            }
        }
    }
    
}



_api void crtl_notifier_unregister(crtl_notifier_t *chain, crtl_notifier_fn_t fn)
{
    if(unlikely(!chain) || unlikely(!fn)) {
        crtl_assert(0);
        return;
    }
    int find = 0;
    struct crtl_notifier_block *iter_next, *iter;
    
    if(list_empty(chain)) {
        return;
    } else {
        
        list_for_each_entry_safe(iter,iter_next, chain, list) {
            if(iter->notifier_call == fn) {
                find = 1;
                break;
            }
        }
    }
    if(find) {
        list_del_init(&iter->list);
        free(iter);
    }
}


_api void crtl_notifier_call_chain(crtl_notifier_t *chain, unsigned long action, void *data)
{
    struct crtl_notifier_block *iter_next, *iter;
    list_for_each_entry_safe_reverse(iter,iter_next, chain, list) {
        if(iter->notifier_call) {
            iter->notifier_call(iter, action, data);
        }
    }
}


_api void crtl_notifier_chain_destroy(crtl_notifier_t *chain)
{
    if(unlikely(!chain)) {
        crtl_assert(0);
        return;
    }
    struct crtl_notifier_block *iter_next, *iter;
    
    if(list_empty(chain)) {
        return;
    } else {
        
        list_for_each_entry_safe(iter,iter_next, chain, list) {
            free(iter);
        }
    }

}


