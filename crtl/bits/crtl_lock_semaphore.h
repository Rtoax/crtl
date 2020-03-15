#ifndef __CRTL_BITS_LOCK_SEMAPHORE_H
#define __CRTL_BITS_LOCK_SEMAPHORE_H 1


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>


typedef sem_t   crtl_lock_sem_t;


int crtl_semaphore_init (crtl_lock_sem_t *__sem, unsigned int value);
int crtl_semaphore_destroy (crtl_lock_sem_t *__sem);
int crtl_semaphore_wait (crtl_lock_sem_t *__sem, int trywait, int timedwait, int seconds, long nanoseconds);
int crtl_semaphore_post (crtl_lock_sem_t *__sem);
int crtl_semaphore_getvalue (crtl_lock_sem_t * __sem);


crtl_lock_sem_t *crtl_semaphore_fopen(const char *__name, unsigned int value);
int crtl_semaphore_fclose(crtl_lock_sem_t *__sem);
int crtl_semaphore_funlink(const char *__name);




#ifndef _SEMAPHORE_H

/* Initialize semaphore object SEM to VALUE.  If PSHARED then share it
   with other processes.  */
extern int sem_init (sem_t *__sem, int __pshared, unsigned int __value);

/* Free resources associated with semaphore object SEM.  */
extern int sem_destroy (sem_t *__sem);

/* Open a named semaphore NAME with open flags OFLAG.  */
sem_t *sem_open(const char *__name, int __oflag, ...);
sem_t *sem_open(const char *name, int oflag);
sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);

/* Close descriptor for named semaphore SEM.  */
extern int sem_close (sem_t *__sem);

/* Remove named semaphore NAME.  */
extern int sem_unlink (const char *__name);

/* Wait for SEM being posted.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int sem_wait (sem_t *__sem);

#ifdef __USE_XOPEN2K
/* Similar to `sem_wait' but wait only until ABSTIME.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int sem_timedwait (sem_t * __sem, const struct timespec * __abstime);
#endif

/* Test whether SEM is posted.  */
extern int sem_trywait (sem_t *__sem);

/* Post SEM.  */
extern int sem_post (sem_t *__sem);

/* Get current value of SEM and store it in *SVAL.  */
extern int sem_getvalue (sem_t * __sem, int * __sval);

#endif//_SEMAPHORE_H

#endif //__CRTL_BITS_LOCK_SEMAPHORE_H
