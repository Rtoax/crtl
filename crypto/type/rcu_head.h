/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TYPES_H
#define _LINUX_TYPES_H


struct callback_head {
	struct callback_head *next;
	void (*func)(struct callback_head *head);
} __attribute__((aligned(sizeof(void *))));

/* Read-Copy Update: rcu */
#define rcu_head callback_head

typedef void (*rcu_callback_t)(struct rcu_head *head);
typedef void (*call_rcu_func_t)(struct rcu_head *head, rcu_callback_t func);

#define call_rcu(head, func) func(head)
#define rcu_assign_pointer(p, v)	do { (p) = (v); } while (0)
#define RCU_INIT_POINTER(p, v)	do { (p) = (v); } while (0)

static void rcu_read_lock(){};
static void rcu_read_unlock(){};


#endif //_LINUX_TYPES_H
