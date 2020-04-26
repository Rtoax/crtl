#include "crtl/crtl_log.h"
#include "crtl/bits/crtl_types_basic.h"
#include "crtl/linux/crtl_linux_user.h"
#include "crtl/easy/macro.h"




/**
 *	Get current user name string
 *	Author:	Rong Tao
 *	Time:	2018.11.17
 */
char* crtl_linux_getcurrentusername(char *username)
{
	if(username == NULL)
	{
		crtl_print_err("null pointer error.\n");
        return NULL;
	}
	
	t_passwd *pwd;
	pwd = t_getpwuid(getuid());
	
	if(NULL == memcpy(username, pwd->pw_name, strlen(pwd->pw_name)+1))
	{
		username = (char*)NULL;
	}
	
	return username;
#if 0
	struct passwd *pwd;
    pwd = getpwuid(getuid());
    return pwd->pw_name;
#endif
}





/**
 *	Get current user realname string
 *	Author:	Rong Tao
 *	Time:	2018.11.17
 */
char* crtl_linux_getcurrentuserrealname(char *realname)
{
	if(realname == NULL)
	{
		crtl_print_err("null pointer error.\n");
        return NULL;
	}
	
	t_passwd *pwd;
	pwd = t_getpwuid(getuid());
	
	if(NULL == memcpy(realname, pwd->pw_gecos, strlen(pwd->pw_gecos)+1))
	{
		realname = (char*)NULL;
	}
	
	return realname;
}




/**
 *	Get current user dir string
 *	Author:	Rong Tao
 *	Time:	2018.11.17
 */
char* crtl_linux_getcurrentuserdir(char *dir)
{
	if(dir == NULL)
	{
		crtl_print_err("null pointer error.\n");
        return NULL;
	}
	
	t_passwd *pwd;
	pwd = t_getpwuid(getuid());
	
	if(NULL == memcpy(dir, pwd->pw_dir, strlen(pwd->pw_dir)+1))
	{
		dir = (char*)NULL;
	}
	
	return dir;
#if 0
    char *dir = (char*)malloc(_STRING_LENGTH);
    getcwd(dir, _STRING_LENGTH);
    return dir;
#endif
}



/**
 *	Get current user shell string
 *	Author:	Rong Tao
 *	Time:	2018.11.17
 */
char* crtl_linux_getcurrentusershell(char *shell)
{
	if(shell == NULL)
	{
		crtl_print_err("null pointer error.\n");
        return NULL;
	}
	
	t_passwd *pwd;
	pwd = t_getpwuid(getuid());
	
	if(NULL == memcpy(shell, pwd->pw_shell, strlen(pwd->pw_shell)+1))
	{
		shell = (char*)NULL;
	}
	
	return shell;
#if 0
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    return pwd->pw_shell;

#endif
}



/**
 *	Get current group name string
 *	Author:	Rong Tao
 *	Time:	2018.11.17
 */
char* crtl_linux_getcurrentgrpname(char *grpname)
{
	if(grpname == NULL)
	{
		crtl_print_err("null pointer error.\n");
        return NULL;
	}
	
	t_group *grp;
	grp = t_getgrgid(getgid());
	
	if(NULL == memcpy(grpname, grp->gr_name, strlen(grp->gr_name)+1))
	{
		grpname = (char*)NULL;
	}
	
	return grpname;
}

/**
 *	Get host name string
 *	Author:	Rong Tao
 *	Time:	2018.11.17
 */
char *crtl_linux_gethostname(char *hostname)
{
	if(hostname == NULL)
	{
		crtl_print_err("null pointer error.\n");
        return NULL;
	}
	gethostname(hostname, 256);
    return hostname;
}

