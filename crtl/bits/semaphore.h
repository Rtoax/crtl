#ifndef __CRTL_BITS_LOCK_SEMAPHORE_H
#define __CRTL_BITS_LOCK_SEMAPHORE_H 1


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>


typedef sem_t   crtl_lock_sem_t;


int crtl_semaphore_init(crtl_lock_sem_t *__sem, unsigned int value);
int crtl_semaphore_destroy(crtl_lock_sem_t *__sem);
int crtl_semaphore_wait(crtl_lock_sem_t *__sem, int trywait, int timedwait, int seconds, long nanoseconds);
int crtl_semaphore_post(crtl_lock_sem_t *__sem);
int crtl_semaphore_getvalue(crtl_lock_sem_t * __sem);


crtl_lock_sem_t *crtl_semaphore_fopen(const char *__name, unsigned int value);
int crtl_semaphore_fclose(crtl_lock_sem_t *__sem);
int crtl_semaphore_funlink(const char *__name);





#endif //__CRTL_BITS_LOCK_SEMAPHORE_H
