
#include "crtl/task.h"
#include "crtl/log.h"
#include "crtl/assert.h"
#include "crtl/types.h"


#define THREAD 32
#define QUEUE  256
#define SIZE   8192
#define QUEUES 64


////////////////////////////////////////////////////////////////////////////demo1

int tasks = 0, done = 0;
static pthread_mutex_t lock;

void dummy_task(void *arg) {
    usleep(10000);
    pthread_mutex_lock(&lock);
    done++;
    pthread_mutex_unlock(&lock);
}

static void _unused demo1()
{
    crtl_threadpool_t *pool;

    pthread_mutex_init(&lock, NULL);

    crtl_assert((pool = crtl_threadpool_create(THREAD, QUEUE, 0)) != NULL);
    crtl_print_err("Pool started with %d threads and queue size of %d\n", THREAD, QUEUE);

    while(crtl_threadpool_add(pool, &dummy_task, NULL, 0) == 0) {
        pthread_mutex_lock(&lock);
        tasks++;
        pthread_mutex_unlock(&lock);
    }

    crtl_print_err("Added %d tasks\n", tasks);

    while((tasks / 2) > done) {
        usleep(10000);
    }
    crtl_assert(crtl_threadpool_destroy(pool, 0) == 0);
    crtl_print_err("Did %d tasks\n", done);

}

void _unused demo2_task1(void *arg)
{
    crtl_print_err("123: threadid=%ld\r\n", (unsigned long)pthread_self());

}

static void _unused demo2()
{
    crtl_threadpool_t *pool = crtl_threadpool_create(THREAD, QUEUE, 0);
    crtl_threadpool_add(pool, &demo2_task1, NULL, 0);
    crtl_threadpool_add(pool, &demo2_task1, NULL, 0);
    
    
    while(1) {
        sleep(1);
        crtl_threadpool_add(pool, &demo2_task1, NULL, 0);
    }
    crtl_threadpool_destroy(pool, 0);
}

static int threadtest()
{
    //demo1();
    demo2();
    
    

    return 0;
}

////////////////////////////////////////////////////////////////////////////demo2
crtl_threadpool_t *pool;
int left2;
pthread_mutex_t lock2;

int error2;

void dummy_task2(void *arg) {
    usleep(100);
    pthread_mutex_lock(&lock2);
    left2--;
    pthread_mutex_unlock(&lock2);
}
static int shutdown()
{
    int i;

    pthread_mutex_init(&lock2, NULL);

    /* Testing immediate shutdown */
    left2 = SIZE;
    pool = crtl_threadpool_create(THREAD, SIZE, 0);
    for(i = 0; i < SIZE; i++) {
        crtl_assert(crtl_threadpool_add(pool, &dummy_task2, NULL, 0) == 0);
    }
    crtl_assert(crtl_threadpool_destroy(pool, 0) == 0);
    crtl_assert(left2 > 0);

    /* Testing graceful shutdown */
    left2 = SIZE;
    pool = crtl_threadpool_create(THREAD, SIZE, 0);
    for(i = 0; i < SIZE; i++) {
        crtl_assert(crtl_threadpool_add(pool, &dummy_task2, NULL, 0) == 0);
    }
    crtl_assert(crtl_threadpool_destroy(pool, crtl_threadpool_graceful) == 0);
    crtl_assert(left2 == 0);

    pthread_mutex_destroy(&lock2);

    return 0;
}


////////////////////////////////////////////////////////////////////////////demo3

crtl_threadpool_t *pool3[QUEUES];
int tasks3[SIZE], left3;
pthread_mutex_t lock3;

int error3;

void dummy_task3(void *arg) {
    int *pi = (int *)arg;
    *pi += 1;

    if(*pi < QUEUES) {
        crtl_assert(crtl_threadpool_add(pool3[*pi], &dummy_task3, arg, 0) == 0);
    } else {
        pthread_mutex_lock(&lock3);
        left3--;
        pthread_mutex_unlock(&lock3);
    }
}

static int heavy()
{
    int i, copy = 1;

    left3 = SIZE;
    pthread_mutex_init(&lock3, NULL);

    for(i = 0; i < QUEUES; i++) {
        pool3[i] = crtl_threadpool_create(THREAD, SIZE, 0);
        crtl_assert(pool3[i] != NULL);
    }

    usleep(10);

    for(i = 0; i < SIZE; i++) {
        tasks3[i] = 0;
        crtl_assert(crtl_threadpool_add(pool3[0], &dummy_task3, &(tasks3[i]), 0) == 0);
    }

    while(copy > 0) {
        usleep(10);
        pthread_mutex_lock(&lock3);
        copy = left3;
        pthread_mutex_unlock(&lock3);
    }

    for(i = 0; i < QUEUES; i++) {
        crtl_assert(crtl_threadpool_destroy(pool3[i], 0) == 0);
    }

    pthread_mutex_destroy(&lock3);

    return 0;
}


int rt_thread_pool_test()
{
    threadtest();

//    shutdown();

//    heavy();

    return 0;
}



int main()
{
    rt_thread_pool_test();
    
    while(1)
    {
        sleep(1);
    }

    return 0;
}



