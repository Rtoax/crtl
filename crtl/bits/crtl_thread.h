#ifndef __CRTL_THREAD_H
#define __CRTL_THREAD_H 1

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sched.h>

#include "crtl/crtl_lock.h"



#define CRTL_THREAD_SCHED_PRIO_TIME_CRITICAL  90    
#define CRTL_THREAD_SCHED_PRIO_HIGHEST    80    
#define CRTL_THREAD_SCHED_PRIO_HIGHER     70    
#define CRTL_THREAD_SCHED_PRIO_HIGH   60   
#define CRTL_THREAD_SCHED_PRIO_ABOVE_NORMAL   50  
#define CRTL_THREAD_SCHED_PRIO_NORMAL     40   
#define CRTL_THREAD_SCHED_PRIO_BELOW_NORMAL    30  
#define CRTL_THREAD_SCHED_PRIO_LOWEST     10
#define CRTL_THREAD_SCHED_PRIO_IDLE   1





#define crtl_thread_normal(p_threadID, fn, arg) \
    crtl_thread_create(p_threadID, PTHREAD_CREATE_DETACHED, 0, \
                        CRTL_THREAD_SCHED_PRIO_NORMAL, SCHED_FIFO, PTHREAD_SCOPE_SYSTEM,\
                        NULL, 0, 0, fn, arg, NULL, NULL)


typedef pthread_t       crtl_thread_t;
typedef pthread_attr_t  crtl_threadattr_t;
typedef pthread_once_t  crtl_thread_once_t;
typedef pthread_key_t   crtl_thread_key_t;



#define CRTL_THREAD_ONCE_INIT   PTHREAD_ONCE_INIT


typedef void *(*crtl_thread_start_routine_fn)(void *);
typedef void (*crtl_thread_cleanup_routine_fn)(void *);







int crtl_threadattr_init(crtl_threadattr_t *attr);
int crtl_threadattr_destroy(crtl_threadattr_t *attr);

int crtl_threadattr_getdetachstate(const crtl_threadattr_t *__attr, int *__detachstate);
int crtl_threadattr_setdetachstate_detached(crtl_threadattr_t *__attr);
int crtl_threadattr_setdetachstate_joinable(crtl_threadattr_t *__attr);
int crtl_threadattr_setdetachstate(crtl_threadattr_t *__attr, int __detachstate);


int crtl_threadattr_getguardsize(const crtl_threadattr_t *__attr, size_t *__guardsize);
int crtl_threadattr_setguardsize(crtl_threadattr_t *__attr, size_t __guardsize);


int crtl_threadattr_getsched(crtl_threadattr_t *__attr, int *__sched_priority, int *__sched_policy);
int crtl_threadattr_setsched(crtl_threadattr_t *__attr, int __sched_priority/*1-99*/, int __sched_policy);
int crtl_threadattr_setsched_fifo(crtl_threadattr_t *__attr, int __sched_priority/*1-99*/);
int crtl_threadattr_setsched_rr(crtl_threadattr_t *__attr, int __sched_priority/*1-99*/);
int crtl_threadattr_setsched_other(crtl_threadattr_t *__attr);


int crtl_threadattr_getinheritsched(const crtl_threadattr_t *__attr, int *__inherit);
int crtl_threadattr_setinheritsched_inherit(crtl_threadattr_t *__attr);
int crtl_threadattr_setinheritsched_explicit(crtl_threadattr_t *__attr);
int crtl_threadattr_setinheritsched(crtl_threadattr_t *__attr, int __inherit);


int crtl_threadattr_getscope(const crtl_threadattr_t *__attr, int *__scope);
int crtl_threadattr_setscope_system(crtl_threadattr_t *__attr);
int crtl_threadattr_setscope_process(crtl_threadattr_t *__attr);
int crtl_threadattr_setscope(crtl_threadattr_t *__attr, int __scope);


int crtl_threadattr_getstack(const crtl_threadattr_t *__attr, void ** __stackaddr, size_t * __stacksize);
int crtl_threadattr_setstack(crtl_threadattr_t *__attr, void *__stackaddr, size_t __stacksize);

int crtl_threadattr_getaffinity(const crtl_threadattr_t *__attr, cpu_set_t *__cpuset);
int crtl_threadattr_setaffinity(crtl_threadattr_t *__attr, const cpu_set_t *__cpuset);


int crtl_thread_create(crtl_thread_t * __newthread, int __detachstate, size_t __guardsize, 
                         int __sched_priority, int __sched_policy, int __scope, 
                         void *__stackaddr, size_t __stacksize, int cpu_core_id,
                         crtl_thread_start_routine_fn __start_routine, void *__start_arg,
                         crtl_thread_cleanup_routine_fn __cleanup_routine, void *__clean_arg);

crtl_thread_t crtl_thread_self();
void crtl_thread_exit(int *retval);
int crtl_thread_kill(crtl_thread_t thread, int sig);
int crtl_thread_kill_self(int sig);

int crtl_thread_join(crtl_thread_t __th, void **__thread_return, int tryjoin, int timedjoin, int seconds, long nanoseconds);
int crtl_thread_getattr(crtl_thread_t thread, crtl_threadattr_t *attr);

int crtl_thread_equal(crtl_thread_t thread1, crtl_thread_t thread2);

int crtl_thread_getschedparam(crtl_thread_t thread, int *__sched_priority, int *__sched_policy);
int crtl_thread_setschedparam(crtl_thread_t thread, int __sched_priority/*1-99*/, int __sched_policy);
int crtl_thread_setschedparam_fifo(crtl_thread_t thread, int __sched_priority/*1-99*/);
int crtl_thread_setschedparam_rr(crtl_thread_t thread, int __sched_priority/*1-99*/);
int crtl_thread_setschedparam_other(crtl_thread_t thread);
int crtl_thread_setschedprio(crtl_thread_t thread, int __sched_priority/*1-99*/);

int crtl_thread_getname(crtl_thread_t thread, char *__buf, int buf_len);
int crtl_thread_setname(crtl_thread_t thread, const char *__name);

int crtl_thread_getconcurrency(void);
int crtl_thread_setconcurrency(int __level);

int crtl_thread_yield(void);


int crtl_thread_getaffinity(const crtl_thread_t thread, cpu_set_t *__cpuset);
int crtl_thread_setaffinity(crtl_thread_t thread, const cpu_set_t *__cpuset);

int crtl_thread_once(crtl_thread_once_t *__once_control, void (*__init_routine) (void));


int crtl_thread_setcancelstate(int __state, int *__oldstate);
int crtl_thread_setcancelstate_enable(int *__oldstate);
int crtl_thread_setcancelstate_disable(int *__oldstate);

int crtl_thread_setcanceltype(int __type, int *__oldtype);
int crtl_thread_setcanceltype_deferred(int *__oldtype);
int crtl_thread_setcanceltype_asynchronous(int *__oldtype);

int crtl_thread_cancel(crtl_thread_t thread);
void crtl_thread_testcancel(void);

int crtl_thread_key_create(crtl_thread_key_t *key, void (*destructor_fn)(void *));
int crtl_thread_key_delete(crtl_thread_key_t key);
void* crtl_thread_key_getspecific(crtl_thread_key_t key);
int crtl_thread_key_setspecific(crtl_thread_key_t key, const void *ptr);




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _PTHREAD_H


extern int pthread_create(pthread_t *__restrict __newthread,
			   const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg);

extern void pthread_exit(void *__retval);

//当调用 pthread_join() 时，当前线程会处于阻塞状态，直到被调用的线程结束后，当前线程才会重新开始执行。
//当 pthread_join() 函数返回后，被调用线程才算真正意义上的结束，它的内存空间也会被释放（如果被调用线程是非分离的）。
//这里有三点需要注意：
//    被释放的内存空间仅仅是系统空间，你必须手动清除程序分配的空间，比如 malloc() 分配的空间。
//    一个线程只能被一个线程所连接。
//    被连接的线程必须是非分离的，否则连接会出错。
extern int pthread_join(pthread_t __th, void **__thread_return);

#ifdef __USE_GNU
/* Check whether thread TH has terminated.  If yes return the status of
   the thread in *THREAD_RETURN, if THREAD_RETURN is not NULL.  */
extern int pthread_tryjoin_np(pthread_t __th, void **__thread_return);

/* Make calling thread wait for termination of the thread TH, but only
   until TIMEOUT.  The exit status of the thread is stored in
   *THREAD_RETURN, if THREAD_RETURN is not NULL.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int pthread_timedjoin_np(pthread_t __th, void **__thread_return,
				 const struct timespec *__abstime);
#endif
/* Obtain the identifier of the current thread.  */
extern pthread_t pthread_self(void);

int pthread_kill(pthread_t thread, int sig);

/* Compare two thread identifiers.  */
extern int pthread_equal(pthread_t __thread1, pthread_t __thread2);



#ifdef __USE_GNU

/* Initialize thread attribute *ATTR with attributes corresponding to the
   already running thread TH.  It shall be called on uninitialized ATTR
   and destroyed with pthread_attr_destroy when no longer needed.  */
extern int pthread_getattr_np (pthread_t __th, pthread_attr_t *__attr);
#endif


/* Functions for scheduling control.  */

/* Set the scheduling parameters for TARGET_THREAD according to POLICY
   and *PARAM.  */
extern int pthread_setschedparam(pthread_t __target_thread, int __policy,
				  const struct sched_param *__param);

/* Return in *POLICY and *PARAM the scheduling parameters for TARGET_THREAD. */
extern int pthread_getschedparam(pthread_t __target_thread,
				  int *__restrict __policy,
				  struct sched_param *__restrict __param);

/* Set the scheduling priority for TARGET_THREAD.  */
extern int pthread_setschedprio(pthread_t __target_thread, int __prio);


#ifdef __USE_GNU
/* Get thread name visible in the kernel and its interfaces.  */
extern int pthread_getname_np (pthread_t __target_thread, char *__buf,
			       size_t __buflen);

/* Set thread name visible in the kernel and its interfaces.  */
extern int pthread_setname_np (pthread_t __target_thread, const char *__name);
#endif


#ifdef __USE_UNIX98
/* Determine level of concurrency.  */
extern int pthread_getconcurrency (void);

/* Set new concurrency level to LEVEL.  */
extern int pthread_setconcurrency (int __level);
#endif

#ifdef __USE_GNU
/* Yield the processor to another thread or process.
   This function is similar to the POSIX `sched_yield' function but
   might be differently implemented in the case of a m-on-n thread
   implementation.  */
extern int pthread_yield (void);//线程让出CPU


/* Limit specified thread TH to run only on the processors represented
   in CPUSET.  */
extern int pthread_setaffinity_np (pthread_t __th, size_t __cpusetsize,
				   const cpu_set_t *__cpuset);

/* Get bit set in CPUSET representing the processors TH can run on.  */
extern int pthread_getaffinity_np (pthread_t __th, size_t __cpusetsize,
				   cpu_set_t *__cpuset);
#endif


/* Functions for handling initialization.  */

/* Guarantee that the initialization function INIT_ROUTINE will be called
   only once, even if pthread_once is executed several times with the
   same ONCE_CONTROL argument. ONCE_CONTROL must point to a static or
   extern variable initialized to PTHREAD_ONCE_INIT.

   The initialization functions might throw exception which is why
   this function is not marked with __THROW.  */
extern int pthread_once (pthread_once_t *__once_control,
			 void (*__init_routine) (void));


/* Functions for handling cancellation.

   Note that these functions are explicitly not marked to not throw an
   exception in C++ code.  If cancellation is implemented by unwinding
   this is necessary to have the compiler generate the unwind information.  */

/* Set cancelability state of current thread to STATE, returning old
   state in *OLDSTATE if OLDSTATE is not NULL.  */
//设置本线程对Cancel信号的反应，state有两种值：PTHREAD_CANCEL_ENABLE（缺省）和PTHREAD_CANCEL_DISABLE，
//分别表示收到信号后设为CANCLED状态和忽略CANCEL信号继续运行；old_state如果不为NULL则存入原来的Cancel状态以便恢复。
extern int pthread_setcancelstate (int __state, int *__oldstate);

/* Set cancellation state of current thread to TYPE, returning the old
   type in *OLDTYPE if OLDTYPE is not NULL.  */
//设置本线程取消动作的执行时机，type由两种取值：PTHREAD_CANCEL_DEFFERED和PTHREAD_CANCEL_ASYCHRONOUS，
//仅当Cancel状态为Enable时有效，分别表示收到信号后继续运行至下一个取消点再退出和立即执行取消动作（退出）；
//oldtype如果不为NULL则存入运来的取消动作类型值。
extern int pthread_setcanceltype (int __type, int *__oldtype);

/* Cancel THREAD immediately or at the next possibility.  */
//发送终止信号给thread线程，如果成功则返回0，否则为非0值。发送成功并不意味着thread会终止
extern int pthread_cancel (pthread_t __th);

/* Test for pending cancellation for the current thread and terminate
   the thread as per pthread_exit(PTHREAD_CANCELED) if it has been
   cancelled.  */
//是说pthread_testcancel在不包含取消点，但是又需要取消点的地方创建一个取消点，
//以便在一个没有包含取消点的执行代码线程中响应取消请求.
//线程取消功能处于启用状态且取消状态设置为延迟状态时，pthread_testcancel()函数有效。
//如果在取消功能处处于禁用状态下调用pthread_testcancel()，则该函数不起作用。
//请务必仅在线程取消线程操作安全的序列中插入pthread_testcancel()。
//除通过pthread_testcancel()调用以编程方式建立的取消点意外，pthread标准还指定了几个取消点。
//测试退出点,就是测试cancel信号.
extern void pthread_testcancel (void);

#ifdef ____TEST_pthread_cancel_wocainimeidingyiba

#include<stdio.h>  
#include<stdlib.h>  
#include <pthread.h>  
void *thread_fun(void *arg)  
{  
    int i=1;  
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    /*同步取消，等到下一个取消点再取消*/
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);   

    printf("thread start \n");  
    while(1)  
    {  
        i++;  
        /*手动建立一个取消点, 防止线程中无取消点，导致线程不取消。*/
        pthread_testcancel();  
    }  
    return (void *)0;  
}  
int main()  
{  
    void *ret=NULL;  
    int iret=0;  
    pthread_t tid;  
    pthread_create(&tid,NULL,thread_fun,NULL);  
    sleep(1);  

    pthread_cancel(tid);//取消线程  
    pthread_join(tid, &ret);  
    printf("thread 3 exit code %d\n", (int)ret);  

    return 0;  

}  
/////////////////////////////////////////////
#include<stdio.h>  
#include<stdlib.h>  
#include <pthread.h>  
void *thread_fun(void *arg)  
{  
    int i=1;  
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    /*同步取消，等到下一个取消点再取消*/
//      pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    /*异步取消， 线程接到取消信号后，立即退出*/
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    printf("thread start \n");  

    while(1)  
    {  
        i++;  
    }  
    return (void *)0;  
}  
int main()  
{  
    void *ret=NULL;  
    int iret=0;  
    pthread_t tid;  
    pthread_create(&tid,NULL,thread_fun,NULL);  
    sleep(1);  

    pthread_cancel(tid);//取消线程  
    pthread_join(tid, &ret);  
    printf("thread 3 exit code %d\n", (int)ret);  

    return 0;  

}  

#endif//____TEST_pthread_cancel_wocainimeidingyiba


/* Thread attribute handling.  */

/* Initialize thread attribute *ATTR with default attributes
   (detachstate is PTHREAD_JOINABLE, scheduling policy is SCHED_OTHER,
    no user-provided stack).  */
extern int pthread_attr_init (pthread_attr_t *__attr);

/* Destroy thread attribute *ATTR.  */
extern int pthread_attr_destroy (pthread_attr_t *__attr);

//在任何一个时间点上，线程是可结合的（joinable），或者是分离的（detached）。
//一个可结合的线程能够被其他线程收回其资源和杀死；在被其他线程回收之前，
//它的存储器资源（如栈）是不释放的。相反，一个分离的线程是不能被其他线程回收或杀死的，
//它的存储器资源在它终止时由系统自动释放。
//
//线程的分离状态决定一个线程以什么样的方式来终止自己。
//在默认情况下线程是非分离状态的，这种情况下，原有的线程等待创建的线程结束。
//只有当pthread_join（）函数返回时，创建的线程才算终止，才能释放自己占用的系统资源。
//而分离线程不是这样子的，它没有被其他的线程所等待，自己运行结束了，线程也就终止了，马上释放系统资源。
//程序员应该根据自己的需要，选择适当的分离状态。所以如果我们在创建线程时就知道不需要了解线程的终止状态，
//则可以pthread_attr_t结构中的detachstate线程属性，让线程以分离状态启动。
//
//设置线程分离状态的函数为pthread_attr_setdetachstate（pthread_attr_t *attr, int detachstate）。
//第二个参数可选为PTHREAD_CREATE_DETACHED（分离线程）和 PTHREAD _CREATE_JOINABLE（非分离线程）。
//这里要注意的一点是，如果设置一个线程为分离线程，而这个线程运行又非常快，
//它很可能在pthread_create函数返回之前就终止了，它终止以后就可能将线程号和系统资源移交给其他的线程使用，
//这样调用pthread_create的线程就得到了错误的线程号。要避免这种情况可以采取一定的同步措施，
//最简单的方法之一是可以在被创建的线程里调用pthread_cond_timewait函数，让这个线程等待一会儿，
//留出足够的时间让函数pthread_create返回。设置一段等待时间，是在多线程编程里常用的方法。
//但是注意不要使用诸如wait（）之类的函数，它们是使整个进程睡眠，并不能解决线程同步的问题。
//
//另外一个可能常用的属性是线程的优先级，它存放在结构sched_param中。
//用函数pthread_attr_getschedparam和函数pthread_attr_setschedparam进行存放，
//一般说来，我们总是先取优先级，对取得的值修改后再存放回去。
/* Get detach state attribute.  */
extern int pthread_attr_getdetachstate(const pthread_attr_t *__attr,
					int *__detachstate);
/* Set detach state attribute.  */
extern int pthread_attr_setdetachstate(pthread_attr_t *__attr,
					int __detachstate);

//获取线程的栈保护区大小
/* Get the size of the guard area created for stack overflow protection.  */
extern int pthread_attr_getguardsize(const pthread_attr_t *__attr,
				      size_t *__guardsize);

/* Set the size of the guard area created for stack overflow protection.  */
extern int pthread_attr_setguardsize(pthread_attr_t *__attr,
				      size_t __guardsize);

//SCHED_FIFO和SCHED_RR政策，唯一需要的参数成员sched_priority。
//struct sched_param {
//    int sched_priority;     /* Scheduling priority */
//};
//PTHREAD_EXPLICIT_SCHED
//PTHREAD_INHERIT_SCHED
/* Return in *PARAM the scheduling parameters of *ATTR.  */
extern int pthread_attr_getschedparam(const pthread_attr_t *__restrict __attr,
				       struct sched_param *__restrict __param);

/* Set scheduling parameters (priority, etc) in *ATTR according to PARAM.  */
extern int pthread_attr_setschedparam(pthread_attr_t *__restrict __attr,
				       const struct sched_param *__restrict
				       __param);

/* Return in *POLICY the scheduling policy of *ATTR.  */
extern int pthread_attr_getschedpolicy(const pthread_attr_t *__restrict
					__attr, int *__restrict __policy);

/* Set scheduling policy in *ATTR according to POLICY.  */
extern int pthread_attr_setschedpolicy(pthread_attr_t *__attr, int __policy);

/* Return in *INHERIT the scheduling inheritance mode of *ATTR.  */
extern int pthread_attr_getinheritsched(const pthread_attr_t *__restrict
					 __attr, int *__restrict __inherit);

/* Set scheduling inheritance mode in *ATTR according to INHERIT.  */
extern int pthread_attr_setinheritsched(pthread_attr_t *__attr,
					 int __inherit);


/* Return in *SCOPE the scheduling contention scope of *ATTR.  */
extern int pthread_attr_getscope(const pthread_attr_t *__restrict __attr,
				  int *__restrict __scope);

/* Set scheduling contention scope in *ATTR according to SCOPE.  */
extern int pthread_attr_setscope(pthread_attr_t *__attr, int __scope);

/* Return the previously set address for the stack.  */
extern int pthread_attr_getstackaddr(const pthread_attr_t *__restrict
				      __attr, void **__restrict __stackaddr);

/* Set the starting address of the stack of the thread to be created.
   Depending on whether the stack grows up or down the value must either
   be higher or lower than all the address in the memory block.  The
   minimal size of the block must be PTHREAD_STACK_MIN.  */
extern int pthread_attr_setstackaddr(pthread_attr_t *__attr,
				      void *__stackaddr);

/* Return the currently used minimal stack size.  */
extern int pthread_attr_getstacksize(const pthread_attr_t *__restrict
				      __attr, size_t *__restrict __stacksize);

/* Add information about the minimum stack size needed for the thread
   to be started.  This size must never be less than PTHREAD_STACK_MIN
   and must also not exceed the system limits.  */
extern int pthread_attr_setstacksize(pthread_attr_t *__attr,
				      size_t __stacksize);

#ifdef __USE_XOPEN2K
/* Return the previously set address for the stack.  */
extern int pthread_attr_getstack(const pthread_attr_t *__restrict __attr,
				  void **__restrict __stackaddr,
				  size_t *__restrict __stacksize);

/* The following two interfaces are intended to replace the last two.  They
   require setting the address as well as the size since only setting the
   address will make the implementation on some architectures impossible.  */
extern int pthread_attr_setstack(pthread_attr_t *__attr, void *__stackaddr,
				  size_t __stacksize);
#endif

#ifdef __USE_GNU
/* Thread created with attribute ATTR will be limited to run only on
   the processors represented in CPUSET.  */
extern int pthread_attr_setaffinity_np(pthread_attr_t *__attr,
					size_t __cpusetsize,
					const cpu_set_t *__cpuset);

/* Get bit set in CPUSET representing the processors threads created with
   ATTR can run on.  */
extern int pthread_attr_getaffinity_np(const pthread_attr_t *__attr,
					size_t __cpusetsize,
					cpu_set_t *__cpuset);
#endif //__USE_GNU



/* Functions for handling thread-specific data.  */
//在多线程的环境下，进程内的所有线程共享进程的数据空间。因此全局变量为所有线程共享。
//在程序设计中有时需要保存线程自己的全局变量，这种特殊的变量仅在线程内部有效。
//
//如常见的errno，它返回标准的错误码。errno不应该是一个局部变量。几乎每个函数都应该可以访问他，
//但他又不能作为是一个全局变量。否则在一个线程里输出的很可能是另一个线程的
//
//出错信息，这个问题可以通过创建线程的私有数据（TSD  thread specific data）来解决。
//在线程内部，私有数据可以被各个函数访问。但他对其他线程是屏蔽的。
//
//线程私有数据采用了一键多值的技术，即一个键对应多个值。访问数据时都是通过键值来访问，
//好像是对一个变量进行访问，其实是在访问不同的数据。
//
// 
//
//int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
//第一个参数为指向一个键值的指针，第二个参数指明了一个destructor函数，
//
//如果这个参数不为空，那么当每个线程结束时，系统将调用这个函数来释放绑定在这个键上的内存块。
//
//key一旦被创建，所有线程都可以访问它，但各线程可根据自己的需要往key中填入不同的值，
//这就相当于提供了一个同名而不同值的全局变量，一键多值。
//
//一键多值靠的是一个关键数据结构数组即TSD池，创建一个TSD就相当于将结构数组中的某一项设置为“in_use”，
//并将其索引返回给*key，然后设置清理函数。
//
// 
//1、创建一个键
//2、为一个键设置线程私有数据
//3、从一个键读取线程私有数据void *pthread_getspecific(pthread_key_t key);
//4、线程退出（退出时，会调用destructor释放分配的缓存，参数是key所关联的数据）
//5、删除一个键
/* Create a key value identifying a location in the thread-specific
   data area.  Each thread maintains a distinct thread-specific data
   area.  DESTR_FUNCTION, if non-NULL, is called with the value
   associated to that key when the key is destroyed.
   DESTR_FUNCTION is not called if the value associated is NULL when
   the key is destroyed.  */
//第一个参数为指向一个键值的指针，
//第二个参数指明了一个destructor函数，如果这个参数不为空，那么当每个线程结束时，系统将调用这个函数来释放绑定在这个键上的内存块。
extern int pthread_key_create(pthread_key_t *__key, void (*__destr_function) (void *));

/* Destroy KEY.  */
extern int pthread_key_delete(pthread_key_t __key);

/* Return current value of the thread-specific data slot identified by KEY.  */
extern void *pthread_getspecific(pthread_key_t __key);

/* Store POINTER in the thread-specific data slot identified by KEY. */
extern int pthread_setspecific(pthread_key_t __key, const void *__pointer);

#ifdef _____PTHREAD_key_test_rongtao

#include <pthread.h>
#include <stdio.h>

pthread_key_t key;
pthread_t thid1;
pthread_t thid2;

void* thread2(void* arg)
{
    printf("thread:%lu is running\n", pthread_self());
    
    int key_va = 3 ;

    pthread_setspecific(key, (void*)key_va);
    
    printf("thread:%lu return %d\n", pthread_self(), (int)pthread_getspecific(key));
}


void* thread1(void* arg)
{
    printf("thread:%lu is running\n", pthread_self());
    
    int key_va = 5;
    
    pthread_setspecific(key, (void*)key_va);

    pthread_create(&thid2, NULL, thread2, NULL);

    printf("thread:%lu return %d\n", pthread_self(), (int)pthread_getspecific(key));
}


int main()
{
    printf("main thread:%lu is running\n", pthread_self());

    pthread_key_create(&key, NULL);

    pthread_create(&thid1, NULL, thread1, NULL);
    
    pthread_join(thid1, NULL);
    pthread_join(thid2, NULL);

    int key_va = 1;
    pthread_setspecific(key, (void*)key_va);
    
    printf("thread:%lu return %d\n", pthread_self(), (int)pthread_getspecific(key));

    pthread_key_delete(key);
        
    printf("main thread exit\n");
    return 0;
}

#endif




#ifdef __USE_XOPEN2K
/* Get ID of CPU-time clock for thread THREAD_ID.  */
extern int pthread_getcpuclockid (pthread_t __thread_id, __clockid_t *__clock_id);
#endif


/* Install handlers to be called when a new process is created with FORK.
   The PREPARE handler is called in the parent process just before performing
   FORK. The PARENT handler is called in the parent process just after FORK.
   The CHILD handler is called in the child process.  Each of the three
   handlers can be NULL, meaning that no handler needs to be called at that
   point.
   PTHREAD_ATFORK can be called several times, in which case the PREPARE
   handlers are called in LIFO order (last added with PTHREAD_ATFORK,
   first called before FORK), and the PARENT and CHILD handlers are called
   in FIFO (first added, first called).  */

extern int pthread_atfork (void (*__prepare) (void),
			   void (*__parent) (void),
			   void (*__child) (void));


#endif//_PTHREAD_H

#endif /*<__CRTL_THREAD_H>*/



