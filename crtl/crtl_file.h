#ifndef __CRTL_FILE_H
#define __CRTL_FILE_H 1

#include <stdio.h>
#include <dirent.h> //DIR 
#include <string.h>
#include <unistd.h>
#include <libgen.h> //dirname
#include <sys/stat.h>
#include <stdlib.h>

#define CRTL_CURRENT_DIR   "."
#define CRTL_FATHER_DIR        ".."


/* default file access permissions for new files */
#define	CRTL_FILE_MODE        (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/* default permissions for new directories */
#define	CRTL_DIR_MODE         (CRTL_FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

#define CRTL_S_ISDIR(mode)   __S_ISTYPE((mode), __S_IFDIR)
#define	CRTL_S_ISREG(mode)   __S_ISTYPE((mode), __S_IFREG)


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


