#include "crtl/bits/crtl_shm_posix.h"
#include "crtl/log.h"
#include "crtl/assert.h"
#include "crtl/easy/attribute.h"



/* POSIX */
/* POSIX **************************************************************************************************************/
_api int crtl_posix_shm_open(const char *name, int size, int oflag, int mode)
{
    if(unlikely(!name) || unlikely(!size))
    {
        crtl_print_err("null pointer error.\n");
        crtl_assert(0);
        return CRTL_ERROR;
    }
    int fd, ret;
    if((fd = shm_open(name, oflag, mode))<=0)
    {
        crtl_print_err("shm_open %s error.\n", name);
        crtl_assert(0);
        return CRTL_ERROR;
    }
	if((ret = ftruncate(fd, size))!=0)
    {
        crtl_print_err("ftruncate fd:%d to size:%d error.\n", fd, size);
        crtl_assert(0);
        shm_unlink(name);
        return CRTL_ERROR;
    }
    return fd;
}



_api void *crtl_posix_shm_mmap(int size, int prot, int flags, int fd)
{
    if(unlikely(!size) || unlikely(!fd))
    {
        crtl_print_err("out of range error.\n");
        crtl_assert(0);
        return NULL;
    }
    struct stat stat;
    fstat(fd, &stat);
	if(stat.st_size < size)
    {
        crtl_print_err("fstat: size:%d > stat.st_size:%d error.\n", size, (int)stat.st_size);
        crtl_assert(0);
        size = stat.st_size;
    }
    
    static void *ptr;
    ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED)
    {
        crtl_print_err("mmap error.\n");
        crtl_assert(0);
        return NULL;
    }
    
    return ptr;
}


_api int crtl_posix_shm_munmap(void *addr, int size)
{
    if(unlikely(!size) || unlikely(!addr))
    {
        crtl_print_err("out of range or null pointer error.\n");
        crtl_assert(0);
        return CRTL_ERROR;
    }
    int ret = munmap(addr, size);
    if(ret != 0)
    {
        crtl_print_err("munmap error, addr:%p, size:%d.\n", addr, size);
        crtl_assert(0);
        return CRTL_ERROR;
    }   
    return CRTL_SUCCESS;
}



_api int crtl_posix_shm_put(void *shm_addr, const void *data, int data_len)
{
    if(unlikely(!shm_addr) || unlikely(!data) || unlikely(!data_len))
    {
        crtl_print_err("out of range or null pointer error.\n");
        crtl_assert(0);
        return CRTL_ERROR;
    }
    if(!memcpy(shm_addr, data, data_len))
    {
        crtl_print_err("memcpy error.\n");
        crtl_assert(0);
        return CRTL_ERROR;
    }
    return data_len;
}

_api int crtl_posix_shm_get(void *shm_addr, void *data, int data_len)
{
    if(unlikely(!shm_addr) || unlikely(!data) || unlikely(!data_len))
    {
        crtl_print_err("out of range or null pointer error.\n");
        crtl_assert(0);
        return CRTL_ERROR;
    }
    if(!memcpy(data, shm_addr, data_len))
    {
        crtl_print_err("memcpy error.\n");
        crtl_assert(0);
        return CRTL_ERROR;
    }
    return data_len;
}



_api int crtl_posix_shm_unlink(const char *name)
{
    if(unlikely(!name) || strlen(name)<=0)
    {
        crtl_print_err("strlen out of range or null pointer error.\n");
        crtl_assert(0);
        return CRTL_ERROR;
    }
    int ret = shm_unlink(name);
    if(ret != 0)
    {
        crtl_print_err("shm_unlink error, name:%s.\n", name);
        crtl_assert(0);
        return CRTL_ERROR;
    }   
    return CRTL_SUCCESS;
}

