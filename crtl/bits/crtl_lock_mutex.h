#ifndef __CRTL_BITS_LOCK_MUTEX_H
#define __CRTL_BITS_LOCK_MUTEX_H 1


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <pthread.h>


typedef pthread_mutex_t     crtl_lock_mutex_t;
typedef pthread_mutexattr_t crtl_lock_mutexattr_t;

#define CRTL_LOCK_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER;



int crtl_mutex_init(crtl_lock_mutex_t *mutex, const crtl_lock_mutexattr_t *attr);
int crtl_mutex_destroy(crtl_lock_mutex_t *mutex);
int crtl_mutex_lock(crtl_lock_mutex_t *__mutex, int trywait, int timedwait, int seconds, long nanoseconds);
int crtl_mutex_unlock(crtl_lock_mutex_t *__mutex);


int crtl_mutexattr_init(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_destroy(crtl_lock_mutexattr_t *__attr);

int crtl_mutexattr_getpshared(const crtl_lock_mutexattr_t *__attr, int *__pshared);
int crtl_mutexattr_setpshared_shared(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setpshared_private(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setpshared(crtl_lock_mutexattr_t *__attr, int __pshared);

int crtl_mutexattr_gettype(const crtl_lock_mutexattr_t *__attr, int *__kind);
int crtl_mutexattr_settype_normal(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_settype_errorcheck(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_settype_recursive(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_settype_default(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_settype(crtl_lock_mutexattr_t *__attr, int __kind);

int crtl_mutexattr_getprotocol(const crtl_lock_mutexattr_t *__attr, int *__protocol);
int crtl_mutexattr_setprotocol_none(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setprotocol_inherit(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setprotocol_protect(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setprotocol(crtl_lock_mutexattr_t *__attr, int __protocol);


int crtl_mutexattr_getrobust(const crtl_lock_mutexattr_t *__attr, int *__robustness);
int crtl_mutexattr_setrobust_stalled(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setrobust_robust(crtl_lock_mutexattr_t *__attr);
int crtl_mutexattr_setrobust(crtl_lock_mutexattr_t *__attr, int __robustness);






#ifndef _PTHREAD_H

/* Mutex handling.  */
/* Initialize a mutex.  */
extern int pthread_mutex_init (pthread_mutex_t *__mutex, const pthread_mutexattr_t *__mutexattr);

/* Destroy a mutex.  */
extern int pthread_mutex_destroy (pthread_mutex_t *__mutex);

/* Try locking a mutex.  */
extern int pthread_mutex_trylock (pthread_mutex_t *__mutex);

/* Lock a mutex.  */
extern int pthread_mutex_lock (pthread_mutex_t *__mutex);

#ifdef __USE_XOPEN2K
/* Wait until lock becomes available, or specified time passes. */
extern int pthread_mutex_timedlock (pthread_mutex_t * __mutex, const struct timespec * __abstime);
#endif

/* Unlock a mutex.  */
extern int pthread_mutex_unlock (pthread_mutex_t *__mutex);


/* Get the priority ceiling of MUTEX.  */
extern int pthread_mutex_getprioceiling (const pthread_mutex_t *__mutex, int *__prioceiling);

/* Set the priority ceiling of MUTEX to PRIOCEILING, return old
   priority ceiling value in *OLD_CEILING.  */
extern int pthread_mutex_setprioceiling (pthread_mutex_t *__mutex, int __prioceiling, int *__old_ceiling);


#ifdef __USE_XOPEN2K8
/* Declare the state protected by MUTEX as consistent.  */
extern int pthread_mutex_consistent (pthread_mutex_t *__mutex);
# ifdef __USE_GNU
extern int pthread_mutex_consistent_np (pthread_mutex_t *__mutex);
# endif
#endif


/* Functions for handling mutex attributes.  */

/* Initialize mutex attribute object ATTR with default attributes
   (kind is PTHREAD_MUTEX_TIMED_NP).  */
extern int pthread_mutexattr_init(pthread_mutexattr_t *__attr);

/* Destroy mutex attribute object ATTR.  */
extern int pthread_mutexattr_destroy(pthread_mutexattr_t *__attr);

//所述pthread_mutexattr_getpshared（）函数应取得的值进程共享从属性对象通过参考属性 ATTR。
//所述pthread_mutexattr_setpshared（）函数将设置进程共享在初始化属性属性对象通过引用 ATTR。
//的进程共享的属性被设置为PTHREAD_PROCESS_SHARED，以允许要由有权访问，
//其中该互斥被分配存储器的任何线程操作时一个互斥，即使该互斥在由多个进程共享存储器分配。
//如果进程共享属性为PTHREAD_PROCESS_PRIVATE，则互斥锁只能由在与初始化互斥锁的线程相同的进程内创建的线程操作；
//如果不同进程的线程尝试对该互斥量进行操作，则该行为是不确定的。该属性的默认值应为PTHREAD_PROCESS_PRIVATE。
/* Get the process-shared flag of the mutex attribute ATTR.  */
extern int pthread_mutexattr_getpshared(const pthread_mutexattr_t * __attr, int *__pshared);

/* Set the process-shared flag of the mutex attribute ATTR.  */
extern int pthread_mutexattr_setpshared(pthread_mutexattr_t *__attr, int __pshared);

#if defined __USE_UNIX98 || defined __USE_XOPEN2K8
//该pthread_mutexattr_gettype（）和pthread_mutexattr_settype（）函数，须分别获取和设置互斥锁类型的属性。
//在type参数中为这些功能设置此属性。type属性的默认值为PTHREAD_MUTEX_DEFAULT。
//
//互斥锁的类型包含在互斥锁属性的type属性中。有效的互斥锁类型包括：
//
//PTHREAD_MUTEX_NORMAL
//这种类型的互斥锁不会检测到死锁。试图重新锁定此互斥锁而未先对其解锁的线程将死锁。
//尝试解锁由其他线程锁定的互斥锁会导致未定义的行为。尝试解锁未锁定的互斥锁会导致未定义的行为。
//
//PTHREAD_MUTEX_ERRORCHECK
//这种互斥锁提供错误检查。试图重新锁定该互斥锁而不先对其进行解锁的线程将返回错误。
//试图解锁另一个线程已锁定的互斥锁的线程将返回错误。试图解锁未锁定的互斥锁的线程将返回错误。
//
//PTHREAD_MUTEX_RECURSIVE
//试图重新锁定该互斥锁而不先对其进行解锁的线程应成功锁定该互斥锁。
//这种类型的互斥锁不会发生PTHREAD_MUTEX_NORMAL类型的互斥锁可能发生的重新锁定死锁。
//在另一个线程可以获取互斥锁之前，此互斥锁的多个锁应要求解锁次数相同才能释放互斥锁。
//试图解锁另一个线程已锁定的互斥锁的线程将返回错误。试图解锁未锁定的互斥锁的线程将返回错误。
//
//PTHREAD_MUTEX_DEFAULT
//尝试递归锁定此类型的互斥锁会导致未定义的行为。尝试解锁未被调用线程锁定的这种类型的互斥锁会导致未定义的行为。
//尝试解锁未锁定的这种类型的互斥锁会导致未定义的行为。一个实现可以将此互斥锁映射到其他互斥锁类型之一。

/* Return in *KIND the mutex kind attribute in *ATTR.  */
extern int pthread_mutexattr_gettype(const pthread_mutexattr_t *__attr, int * __kind);

/* Set the mutex kind attribute in *ATTR to KIND (either PTHREAD_MUTEX_NORMAL,
   PTHREAD_MUTEX_RECURSIVE, PTHREAD_MUTEX_ERRORCHECK, or
   PTHREAD_MUTEX_DEFAULT).  */
extern int pthread_mutexattr_settype(pthread_mutexattr_t *__attr, int __kind);
#endif


//所述pthread_mutexattr_getprotocol（）和pthread_mutexattr_setprotocol（）函数，
//须分别获取和设置互斥属性的协议属性对象指向的ATTR这是以前由函数创建pthread_mutexattr_init（）。
//
//该协议属性定义应遵循在使用互斥的协议。协议的值可以是以下之一：
//
//PTHREAD_PRIO_NONE
//
//PTHREAD_PRIO_INHERIT
//
//PTHREAD_PRIO_PROTECT
//
//在< pthread.h >头文件中定义。
//
//当线程拥有具有PTHREAD_PRIO_NONE 协议属性的互斥锁时，其优先级和调度将不受其互斥锁所有权的影响。
//
//当一个线程由于拥有一个或多个具有PTHREAD_PRIO_INHERIT 协议属性的互斥锁而阻塞较高优先级的线程时，
//它应以其优先级或最高优先级线程的优先级中的较高者执行，等待该线程拥有并初始化的任何互斥锁使用此协议。
//
//当一个线程拥有一个或多个使用PTHREAD_PRIO_PROTECT协议初始化的互斥锁时，
//它将以该线程拥有并使用此属性初始化的所有互斥锁的优先级较高或最高优先级的上限执行，
//而不管其他线程是否是否阻止了这些互斥锁。
//
//当线程持有已使用PTHREAD_PRIO_INHERIT或PTHREAD_PRIO_PROTECT协议属性初始化的互斥锁时，
//如果更改了其原始优先级（例如通过更改），则该优先级不得移动到调度队列的尾部。调用 sched_setparam（）。
//同样，当线程解锁已使用PTHREAD_PRIO_INHERIT或PTHREAD_PRIO_PROTECT协议属性初始化的互斥锁时，
//如果更改了其原始优先级，则该优先级将不被移动到调度队列的尾部。
//
//如果一个线程同时拥有用不同协议初始化的多个互斥锁，则它应以这些协议中的每个协议获得的最高优先级执行。
//
//当一个线程调用pthread_mutex_lock（）时，互斥锁被初始化为具有属性值PTHREAD_PRIO_INHERIT的协议属性，
//当由于互斥锁由另一个线程拥有而导致调用线程被阻塞时，该所有者线程将继承调用的优先级。
//线程，只要它继续拥有互斥体即可。实现应将其执行优先级更新为分配的优先级和所有继承的优先级中的最大值。
//此外，如果该所有者线程本身在另一个互斥锁上被阻塞，则应以递归的方式将相同的优先级继承效果传播到该另一个所有者线程。
/* Return in *PROTOCOL the mutex protocol attribute in *ATTR.  */
extern int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *__attr, int *__protocol);

/* Set the mutex protocol attribute in *ATTR to PROTOCOL (either
   PTHREAD_PRIO_NONE, PTHREAD_PRIO_INHERIT, or PTHREAD_PRIO_PROTECT).  */
extern int pthread_mutexattr_setprotocol(pthread_mutexattr_t *__attr,int __protocol);

//#define SCHED_OTHER	0
//#define SCHED_FIFO	1
//#define SCHED_RR	2
//#ifdef __USE_GNU
//# define SCHED_BATCH	3
//#endif
/* Return in *PRIOCEILING the mutex prioceiling attribute in *ATTR.  */
extern int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *__attr, int * __prioceiling);

/* Set the mutex prioceiling attribute in *ATTR to PRIOCEILING.  */
extern int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *__attr,int __prioceiling);


//pthread_mutexattr_getrobust，pthread_mutexattr_setrobust-获取和    设置互斥量属性对象的鲁棒性属性
//该pthread_mutexattr_getrobust（）函数将值了
//       引用的互斥体对象的鲁棒性属性
//       ATTR在*鲁棒性。所述pthread_mutexattr_setrobust（）函数集
//       互斥属性对象的鲁棒性属性的值
//       由attr引用* robustness中指定的值。
//
//       鲁棒性属性指定互斥锁在以下情况下的行为：
//       拥有线程死亡而没有解锁互斥锁。以下值
//       对于健壮性有效：
//
//       PTHREAD_MUTEX_STALLED
//              这是互斥体属性对象的默认值。如果一个
//              互斥锁使用PTHREAD_MUTEX_STALLED属性初始化
//              并且其所有者在没有解锁的情况下死亡，互斥体仍然存在
//              之后锁定，以后再尝试致电
//              互斥锁上的pthread_mutex_lock（3）将无限期阻塞。
//
//       PTHREAD_MUTEX_ROBUST 
//              如果使用PTHREAD_MUTEX_ROBUST初始化了互斥锁
//              属性及其所有者在未解锁的情况下死亡
//              尝试在此互斥锁上调用pthread_mutex_lock（3）
//              成功并返回EOWNERDEAD以表明原始
//              所有者不再存在，并且互斥体不一致
//              州。通常在EOWNERDEAD返回之后，下一个所有者
//              应该在获取的互斥锁上调用pthread_mutex_consistent（3）
//              在再次使用之前使其保持一致。
//
//              如果下一个所有者使用以下方法解锁互斥锁
//              pthread_mutex_unlock（3）在使其一致之前，互斥锁
//              将永远无法使用，并且任何后续尝试
//              使用pthread_mutex_lock（3）对其进行锁定将失败，并显示错误
//               ENOTRECOVERABLE。此类互斥锁上唯一允许的操作
//              是pthread_mutex_destroy（3）。
//
//              如果下一个所有者在致电之前终止
//              pthread_mutex_consistent（3），进一步pthread_mutex_lock（3）
//              此互斥锁上的操作仍将返回EOWNERDEAD。

#ifdef __USE_XOPEN2K
/* Get the robustness flag of the mutex attribute ATTR.  */
extern int pthread_mutexattr_getrobust(const pthread_mutexattr_t *__attr, int *__robustness);
# ifdef __USE_GNU
extern int pthread_mutexattr_getrobust_np(const pthread_mutexattr_t *__attr, int *__robustness);
# endif

/* Set the robustness flag of the mutex attribute ATTR.  */
extern int pthread_mutexattr_setrobust(pthread_mutexattr_t *__attr, int __robustness);
# ifdef __USE_GNU
extern int pthread_mutexattr_setrobust_np(pthread_mutexattr_t *__attr, int __robustness);
# endif
#endif


#endif //_PTHREAD_H


#endif //__CRTL_BITS_LOCK_MUTEX_H

