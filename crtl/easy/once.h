#ifndef __CRTL_EASY_ONCE_H
#define __CRTL_EASY_ONCE_H 1

#include <pthread.h>
#include "crtl/easy/attribute.h"

/*
 * Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#define DEFINE_ONCE_FUN(init)                   \
    static int init(void);                     \
    int init##_crtl_ret_ = 0;                   \
    void init##_crtl_(void)                     \
    {                                           \
        init##_crtl_ret_ = init();              \
    }                                           \
    static int init(void)
#define DECLARE_ONCE_FUN(init)                  \
    extern int init##_crtl_ret_;                \
    void init##_crtl_(void);

#define DEFINE_ONCE_FUN_STATIC(init)            \
    static int init(void);                     \
    static int init##_crtl_ret_ = 0;            \
    static void init##_crtl_(void)              \
    {                                           \
        init##_crtl_ret_ = init();              \
    }                                           \
    static int init(void)

/*
 * RUN_ONCE - use CRYPTO_THREAD_run_once, and check if the init succeeded
 * @once: pointer to static object of type CRYPTO_ONCE
 * @init: function name that was previously given to DEFINE_RUN_ONCE,
 *        DEFINE_RUN_ONCE_STATIC or DECLARE_RUN_ONCE.  This function
 *        must return 1 for success or 0 for failure.
 *
 * The return value is 1 on success (*) or 0 in case of error.
 *
 * (*) by convention, since the init function must return 1 on success.
 */
#define ONCE_RUN(once, init) \
_choose_expr(_types_compatible_p(typeof(once), unsigned int*), \
                __crtl_unsigendint_run_once(once, init##_crtl_), \
                _choose_expr(_types_compatible_p(typeof(once), pthread_once_t*), \
                                __crtl_pthread_run_once(once, init##_crtl_), \
                                __crtl_unsigendlong_run_once((unsigned long *)once, init##_crtl_))\
            ) 


#define _choose_expr(exp, e1, e2) __builtin_choose_expr(exp, e1, e2)
#define _types_compatible_p(a, b) __builtin_types_compatible_p(a, b)

static int _unused __crtl_unsigendint_run_once(unsigned int *once, void (*init)(void))
{
    if (*once != 0)
        return 0;

    init();
    *once = 1;

    return 1;
}

static int _unused __crtl_unsigendlong_run_once(unsigned long *once, void (*init)(void))
{
    if (*once != 0)
        return 0;

    init();
    *once = 1;

    return 1;
}

static int _unused __crtl_pthread_run_once(pthread_once_t *once, void (*init)(void))
{
    if (pthread_once(once, init) != 0)
        return 0;

    return 1;
}



#endif //__CRTL_EASY_ONCE_H

