#ifndef __CRTL_THREAD_H
#define __CRTL_THREAD_H 1

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
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





#define crtl_thread_normal(p_threadID, fn, arg, cleanup_fn, cleanup_arg) \
    crtl_thread_create(p_threadID, PTHREAD_CREATE_DETACHED, 0, \
                        CRTL_THREAD_SCHED_PRIO_NORMAL, SCHED_FIFO, PTHREAD_SCOPE_SYSTEM,\
                        NULL, 0, 0, fn, arg, cleanup_fn, cleanup_arg)





typedef pthread_t       crtl_thread_t;
typedef pthread_attr_t  crtl_threadattr_t;


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
int crtl_thread_join(crtl_thread_t __th, void **__thread_return, int tryjoin, int timedjoin, int seconds, long nanoseconds);
int crtl_thread_getattr(crtl_thread_t thread, crtl_threadattr_t *attr);

int crtl_thread_equal(crtl_thread_t thread1, crtl_thread_t thread2);





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
extern int pthread_yield (void);


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
extern int pthread_setcancelstate (int __state, int *__oldstate);

/* Set cancellation state of current thread to TYPE, returning the old
   type in *OLDTYPE if OLDTYPE is not NULL.  */
extern int pthread_setcanceltype (int __type, int *__oldtype);

/* Cancel THREAD immediately or at the next possibility.  */
extern int pthread_cancel (pthread_t __th);

/* Test for pending cancellation for the current thread and terminate
   the thread as per pthread_exit(PTHREAD_CANCELED) if it has been
   cancelled.  */
extern void pthread_testcancel (void);




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

#endif//_PTHREAD_H

#endif /*<__CRTL_THREAD_H>*/



