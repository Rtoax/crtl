#ifndef __CRTL_FILE_H
#define __CRTL_FILE_H 1

#include <stdio.h>
#include <dirent.h> //DIR 
#include <string.h>
#include <unistd.h>
#include <libgen.h> //dirname
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define CRTL_CURRENT_DIR   "."
#define CRTL_FATHER_DIR        ".."


/* default file access permissions for new files */
#define	CRTL_FILE_MODE        (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/* default permissions for new directories */
#define	CRTL_DIR_MODE         (CRTL_FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

#define CRTL_S_ISDIR(mode)   __S_ISTYPE((mode), __S_IFDIR)
#define	CRTL_S_ISREG(mode)   __S_ISTYPE((mode), __S_IFREG)

#define CRTL_O_RDONLY   O_RDONLY
#define CRTL_O_WRONLY   O_WRONLY
#define CRTL_O_RDWR     O_RDWR
#define CRTL_O_NONBLK   O_NONBLOCK
#define CRTL_O_CREAT    O_CREAT
#define CRTL_O_EXCL     O_EXCL


#define CRTL_O_MSGQ         (CRTL_O_RDWR|CRTL_O_CREAT)
#define CRTL_O_MSGQ_EXCL     (CRTL_O_RDWR|CRTL_O_CREAT|CRTL_O_EXCL)

#define CRTL_MSGQ_MODE  (S_IRUSR|S_IWUSR)

//O_RDONLY
//Open the queue to receive messages only.
//
//O_WRONLY
//Open the queue to send messages only.
//
//O_RDWR Open the queue to both send and receive messages.
//
//Zero or more of the following flags can additionally be ORed in oflag:
//
//O_NONBLOCK
//Open  the  queue  in  nonblocking mode.  In circumstances where mq_receive(3) and mq_send(3) would
//normally block, these functions instead fail with the error EAGAIN.
//
//O_CREAT
//Create the message queue if it does not exist.  The owner (user ID) of the message queue is set to
//the effective user ID of the calling process.  The group ownership (group ID) is set to the effec‐
//tive group ID of the calling process.
//
//O_EXCL If O_CREAT was specified in oflag, and a queue with the given name already exists, then fail  with
//the error EEXIST.


/* Chmod */
#define CRTL_S_USR       (S_IRUSR|S_IWUSR|S_IXUSR)
#define CRTL_S_GRP       (S_IRGRP|S_IWGRP|S_IXGRP)
#define CRTL_S_OTH       (S_IROTH|S_IWOTH|S_IXOTH)
#define CRTL_S_R         (S_IRUSR|S_IRGRP|S_IROTH)
#define CRTL_S_W         (S_IWUSR|S_IWGRP|S_IWOTH)
#define CRTL_S_X         (S_IXUSR|S_IXGRP|S_IXOTH)
#define CRTL_S_ALL       (S_USR|S_GRP|S_OTH)


#define chmod_r(f)      chmod(f, CRTL_S_R);
#define chmod_w(f)      chmod(f, CRTL_S_W);
#define chmod_x(f)      chmod(f, CRTL_S_X);
#define chmod_usr(f)    chmod(f, CRTL_S_USR);
#define chmod_grp(f)    chmod(f, CRTL_S_GRP);
#define chmod_oth(f)    chmod(f, CRTL_S_OTH);
#define chmod_all(f)    chmod(f, CRTL_S_ALL);


int crtl_mkdir(const char *dir, mode_t mode);
int crtl_rmdir(const char *dir);

int crtl_is_directory(const char *file);
int crtl_is_regular_file(const char *file);

char *crtl_mktempfile(char * const tempfile_out, const char *path, const char *fileprefix);



#endif /*<__CRTL_FILE_H>*/


