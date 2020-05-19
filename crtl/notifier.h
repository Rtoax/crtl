#ifndef __CRTL_BITS_NOTIFIER_H
#define __CRTL_BITS_NOTIFIER_H 1


/**
 * notifier chain handle
 */
typedef void* crtl_notifier_t;


/**
 * notifier chain callback type
 * @param action: notifier chain
 * @param priority: priority of callback
 * @param data: user data
 * @return user error number
 */
typedef	int (*crtl_notifier_fn_t)(unsigned long action, int priority, void *data);

/**
 * Initialized a notifier chain
 * @param chain: notifier chain
 * @return void
 */
void crtl_notifier_chain_init(crtl_notifier_t chain);

/**
 * register a callback to notifier chain
 * @param chain: notifier chain
 * @param fn: callback
 * @param priority: priority of callback
 * @return void
 */
void crtl_notifier_register(crtl_notifier_t chain, crtl_notifier_fn_t fn, int priority);

/**
 * unregister a callback from notifier chain
 * @param chain: notifier chain
 * @param fn: callback
 * @return void
 */
void crtl_notifier_unregister(crtl_notifier_t chain, crtl_notifier_fn_t fn);

/**
 * call a notifier from notifier chain
 * @param chain: notifier chain
 * @param action: action
 * @param data: user data
 * @return void
 */
void crtl_notifier_call_chain(crtl_notifier_t chain, unsigned long action, void *data);

/**
 * destroy notifier chain
 * @param chain: notifier chain
 * @return void
 */
void crtl_notifier_chain_destroy(crtl_notifier_t chain);




#endif /*<__CRTL_BITS_NOTIFIER_H>*/
