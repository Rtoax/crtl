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

#include "crtl/easy/attribute.h"

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



typedef enum {
    CRTL_DIRENT_UNKNOWN,
    CRTL_DIRENT_FILE,
    CRTL_DIRENT_DIR,
    CRTL_DIRENT_LINK,
    CRTL_DIRENT_FIFO,
    CRTL_DIRENT_SOCKET,
    CRTL_DIRENT_CHAR,
    CRTL_DIRENT_BLOCK
} crtl_dirent_type_t;

struct crtl_dirent_struct {
    const char* name;
    crtl_dirent_type_t type;
};


//#define CRTL_FD_Available(fd) ({crtl_boolean __Available = crtl_chk_fd(fd)==CRTL_SUCCESS?true:false;__Available;})
//
//int crtl_chk_fd(int fd);


/* 创建文件夹 */
/**
 * create directory with mode 
 * @param dir: directory
 * @param mode: mode of directory, S_IWUSR,...
 * @return error number
 */
_api int crtl_mkdir(const char *dir, mode_t mode);


/**
 * remove directory
 * @param dir: directory
 * @return error number
 */
_api int crtl_rmdir(const char *dir); /* 删除文件夹 */


/**
 * change current work directory
 * @param dir: directory
 * @return error number
 */
_api int crtl_chdir(const char* dir);


/**
 * get current work directory
 * @param dir: directory
 * @param size: size of string of directory buffer
 * @return error number
 */
_api int crtl_cwd(char* buffer, size_t* size);

/**
 * get temperaory directory
 * @param dir: directory
 * @param size: size of string of directory buffer
 * @return error number
 */
_api int crtl_tmpdir(char* buffer, size_t* size);


/**
 * scan a directory's filter -> filter '.' and '..'
 * @param dent: type of struct dirent
 * @return error number
 */
_api int crtl_scandir_filter_default(const struct dirent* dent);

/**
 * scan a directory's sort 
 * @param a: type of struct dirent
 * @param b: type of struct dirent
 * @return error number
 */
_api int crtl_scandir_sort_default(const struct dirent** a, const struct dirent** b);


/**
 * scan a directory
 * @param maindir: directory string
 * @param namelist: name list of file in this 'maindir' directory
 * @param filter: directory
 * @param compar: directory
 * @return error number
 */
_api int crtl_scandir(const char* maindir, struct dirent*** namelist,
                        int (*filter)(const struct dirent*),
                        int (*compar)(const struct dirent**, const struct dirent **));


/* 文件选项 */

/**
 * file or directory is exist or not
 * @param dirname: file or directory name
 * @return error number
 */
_api int crtl_is_exist(const char *dirname);

/**
 * file or directory is readable or not
 * @param dirname: file or directory name
 * @return error number
 */
_api int crtl_is_readable(const char *dirname);

/**
 * file or directory is writeable or not
 * @param dirname: file or directory name
 * @return error number
 */
_api int crtl_is_writeable(const char *dirname);

/**
 * file or directory is executable or not
 * @param dirname: file or directory name
 * @return error number
 */
_api int crtl_is_executable(const char *dirname);


/**
 * file is directory or not
 * @param file: file or directory name
 * @return error number
 */
_api int crtl_is_directory(const char *file);

/**
 * file is regular file or not
 * @param file: file or directory name
 * @return error number
 */
_api int crtl_is_regular_file(const char *file);

/* 更改文件权限 */

/**
 * add read permission of file for all user
 * @param dirname: file or directory name
 * @return error number
 */
_api int crtl_add_read_permission(const char *dirname);

/**
 * add write permission of file for all user
 * @param dirname: file or directory name
 * @return error number
 */
_api int crtl_add_write_permission(const char *dirname);

/**
 * add execute permission of file for all user
 * @param dirname: file or directory name
 * @return error number
 */
_api int crtl_add_execute_permission(const char *dirname);

/**
 * add all permission of file for current user
 * @param dirname: file or directory name
 * @return error number
 */
_api int crtl_add_usr_permission(const char *dirname);

/**
 * add all permission of file for current group
 * @param dirname: file or directory name
 * @return error number
 */
_api int crtl_add_grp_permission(const char *dirname);

/**
 * add all permission of file for other users
 * @param dirname: file or directory name
 * @return error number
 */
_api int crtl_add_oth_permission(const char *dirname);


/**
 * add all permission of file for all user
 * @param dirname: file or directory name
 * @return error number
 */
_api int crtl_add_all_permission(const char *dirname);


/**
 * make tmp string with prefix and path
 * @param dirname: file or directory name
 * @return error number
 */
_api char *crtl_mktemp_string(char * const tempfile_out, const char *path, const char *fileprefix);



/**
 * there are some system swap functions with error print
 * @param 
 * @return
 */
_api FILE *crtl_efopen(const char *file, const char *mode);
_api FILE *crtl_efreopen(const char *file, const char *mode, FILE *stream1);
_api FILE *crtl_efdopen(int fd, const char *mode);
_api FILE *crtl_epopen(char *command, char *type);
_api int crtl_efclose(FILE *stream);
_api int crtl_epclose(FILE *stream);
_api int crtl_efflush(FILE *stream);
_api int crtl_eremove(const char *file);
_api int crtl_erename(const char *oldfile, const char *newfile);
_api int crtl_efseek(FILE *stream, off_t offset, int origin);
_api void crtl_erewind(FILE *stream)	/* dummy function */;
_api long crtl_eftell(FILE *stream);
_api int crtl_efseeko(FILE *stream, off_t offset, int origin);
_api off_t crtl_eftello(FILE *streem);
//FILE *crtl_etmpstream(char * prefix);
_api FILE *crtl_etmpfile(void);
_api void *crtl_emalloc(size_t size);
_api void *crtl_erealloc(void *memptr, size_t size);
_api void *crtl_ecalloc(size_t count, size_t size);
_api int crtl_efgetpos(FILE *stream, fpos_t *position);
_api int crtl_efsetpos(FILE *stream, const fpos_t *position);
_api size_t crtl_efwrite(void *bufptr, size_t size, size_t count, FILE *stream);
_api size_t crtl_efread(void *bufptr, size_t size, size_t count, FILE *stream);



#endif /*<__CRTL_FILE_H>*/


