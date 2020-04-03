#ifndef __CRTL_BITS_SHM_SYSTEMV_H
#define __CRTL_BITS_SHM_SYSTEMV_H 1


/* System V */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


/* System V Structure */
struct shmid_ds;


/* System V API */
extern void *shmat(int shmid, const void *shmaddr, int shmflg);
extern int shmdt(const void *shmaddr);
extern int shmctl(int shmid, int cmd, struct shmid_ds *buf);
extern int shmget(key_t key, size_t size, int shmflg);


extern key_t ftok(const char *pathname, int proj_id);



#endif /*<__CRTL_BITS_SHM_SYSTEMV_H>*/
