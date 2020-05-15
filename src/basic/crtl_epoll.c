#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "crtl/bits/crtl_epoll.h"
#include "crtl/easy/attribute.h"
#include "crtl/easy/macro.h"
#include "crtl/crtl_assert.h"
#include "crtl/crtl_log.h"

/**
 *  Creates an epoll instance.  Returns an fd for the new instance.
 * The "size" parameter is a hint specifying the number of file
 * descriptors to be associated with the new instance.  The fd
 * returned by crtl_epoll_create() should be closed with crtl_epoll_close().
 *
 * @param size: Since Linux 2.6.8, the size argument is ignored, but must
 *              be greater than zero; see NOTES below.
 * @param flag: If flags is 0, then, other than the fact that the obsolete size argument is dropped,  
 *              epoll_create1()  is the same as epoll_create().  
 *              The following value can be included in flags to obtain different behavior:
 *              EPOLL_CLOEXEC   Set  the  close-on-exec  (FD_CLOEXEC) flag on the new file descriptor.  
 *                              See the description of the O_CLOEXEC flag in open(2) for reasons why this may be useful.
 * @return epoll instance number
 */
int crtl_epoll_create(int size, int flag)
{
    if(unlikely(size<=0)) {
        crtl_assert(size);
        return CRTL_ERROR;
    }
    int epfd;
//    EPOLL_CLOEXEC
    /*生成用于处理accept的epoll专用文件描述符*/
    if(size || flag == 0) {
	    epfd = epoll_create(size);
    } else {
        if(flag != EPOLL_CLOEXEC) {
            crtl_print_warning("flag != EPOLL_CLOEXEC\n");
        }
        epfd = epoll_create1(flag);
    }
    
    return epfd;
}


/**
 * Manipulate an epoll instance "epfd". Returns 0 in case of success,
 * -1 in case of error ( the "errno" variable will contain the
 * specific error code ) The "op" parameter is one of the EPOLL_CTL_*
 * constants defined above. The "fd" parameter is the target of the
 * operation. The "event" parameter describes which events the caller
 * is interested in and any associated user data.  
 *
 * @param epfd: epoll instance, create by crtl_epoll_create
 * @param operate: Valid opcodes ( "op" parameter ) to issue to crtl_epoll_ctl().
 *                  EPOLL_CTL_ADD   Add a file descriptor to the interface.
 *                  EPOLL_CTL_DEL   Remove a file descriptor from the interface.
 *                  EPOLL_CTL_MOD   Change file descriptor epoll_event structure.
 * @param fd: It requests that the operation operate be performed for the target file descriptor, fd.
 * @param event: The event argument describes the object linked to the file descriptor  fd.   
 *              The  struct  epoll_event  is defined as :
 *
 *              typedef union epoll_data {
 *                void        *ptr;
 *                int          fd;
 *                uint32_t     u32;
 *                uint64_t     u64;
 *              } epoll_data_t;
 *
 *              struct epoll_event {
 *                uint32_t     events;      // Epoll events 
 *                epoll_data_t data;        // User data variable 
 *              };
 *              events is one of "EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLPRI|EPOLLERR|EPOLLHUP|EPOLLET|EPOLLONESHOT"
 *
 * @return error number
 */
int crtl_epoll_ctl(int epfd, int operate, int fd, struct epoll_event *event)
{
    if(unlikely(!event)) {
        crtl_print_err("crtl_epoll_ctl null pointer error.\n");
        crtl_assert(event);
        return CRTL_ERROR;
    }
    if(operate != EPOLL_CTL_ADD && operate != EPOLL_CTL_DEL && operate != EPOLL_CTL_MOD) {
        crtl_print_err("crtl_epoll_ctl error. Not support operate\n");
        return CRTL_ERROR;
    }
    
    if(!(event->events&(EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLPRI|EPOLLERR|EPOLLHUP|EPOLLET|EPOLLONESHOT))) {
        crtl_print_err("crtl_epoll_ctl error. Not support event->events.\n");
        return CRTL_ERROR;
    }
    if(0 != epoll_ctl(epfd, operate, fd, event)) {/*注册事件*/
        crtl_print_err("epoll_ctl error. %s\n", strerror(errno));
        return CRTL_ERROR;
    }
    return CRTL_SUCCESS;
}

	
/**
 * Wait for events on an epoll instance "epfd". Returns the number of
 * triggered events returned in "events" buffer. Or -1 in case of
 * error with the "errno" variable set to the specific error code. The
 * "events" parameter is a buffer that will contain triggered
 * events. The "maxevents" is the maximum number of events to be
 * returned ( usually size of "events" ). The "timeout" parameter
 * specifies the maximum wait time in milliseconds (-1 == infinite).
 *
 * @param epfd: epoll instance, create by crtl_epoll_create
 * @param events: The memory area pointed to by events will contain 
 *                  the events that will be available for the  caller.
 * @param maxevents: Up to maxevents are returned by epoll_wait().
 * @param timeout: The  timeout  argument  specifies the minimum number of 
 *                  milliseconds that epoll_wait() will block.
 * @param sigmask: Same as epoll_wait, but the thread's signal mask is temporarily
 *                   and atomically replaced with the one provided as parameter.
 *
 * @return error number
 */
int crtl_epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout, const sigset_t *sigmask)
{
    if(unlikely(!events) || unlikely(!maxevents) || unlikely(!epfd)) {
        crtl_print_err("crtl_epoll_wait error. Invalid argument.\n");
        crtl_assert(0);
        return CRTL_ERROR;
    }
    int ret = -1;
    
    if(sigmask) {
        ret = epoll_pwait(epfd, events, maxevents, timeout, sigmask);
    } else {
        ret = epoll_wait(epfd, events, maxevents, timeout);        
    }
    
    if(ret < 0) {
        crtl_print_err("epoll_wait/epoll_pwait error. %s\n", strerror(errno));
        return CRTL_ERROR;
    }
    return ret;
}




/**
 *  Close an epoll instance.  
 *
 * @param epfd: epoll instance.
 * @return
 */
int crtl_epoll_close(int epfd)
{
    return close(epfd);
}
