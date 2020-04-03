#ifndef __CRTL_LINUX_USER_H
#define __CRTL_LINUX_USER_H 1

#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <malloc.h>



/**
 *  There are some old version
 *  Rongtao 2019.07.20
 */
/*	file name: 		user.h
 *	author:			Rong Tao
 *	create time:	2018.11.17
 * 	
 */

#include <pwd.h>
#include <grp.h>

typedef struct passwd t_passwd;
typedef struct group t_group;


/*struct passwd * getpwuid (uid t uid)*/
#define t_getpwuid(uid) getpwuid(uid) 
/*struct group * getgrgid (gid t gid)*/
#define t_getgrgid(gid) getgrgid(gid)

/*getpwuid_r (uid_t uid, struct passwd *result_buf, char *buffer,size t buflen, struct passwd **result)*/
#define t_getpwuid_r(uid,passwd_buf,buf,buflen,result) getpwuid_r(uid,passwd_buf,buf,buflen,result)

/*struct passwd * getpwnam (const char *name)*/
#define t_getpwnam(name) getpwnam(name)
/*struct group * getgrnam (const char *name)*/
#define t_getgrnam(name) getgrnam(name)

/*int getpwnam_r (const char *name, struct passwd *result_buf, char *buffer, size t buflen, struct passwd **result)*/
#define t_getpwnam_r(name,passwd_buf,buf,buflen,result) getpwnam_r(name,passwd_buf,buf,buflen,result)

/*struct passwd * fgetpwent (FILE *stream)*/
#define t_fgetpwent(fp) fgetpwent(fp)




char* crtl_linux_getcurrentusername(char *username);
char* crtl_linux_getcurrentuserrealname(char *realname);
char* crtl_linux_getcurrentuserdir(char *dir);
char* crtl_linux_getcurrentusershell(char *shell);
char* crtl_linux_getcurrentgrpname(char *grpname);
char* crtl_linux_gethostname(char *hostname);


#endif /*<__CRTL_LINUX_USER_H>*/

