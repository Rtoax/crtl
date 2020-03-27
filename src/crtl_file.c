#include <malloc.h>
#include <errno.h>

#include "crtl/crtl_file.h"
#include "crtl/crtl_types.h"

#include "crtl/crtl_log.h"
#include "crtl/easy/attribute.h"



/* create dir */
int crtl_mkdir(const char *dir, mode_t mode)
{
    if(dir == NULL || strlen(dir) <= 0)
    {
        crtl_print_err("null pointer error.\n");
        return CRTL_ERROR;
    }
    int _unused ret = -1;
    
    char *__dir = malloc(strlen(dir)+1);
    strcpy(__dir, dir);
    
    //log_debug("dir: %s\n", __dir);
    
    /**
     *  If directory not exist, return 0.
     */ 
    if(0 == access(__dir, F_OK))  
    {       
        DIR *dirp;
        if((dirp = opendir(__dir)) == NULL)
        {
            crtl_print_err("File already exist, but it's not directory.\n");
            return CRTL_ERROR;
        }
        closedir(dirp);
        return CRTL_SUCCESS;   
    }
    else
    {
        ret = mkdir(__dir, mode);
        //if(ret == 0)log_notice("mkdir: %s\n", __dir);

        dirname(__dir);
        crtl_mkdir(__dir, mode);
        
        ret = mkdir(dir, mode);
        //if(ret == 0)log_notice("mkdir: %s\n", dir);
    }
    free(__dir);
    
    return CRTL_SUCCESS;
}

/* remove dir */
int crtl_rmdir(const char *dir)
{
    if(dir == NULL)
    {
        crtl_print_err("null pointer error.\n");
        return CRTL_ERROR;
    } 
    
    char dir_name[1024]; 
    DIR *dirp;  
    struct dirent *dp;  
    struct stat dir_stat;   
    
    /**
     *  If directory not exist, return 0.
     */ 
    if(0 != access(dir, F_OK))  
    {       
        return CRTL_ERROR;   
    }   
    /**
     * Get attribution error
     */   
    if(0 != stat(dir, &dir_stat))    
    {       
        crtl_print_err("Get directory stat error.\n");        
        return CRTL_ERROR;  
    }   
    /**
     *  If regular file, delete it.
     */    
    //if(__RT_S_ISREG(dir_stat.st_mode)) 
    if(CRTL_S_ISREG(dir_stat.st_mode))   
    {       
        remove(dir);    
    }   
    else if(CRTL_S_ISDIR(dir_stat.st_mode))  
    {       
        dirp = opendir(dir);        
        while((dp = readdir(dirp))!= NULL)      
        {           
            if((0 == strcmp(CRTL_CURRENT_DIR, dp->d_name)) ||               
                (0 == strcmp(CRTL_FATHER_DIR, dp->d_name)) )          
            {               
                continue;           
            }           
            sprintf(dir_name, "%s/%s", dir, dp->d_name);            
            crtl_rmdir(dir_name);       
        }       
        closedir(dirp);     
        rmdir(dir); 
    }   
    else    
    {       
        crtl_print_err("Unknow file type!\n");    
    }
    return CRTL_SUCCESS;
}

/* file is directory */
int crtl_is_directory(const char *file)
{
    if(file == NULL)
    {
        crtl_print_err("null pointer error.\n");
        return CRTL_ERROR;
    }

    struct stat dir_stat;   
    
    /**
     *  If directory not exist, return 0.
     */ 
    if(0 != access(file, F_OK))  
    {       
        return CRTL_ERROR;   
    }   
    /**
     * Get attribution error
     */   
    if(0 != stat(file, &dir_stat))    
    {       
        crtl_print_err("Get directory stat error.\n");        
        return CRTL_ERROR;  
    }
    
    if(CRTL_S_ISDIR(dir_stat.st_mode))  
        return CRTL_SUCCESS;
    else 
        return CRTL_ERROR;
}

/* file is regular file */
int crtl_is_regular_file(const char *file)
{
    if(file == NULL)
    {
        crtl_print_err("null pointer error.\n");
        return CRTL_ERROR;
    }

    struct stat dir_stat;   
    
    /**
     *  If directory not exist, return 0.
     */ 
    if(0 != access(file, F_OK))  
    {       
        return CRTL_ERROR;   
    }   
    /**
     * Get attribution error
     */   
    if(0 != stat(file, &dir_stat))    
    {       
        crtl_print_err("Get directory stat error.\n");        
        return CRTL_ERROR;  
    }
    
    if(CRTL_S_ISREG(dir_stat.st_mode))  
        return CRTL_SUCCESS;
    else 
        return CRTL_ERROR;
}



char *crtl_mktemp_string(char * const tempfile_out, const char *path, const char *fileprefix) 
{
    if(!tempfile_out)
    {
        crtl_print_err("null pointer error.\n");
        return NULL;
    }
    tempfile_out[0] = '\0';
    
	/* char buffer[L_tmpnam]; */
	char template_name[BUFSIZ]={0};
    sprintf(template_name, "/tmp/%sXXXXXX", fileprefix?fileprefix:"tmp.");
    
	int _unused temp_fd;
	
	temp_fd=mkstemp(template_name);
    /* [ak] tmpnam is considered unsafe */
    
    char *tmp = NULL;
    if(path)
    {
	    tmp = strrchr(template_name, '/');
        tmp++;
	    strcpy(tempfile_out, path);
    }
    else
        tmp = template_name;

//    __crtl_dbg("Mk %s\n", template_name);

    /* 此处需要关闭文件描述符 */
    close(temp_fd);
    unlink(template_name);
    
	return strcat(tempfile_out, tmp);
}

#ifndef _SYS_STAT_H
struct stat {
    mode_t     st_mode;       //文件对应的模式，文件，目录等
    ino_t      st_ino;       //inode节点号
    dev_t      st_dev;        //设备号码
    dev_t      st_rdev;       //特殊设备号码
    nlink_t    st_nlink;      //文件的连接数
    uid_t      st_uid;        //文件所有者
    gid_t      st_gid;        //文件所有者对应的组
    off_t      st_size;       //普通文件，对应的文件字节数
    time_t     st_atime;      //文件最后被访问的时间
    time_t     st_mtime;      //文件内容最后被修改的时间
    time_t     st_ctime;      //文件状态改变时间
    blksize_t  st_blksize;    //文件内容对应的块大小
    blkcnt_t   st_blocks;     //伟建内容对应的块数量
};
#endif //_SYS_STAT_H

//
//
//int crtl_chk_fd(int fd)
//{  
//
//    struct stat _stat;  
//    int ret = CRTL_ERROR;  
//    if(!fcntl(fd, F_GETFL)) {  
//        crtl_print_err("File was deleted!\n");  
//        if(!fstat(fd, &_stat)) {  
//            if(_stat.st_nlink >= 1)  /* 文件连接数 */
//                ret = CRTL_SUCCESS;  
//            else  
//                crtl_print_err("File was deleted!\n");  
//        }  
//    }
//    
//    return ret;  
//} 



FILE *crtl_efopen(const char *file, const char *mode)
{
	FILE *stream;

	if (NULL == (stream = fopen(file, mode)))
		crtl_print_err("efopen: fopen failed\n");
	
	return stream;
}


FILE *crtl_efreopen(const char *file, const char *mode, FILE *stream1)
{
	FILE *stream2;

	if (NULL == (stream2 = freopen(file, mode, stream1)))
			crtl_print_err("efreopen: freopen failed\n");
	
	return stream2;
}

FILE *crtl_efdopen(int fd, const char *mode)
{
	FILE *stream;

	if (NULL == (stream = (FILE *) fdopen(fd, mode)))
		      crtl_print_err("efdopen: fdopen failed\n");
	
	return stream;
}


FILE *crtl_epopen(char *command, char *type)
{
	FILE *stream;

	if (NULL == (stream = (FILE *) popen(command, type)))
		      crtl_print_err("epopen: popen failed\n");
	
	return stream;
}


int crtl_efclose(FILE *stream)
{
	int status;

	if (EOF == (status = fclose(stream)))
		      crtl_print_err("efclose: fclose failed\n");

	return status;
}


int crtl_epclose(FILE *stream)
{
	int status;

	if (EOF == (status = pclose(stream)))
		      crtl_print_err("epclose: pclose failed\n");

	return status;
}

int crtl_efflush(FILE *stream)
{
	int status;

	if (EOF == (status = fflush(stream)))
		      crtl_print_err("efflush: fflush failed\n");

	return status;
}


int crtl_eremove(const char *file)
{
	int status;

	if ((status = remove(file)))
		crtl_print_err("eremove: remove failed\n");

	return status;
}


int crtl_erename(const char *oldfile, const char *newfile)
{
	int status;

	if ((status = rename(oldfile, newfile)))
		crtl_print_err("erename: rename failed\n");

	return status;
}


int crtl_efseek(FILE *stream, off_t offset, int origin)
{
	if (fseek(stream, offset, origin))  /* non-zero => error */
		      crtl_print_err("efseek: fseek failed\n");

	return 0;
}



void crtl_erewind(FILE *stream)	/* dummy function */
{
	rewind(stream);
	return;
}


long crtl_eftell(FILE *stream)
{
	long position;

	if (-1L == (position = ftell(stream)))
		crtl_print_err("eftell: ftell failed\n");

	return position;
}

int crtl_efseeko(FILE *stream, off_t offset, int origin)
{

	/* non-zero => error */
	if (fseeko(stream, (off_t) offset, (int) origin))
		crtl_print_err("efseeko: fseeko failed\n");

	return 0;
}

off_t crtl_eftello(FILE *streem)
{
	off_t eposition;
	off_t test=-1;

	eposition = ftello(streem);
	if (test == eposition) {
		fprintf(stderr,"sizeof(off_t)=%lu\n",
				(unsigned long) sizeof(eposition));
	}
	

	return eposition;
}


//FILE * crtl_etmpstream (char * prefix) 
//{
//   	FILE * stream;
//
//   	if (NULL == (stream = temporary_stream(prefix)))
//		crtl_print_err("etmpstream: temporary_stream failed\n");
//
//   	return stream;
//}

FILE *crtl_etmpfile(void)
{
	FILE *stream;

	if (NULL == (stream = tmpfile()))
		      crtl_print_err("etmpfile: tmpfile failed\n");
	
	return stream;
}


void *crtl_emalloc(size_t size)
{
	void *memptr;

	if (NULL == (memptr = malloc(size)))
		crtl_print_err("emalloc: malloc failed\n");
	
	return memptr;
}



void *crtl_erealloc(void *memptr, size_t size)
{
	void *newptr;

	if (NULL == (newptr = realloc(memptr, size)))
		crtl_print_err("erealloc: realloc failed\n");
	
	return newptr;
}


void *crtl_ecalloc(size_t count, size_t size)
{
	void *memptr;

	if (NULL == (memptr = calloc(count, size)))
		crtl_print_err("ecalloc: calloc failed\n");
	
	return memptr;
}

/* fgetpos and fsetpos may not exist on some systems */
/* if you get error messages about these then comment out the next two */
/* subroutine definitions */
/* beginning of fgetpos  and fsetpos block */


int crtl_efgetpos(FILE *stream, fpos_t *position)
{
	int status;

	if ((status = fgetpos(stream, position)))
		crtl_print_err("efgetpos: fgetpos failed\n");

	return status;
}


int crtl_efsetpos(FILE *stream, const fpos_t *position)
{
	int status;

	if ((status = fsetpos(stream, position)))
		crtl_print_err("efsetpos: fsetpos failed\n");

	return status;
}


size_t crtl_efread(void *bufptr, size_t size, size_t count, FILE *stream)
{
	size_t nread;

	if (!size) {
        crtl_print_err("efread: fread given 0 item size\n");
        return 0;
    }

	nread = fread(bufptr, size, count, stream);

	if (nread != count && ferror(stream))
		      crtl_print_err("efread: fread only %d items of %d\n",
				nread, count);

	return nread;
}


size_t crtl_efwrite(void *bufptr, size_t size, size_t count, FILE *stream)
{
	size_t nwrite;

	nwrite = fwrite(bufptr, size, count, stream);

	if (nwrite != count)
		      crtl_print_err("efwrite: fwrite only %d items of %d\n",
				nwrite, count);

	return nwrite;
}

