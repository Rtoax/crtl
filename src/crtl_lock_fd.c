#include "crtl_lock.h"
#include "crtl_types.h"


void crtl_fd_lock(int fd)
{
    struct flock    lock;

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;             /* write lock entire file */

    fcntl(fd, F_SETLKW, &lock);
}

void crtl_fd_unlock(int fd)
{
    struct flock    lock;

    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;             /* unlock entire file */

    fcntl(fd, F_SETLK, &lock);
}

int crtl_lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock	lock;

	lock.l_type = type;		/* F_RDLCK, F_WRLCK, F_UNLCK */
	lock.l_start = offset;	/* byte offset, relative to l_whence */
	lock.l_whence = whence;	/* SEEK_SET, SEEK_CUR, SEEK_END */
	lock.l_len = len;		/* #bytes (0 means to EOF) */

	return( fcntl(fd, cmd, &lock) );	/* -1 upon error */
}

pid_t crtl_lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
	struct flock	lock;

	lock.l_type = type;		/* F_RDLCK or F_WRLCK */
	lock.l_start = offset;	/* byte offset, relative to l_whence */
	lock.l_whence = whence;	/* SEEK_SET, SEEK_CUR, SEEK_END */
	lock.l_len = len;		/* #bytes (0 means to EOF) */

	if (fcntl(fd, F_GETLK, &lock) == -1)
		return (crtl_false);			/* unexpected error */

	if (lock.l_type == F_UNLCK)
		return(crtl_false);			/* false, region not locked by another proc */
	return(lock.l_pid);		/* true, return positive PID of lock owner */
}

