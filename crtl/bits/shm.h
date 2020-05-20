#ifndef __CRTL_BITS_SHM_POSIX_H
#define __CRTL_BITS_SHM_POSIX_H 1


#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>          /* For truncate */

#include "crtl/file.h"

#include "crtl/bits/types_basic.h"

#include "crtl/easy/attribute.h"



/* shm_open oflag param */
#define CRTL_SHM_FILE_OFLAG          (O_RDWR | O_CREAT) /* exist->return fd */
#define CRTL_SHM_FILE_EOFLAG         (O_RDWR | O_CREAT | O_EXCL) /* exist->return -1 */

/* shm_open mode_t param */
#define CRTL_SHM_FILE_MODE           CRTL_FILE_MODE


/* mmap prot */
#define CRTL_SHM_MMAP_PROT_CREATE    (PROT_READ|PROT_WRITE)/* page can be read/written */
#define CRTL_SHM_MMAP_PROT_READ      (PROT_READ)/* page can be read */
#define CRTL_SHM_MMAP_PROT_WRITE     (PROT_WRITE)/* page can be written */


/* mmap flags */
#define CRTL_SHM_MMAP_FLAGS           MAP_SHARED  /* Share changes */




_api int crtl_posix_shm_open(const char *name, int size, int oflag, int mode);
_api void *crtl_posix_shm_mmap(int size, int prot, int flags, int fd);
_api int crtl_posix_shm_munmap(void *addr, int size);
_api int crtl_posix_shm_munmap(void *addr, int size);
_api int crtl_posix_shm_put(void *shm_addr, const void *data, int data_len);
_api int crtl_posix_shm_get(void *shm_addr, void *data, int data_len);
_api int crtl_posix_shm_unlink(const char *name);





/* POSIX API */
extern int shm_open(const char *name, int oflag, mode_t mode);
extern int shm_unlink(const char *name);


extern void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
extern int munmap(void *addr, size_t length);


extern int truncate(const char *path, off_t length);
extern int ftruncate(int fildes, off_t length);




#endif /*<__CRTL_BITS_SHM_POSIX_H>*/
