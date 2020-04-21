#ifndef __CRTL_BITS_WAIT_QUEUE_H
#define __CRTL_BITS_WAIT_QUEUE_H 1

#include "crtl/bits/crtl_list.h"
#include "crtl/bits/crtl_lock_spin.h"


typedef struct crtl_wait_queue_entry crtl_wait_queue_entry_t;

typedef int (*crtl_wait_queue_func_t)(struct crtl_wait_queue_entry *wq_entry, unsigned mode, int flags, void *key);
int crtl_default_wake_function(struct crtl_wait_queue_entry *wq_entry, unsigned mode, int flags, void *key);


/* wait_queue_entry::flags */
#define CRTL_WQ_FLAG_EXCLUSIVE	0x01
#define CRTL_WQ_FLAG_WOKEN		0x02
#define CRTL_WQ_FLAG_BOOKMARK	0x04

/*
 * A single wait-queue entry structure:
 */
struct crtl_wait_queue_entry {
	unsigned int		flags;
	void			*private;
	crtl_wait_queue_func_t	func;
	struct list_head	entry;
};

struct crtl_wait_queue_head {
	crtl_lock_spin_t		lock;
	struct list_head	head;
};
typedef struct crtl_wait_queue_head crtl_wait_queue_head_t;



//TODO:  

#endif /*<__CRTL_BITS_WAIT_QUEUE_H>*/
