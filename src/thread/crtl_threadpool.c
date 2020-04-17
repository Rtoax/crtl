
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <stdlib.h>

#include "crtl/bits/crtl_threadpool.h"

/*
 * Copyright (c) 2016, Mathias Brossard <mathias@brossard.org>.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file threadpool.c
 * @brief Threadpool implementation file
 */





typedef enum {
    immediate_shutdown = 1,
    graceful_shutdown  = 2
} crtl_threadpool_shutdown_t;

/**
 *  @struct crtl_threadpool_task
 *  @brief the work struct
 *
 *  @var function Pointer to the function that will perform the task.
 *  @var argument Argument to be passed to the function.
 */

typedef struct {
    void (*function)(void *);
    void *argument;
} crtl_threadpool_task_t;

/**
 *  @struct threadpool
 *  @brief The threadpool struct
 *
 *  @var notify       Condition variable to notify worker threads.
 *  @var threads      Array containing worker threads ID.
 *  @var thread_count Number of threads
 *  @var queue        Array containing the task queue.
 *  @var queue_size   Size of the task queue.
 *  @var head         Index of the first element.
 *  @var tail         Index of the next element.
 *  @var count        Number of pending tasks
 *  @var shutdown     Flag indicating if the pool is shutting down
 *  @var started      Number of started threads
 */
struct crtl_threadpool_t {
#define THREADPOOL_NAME_LEN 20
    char name[THREADPOOL_NAME_LEN];
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t *threads;
    crtl_threadpool_task_t *queue;
    int thread_count;
    int queue_size;
    int head;
    int tail;
    int count;
    int shutdown;
    int started;
};



/**
 * @function void *__crtl_threadpool_thread(void *threadpool)
 * @brief the worker thread
 * @param threadpool the pool which own the thread
 */
static void *__crtl_threadpool_thread(void *threadpool);

static int __crtl_threadpool_free(crtl_threadpool_t *pool);

/* create thread pool */
/**
 * @function crtl_threadpool_create
 * @brief Creates a crtl_threadpool_t object.
 * @param thread_count Number of worker threads.
 * @param queue_size   Size of the queue.
 * @param flags        Unused parameter.
 * @return a newly created thread pool or NULL
 */
crtl_threadpool_t *crtl_threadpool_create(int thread_count, int queue_size, int flags)
{  

    crtl_threadpool_t *pool;
    int i;
    (void) flags;

    /* 参数有效性 */
    if(thread_count <= 0 || thread_count > MAX_THREADS || queue_size <= 0 || queue_size > MAX_QUEUE) 
    {
        crtl_print_err("out of range error.\n");
        return NULL;
    }

    /* 初始化内存 */
    if((pool = (crtl_threadpool_t *)malloc(sizeof(crtl_threadpool_t))) == NULL) 
    {
        crtl_print_err("malloc error.\n");
        goto err;
    }

    /* Initialize */
    pool->thread_count = 0;
    pool->queue_size = queue_size;
    pool->head = pool->tail = pool->count = 0;
    pool->shutdown = pool->started = 0;

    /* Allocate thread and task queue *//* 线程ID数组内存申请 */
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    pool->queue = (crtl_threadpool_task_t *)malloc(sizeof(crtl_threadpool_task_t) * queue_size);

    /* Initialize mutex and conditional variable first */
    if((pthread_mutex_init(&(pool->lock), NULL) != 0) ||/* 初始化一个互斥锁 */
       (pthread_cond_init(&(pool->notify), NULL) != 0) ||/* 初始化一个条件变量  */
       (pool->threads == NULL) ||
       (pool->queue == NULL)) 
    {
        crtl_print_err("init thread pool error.\n");
        goto err;
    }

    /* Start worker threads */
    for(i = 0; i < thread_count; i++) {
        /* 创建线程，并将线程置于等待状态 */
        if(pthread_create(&(pool->threads[i]), NULL, __crtl_threadpool_thread, (void*)pool) != 0) {
            /* 如果创建失败，则销毁 */
            crtl_print_err("ptread_create error.\n");
            crtl_threadpool_destroy(pool, 0);/* 失败，销毁这个线程池 */
            return NULL;
        }
        pool->thread_count++;/* 成功，线程计数+1 */
        pool->started++;/* 起始值+1 */
    }
    return pool;

 err:
    crtl_print_err("some happend during init threadpool.\n");
    
    if(pool) {
        __crtl_threadpool_free(pool);
    }
    return NULL;
}

/* 添加任务 */
/**
 * @function crtl_threadpool_add
 * @brief add a new task in the queue of a thread pool
 * @param pool     Thread pool to which add the task.
 * @param function Pointer to the function that will perform the task.
 * @param argument Argument to be passed to the function.
 * @param flags    Unused parameter.
 * @return 0 if all goes well, negative values in case of error (@see
 * crtl_threadpool_error_t for codes).
 */
int crtl_threadpool_add(crtl_threadpool_t *pool, void (*function)(void *), void *argument, int flags)
{
    int err = 0;
    int next;
    (void) flags;

    if(pool == NULL || function == NULL) 
    {
        crtl_print_err("null pointer error.\n");
        return crtl_threadpool_invalid;
    }

    if(pthread_mutex_lock(&(pool->lock)) != 0) /* 锁定线程池  */
    {
        crtl_print_err("mutex lock error.\n");
        return crtl_threadpool_lock_failure;
    }

    next = (pool->tail + 1) % pool->queue_size;/* 查找任务要添加到线程池的位置 */

    do {
        /* Are we full ? */
        if(pool->count == pool->queue_size) /* 线程池已满 */
        {
            crtl_print_err("this thread pool is full.\n");
            err = crtl_threadpool_queue_full;
            break;
        }

        /* Are we shutting down ? */
        if(pool->shutdown) /* 是否为关闭状态 */
        {
            crtl_print_err("this thread pool is shutdown.\n");
            err = crtl_threadpool_shutdown;
            break;
        }

        /* Add task to queue */
        pool->queue[pool->tail].function = function;
        pool->queue[pool->tail].argument = argument;
        pool->tail = next;
        pool->count += 1;

        /* pthread_cond_broadcast: 向一个任务发送条件信号，执行任务 */
        if(pthread_cond_signal(&(pool->notify)) != 0)  /* 执行任务 */
        {
            crtl_print_err("pthread_cond_signal error.\n");
            err = crtl_threadpool_lock_failure;
            break;
        }
    } while(0);

    if(pthread_mutex_unlock(&pool->lock) != 0) 
    {
        crtl_print_err("mutex unlock error.\n");
        err = crtl_threadpool_lock_failure;
    }

    return err;
}

/* 销毁线程池 */
/**
 * @function crtl_threadpool_destroy
 * @brief Stops and destroys a thread pool.
 * @param pool  Thread pool to destroy.
 * @param flags Flags for shutdown
 *
 * Known values for flags are 0 (default) and crtl_threadpool_graceful in
 * which case the thread pool doesn't accept any new tasks but
 * processes all pending tasks before shutdown.
 */
int crtl_threadpool_destroy(crtl_threadpool_t *pool, int flags)
{
    int i, err = 0;

    if(pool == NULL) 
    {
        crtl_print_err("null pointer error.\n");
        return crtl_threadpool_invalid;
    }

    if(pthread_mutex_lock(&(pool->lock)) != 0) 
    {
        crtl_print_err("mutex lock error.\n");
        return crtl_threadpool_lock_failure;
    }

    do {
        /* Already shutting down */
        if(pool->shutdown) {
            err = crtl_threadpool_shutdown;
            break;
        }

        pool->shutdown = (flags & crtl_threadpool_graceful) ?
            graceful_shutdown : immediate_shutdown;

        /* Wake up all worker threads */
        if((pthread_cond_broadcast(&(pool->notify)) != 0) ||
           (pthread_mutex_unlock(&(pool->lock)) != 0)) 
        {
            crtl_print_err("pthread_cond_broadcast|pthread_mutex_unlock error.\n");
            err = crtl_threadpool_lock_failure;
            break;
        }

        /* Join all worker thread */
        for(i = 0; i < pool->thread_count; i++) 
        {
            if(pthread_join(pool->threads[i], NULL) != 0) 
            {
                crtl_print_err("pthread_join error.\n");
                err = crtl_threadpool_thread_failure;
            }
        }
    } while(0);

    /* Only if everything went well do we deallocate the pool */
    if(!err) 
    {
        __crtl_threadpool_free(pool);
    }
    return err;
}

/* 释放线程池资源 */
static int __crtl_threadpool_free(crtl_threadpool_t *pool)
{
    if(pool == NULL || pool->started > 0) {
        return -1;
    }

    /* Did we manage to allocate ? */
    if(pool->threads) {
        free(pool->threads);
        free(pool->queue);
 
        /* Because we allocate pool->threads after initializing the
           mutex and condition variable, we're sure they're
           initialized. Let's lock the mutex just in case. */
        pthread_mutex_lock(&(pool->lock));
        pthread_mutex_destroy(&(pool->lock));
        pthread_cond_destroy(&(pool->notify));
    }
    free(pool);    
    return 0;
}

/* 线程池线程的初始状态与任务分配 */
static void *__crtl_threadpool_thread(void *threadpool)
{
    crtl_threadpool_t *pool = (crtl_threadpool_t *)threadpool;
    crtl_threadpool_task_t task;

    for(;;) 
    {
        /* Lock must be taken to wait on conditional variable */
        pthread_mutex_lock(&(pool->lock));

        /* Wait on condition variable, check for spurious wakeups.
           When returning from pthread_cond_wait(), we own the lock. */
        while((pool->count == 0) && (!pool->shutdown)) 
        {
            /* waiting for pthread_cond_signal/pthread_cond_broadcast */
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        if((pool->shutdown == immediate_shutdown) ||
           ((pool->shutdown == graceful_shutdown) && (pool->count == 0))) 
        {
            break;
        }

        /* Grab our task */
        task.function = pool->queue[pool->head].function;
        task.argument = pool->queue[pool->head].argument;
        pool->head = (pool->head + 1) % pool->queue_size;
        pool->count -= 1;
        
        /* Unlock */
        pthread_mutex_unlock(&(pool->lock));

        /* Get to work */
        (*(task.function))(task.argument);
    }

    pool->started--;

    pthread_mutex_unlock(&(pool->lock));
    pthread_exit(NULL);
    return(NULL);
}

