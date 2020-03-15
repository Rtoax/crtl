#include <malloc.h>

#include "crtl/crtl_file.h"
#include "crtl/crtl_log.h"
#include "crtl/easy/attribute.h"



/* create dir */
int crtl_mkdir(const char *dir, mode_t mode)
{
    if(dir == NULL || strlen(dir) <= 0)
    {
        crtl_print_err("null pointer error.\n");
        return -1;
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
            return -1;
        }
        closedir(dirp);
        return 0;   
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
    
    return 0;
}

/* remove dir */
int crtl_rmdir(const char *dir)
{
    if(dir == NULL)
    {
        crtl_print_err("null pointer error.\n");
        return -1;
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
        return -1;   
    }   
    /**
     * Get attribution error
     */   
    if(0 != stat(dir, &dir_stat))    
    {       
        crtl_print_err("Get directory stat error.\n");        
        return -1;  
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
    return 0;
}

/* file is directory */
int crtl_is_directory(const char *file)
{
    if(file == NULL)
    {
        crtl_print_err("null pointer error.\n");
        return -1;
    }

    struct stat dir_stat;   
    
    /**
     *  If directory not exist, return 0.
     */ 
    if(0 != access(file, F_OK))  
    {       
        return -1;   
    }   
    /**
     * Get attribution error
     */   
    if(0 != stat(file, &dir_stat))    
    {       
        crtl_print_err("Get directory stat error.\n");        
        return -1;  
    }
    
    if(CRTL_S_ISDIR(dir_stat.st_mode))  
        return 0;
    else 
        return -1;
}

/* file is regular file */
int crtl_is_regular_file(const char *file)
{
    if(file == NULL)
    {
        crtl_print_err("null pointer error.\n");
        return -1;
    }

    struct stat dir_stat;   
    
    /**
     *  If directory not exist, return 0.
     */ 
    if(0 != access(file, F_OK))  
    {       
        return -1;   
    }   
    /**
     * Get attribution error
     */   
    if(0 != stat(file, &dir_stat))    
    {       
        crtl_print_err("Get directory stat error.\n");        
        return -1;  
    }
    
    if(CRTL_S_ISREG(dir_stat.st_mode))  
        return 0;
    else 
        return -1;
}



char *crtl_mktempfile(char * const tempfile_out, const char *path, const char *fileprefix) 
{
    if(!tempfile_out)
    {
        crtl_print_err("null pointer error.\n");
        return NULL;
    }

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
	    strcpy(tempfile_out, path);
    }
    else
        tmp = template_name;
    
	return strcat(tempfile_out, tmp);
}

