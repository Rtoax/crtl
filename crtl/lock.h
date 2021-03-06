#ifndef __CRTL_LOCK_H
#define __CRTL_LOCK_H 1

#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>


#include "crtl/bits/semaphore.h"
#include "crtl/bits/mutex.h"
#include "crtl/bits/spinlock.h"
#include "crtl/bits/rwlock.h"
#include "crtl/bits/cond.h"
#include "crtl/bits/barrier.h"




/* macro */
#define	read_lock(fd, offset, whence, len)          crtl_lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)
#define	readw_lock(fd, offset, whence, len)         crtl_lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len)
#define	write_lock(fd, offset, whence, len)         crtl_lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)
#define	writew_lock(fd, offset, whence, len)        crtl_lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define	un_lock(fd, offset, whence, len)            crtl_lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)
#define	is_read_lockable(fd, offset, whence, len)   crtl_lock_test(fd, F_RDLCK, offset, whence, len)
#define	is_write_lockable(fd, offset, whence, len)  crtl_lock_test(fd, F_WRLCK, offset, whence, len)




/**
 * file descripter lock write
 * @param fd: file descripter
 * @return void
 */
void crtl_fd_lock(int fd);

/**
 * file descripter unlock
 * @param fd: file descripter
 * @return void
 */
void crtl_fd_unlock(int fd);


int  crtl_lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);
pid_t crtl_lock_test(int fd, int type, off_t offset, int whence, off_t len);



#endif /*<__CRTL_LOCK_H>*/

