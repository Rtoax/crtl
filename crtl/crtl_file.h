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

#include "crtl/bits/crtl_types_basic.h"

//当前目录
#define CRTL_CURRENT_DIR   "."
#define CRTL_FATHER_DIR        ".." //上一级目录


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



/* Chmod */
#define CRTL_S_USR       (S_IRUSR|S_IWUSR|S_IXUSR)
#define CRTL_S_GRP       (S_IRGRP|S_IWGRP|S_IXGRP)
#define CRTL_S_OTH       (S_IROTH|S_IWOTH|S_IXOTH)
#define CRTL_S_R         (S_IRUSR|S_IRGRP|S_IROTH)
#define CRTL_S_W         (S_IWUSR|S_IWGRP|S_IWOTH)
#define CRTL_S_X         (S_IXUSR|S_IXGRP|S_IXOTH)
#define CRTL_S_ALL       (CRTL_S_USR|CRTL_S_GRP|CRTL_S_OTH)


#define chmod_r(f)      chmod(f, CRTL_S_R);
#define chmod_w(f)      chmod(f, CRTL_S_W);
#define chmod_x(f)      chmod(f, CRTL_S_X);
#define chmod_usr(f)    chmod(f, CRTL_S_USR);
#define chmod_grp(f)    chmod(f, CRTL_S_GRP);
#define chmod_oth(f)    chmod(f, CRTL_S_OTH);
#define chmod_all(f)    chmod(f, CRTL_S_ALL);



//#define CRTL_FD_Available(fd) ({crtl_boolean __Available = crtl_chk_fd(fd)==CRTL_SUCCESS?true:false;__Available;})
//
//int crtl_chk_fd(int fd);


/* 创建文件夹 */
int crtl_mkdir(const char *dir, mode_t mode);
int crtl_rmdir(const char *dir); /* 删除文件夹 */

/* 文件选项 */
int crtl_is_exist(const char *dirname);
int crtl_is_readable(const char *dirname);
int crtl_is_writeable(const char *dirname);
int crtl_is_executable(const char *dirname);

int crtl_is_directory(const char *file);
int crtl_is_regular_file(const char *file);

/* 更改文件权限 */
int crtl_add_read_permission(const char *dirname);
int crtl_add_write_permission(const char *dirname);
int crtl_add_execute_permission(const char *dirname);
int crtl_add_usr_permission(const char *dirname);
int crtl_add_grp_permission(const char *dirname);
int crtl_add_oth_permission(const char *dirname);
int crtl_add_oth_permission(const char *dirname);


char *crtl_mktemp_string(char * const tempfile_out, const char *path, const char *fileprefix);




FILE *crtl_efopen(const char *file, const char *mode);
FILE *crtl_efreopen(const char *file, const char *mode, FILE *stream1);
FILE *crtl_efdopen(int fd, const char *mode);
FILE *crtl_epopen(char *command, char *type);
int crtl_efclose(FILE *stream);
int crtl_epclose(FILE *stream);
int crtl_efflush(FILE *stream);
int crtl_eremove(const char *file);
int crtl_erename(const char *oldfile, const char *newfile);
int crtl_efseek(FILE *stream, off_t offset, int origin);
void crtl_erewind(FILE *stream)	/* dummy function */;
long crtl_eftell(FILE *stream);
int crtl_efseeko(FILE *stream, off_t offset, int origin);
off_t crtl_eftello(FILE *streem);
//FILE *crtl_etmpstream(char * prefix);
FILE *crtl_etmpfile(void);
void *crtl_emalloc(size_t size);
void *crtl_erealloc(void *memptr, size_t size);
void *crtl_ecalloc(size_t count, size_t size);
int crtl_efgetpos(FILE *stream, fpos_t *position);
int crtl_efsetpos(FILE *stream, const fpos_t *position);
size_t crtl_efwrite(void *bufptr, size_t size, size_t count, FILE *stream);
size_t crtl_efread(void *bufptr, size_t size, size_t count, FILE *stream);



#endif /*<__CRTL_FILE_H>*/


