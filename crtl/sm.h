#ifndef __CRTL_BITS_SM_H
#define __CRTL_BITS_SM_H 1


/**
 * State machine state type
 */
typedef unsigned long crtl_sm_state_t;

/**
 * State machine event type
 */
typedef signed long crtl_sm_event_t;

/**
 * State machine event handler function type
 */
typedef int (*crtl_sm_handler_fn_t)(const crtl_sm_state_t curr_state, \
                                    const crtl_sm_event_t event, \
                                    const crtl_sm_state_t next_state, \
                                    int argc, char *argv[]);

/* 状态机 类型 */
/**
 * State machine type
 */
typedef unsigned long crtl_sm_t;



/* 初始化状态机 */
/**
 * State machine Init
 * @param init_state: initial state
 * @return State machine type if success, 0 if failed
 */
crtl_sm_t crtl_sm_init(crtl_sm_state_t init_state);

/**
 * Register a state into State machine
 * @param sm: State machine, created by crtl_sm_init
 * @param state: state of State machine
 * @return error number
 */
int crtl_sm_state_register(crtl_sm_t sm, crtl_sm_state_t state);


/**
 * Register a handler into State machine
 * Note: You must register the "curr_state" and "next_state" state first,
 *       Then you can register a handler.
 *
 * @param sm: State machine, created by crtl_sm_init
 * @param curr_state: current state of State machine
 * @param event: event of State machine
 * @param next_state: after handler this event, state of State machine
 * @param handler: event handler  function
 * @param argc: argument count
 * @param argv: argument values
 * @return error number
 */
int crtl_sm_handler_register(crtl_sm_t sm, crtl_sm_state_t curr_state, crtl_sm_event_t event, crtl_sm_state_t next_state,
                                crtl_sm_handler_fn_t handler, int argc, char *argv[]);


/**
 * Execute State machine
 * @param sm: State machine, created by crtl_sm_init
 * @param event: event of State machine
 * @return error number
 */
int crtl_sm_exec(crtl_sm_t sm, crtl_sm_event_t event);

/**
 * State machine Destroy
 * @param sm: State machine, created by crtl_sm_init
 * @return error number
 */
int crtl_sm_destroy(crtl_sm_t sm);



#endif /*<__CRTL_BITS_SM_H>*/

