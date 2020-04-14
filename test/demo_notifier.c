#include "crtl/bits/crtl_notifier.h"
#include "crtl/crtl_log.h"


#define DEFUN_Name(name) notifier_test1##name
#define DEFUN_NOTIFIER_CALLBACK(name) \
    int DEFUN_Name(name)(struct crtl_notifier_block *nb, unsigned long action, void *data) {\
        crtl_print_info("---action %d, %s, prio %d\n", action, #name, nb->priority); \
        return 0;\
    }
    
DEFUN_NOTIFIER_CALLBACK(1);
DEFUN_NOTIFIER_CALLBACK(2);
DEFUN_NOTIFIER_CALLBACK(3);
DEFUN_NOTIFIER_CALLBACK(4);
DEFUN_NOTIFIER_CALLBACK(5);


void demo_notifier_test1()
{
    crtl_notifier_t chain;
    crtl_notifier_chain_init(&chain);

    crtl_notifier_register(&chain, &DEFUN_Name(1), 6);
    crtl_notifier_register(&chain, &DEFUN_Name(2), 9);
    crtl_notifier_register(&chain, &DEFUN_Name(3), 3);
    crtl_notifier_register(&chain, &DEFUN_Name(4), 4);
    crtl_notifier_register(&chain, &DEFUN_Name(5), 1);


    crtl_notifier_call_chain(&chain, 1, NULL);
    
    crtl_notifier_unregister(&chain, &DEFUN_Name(1));
    crtl_notifier_unregister(&chain, &DEFUN_Name(3));

    crtl_notifier_call_chain(&chain, 1, NULL);

    crtl_notifier_chain_destroy(&chain);
}

int main()
{
    demo_notifier_test1();
    


    return 0;
}

