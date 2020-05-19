#ifndef __CRTL_BITS_EPOLL_H
#define __CRTL_BITS_EPOLL_H 1

#include <sys/epoll.h>
#include "crtl/bits/types_basic.h"


#define CRTL_EPOLL_CLOEXEC EPOLL_CLOEXEC

/**
 *  struct epoll_event's events
 *  
 *  ```c example
 *  struct epoll_event ev;
 *  ev.data.fd = 0; //设置监听描述符
 *  ev.events = CRTL_EPOLLIN | CRTL_EPOLLET;//设置处理事件类型
 *  crtl_epoll_ctl(epfd, CRTL_EPOLL_CTL_ADD, 1, &ev);//注册事件
 *  ```
 */
#define CRTL_EPOLLIN EPOLLIN    //The associated file is available for read(2) operations.
#define CRTL_EPOLLPRI EPOLLPRI  //There is urgent data available for read(2) operations.
#define CRTL_EPOLLOUT EPOLLOUT  //The associated file is available for write(2) operations.
#define CRTL_EPOLLRDNORM EPOLLRDNORM
#define CRTL_EPOLLRDBAND EPOLLRDBAND
#define CRTL_EPOLLWRNORM EPOLLWRNORM
#define CRTL_EPOLLWRBAND EPOLLWRBAND
#define CRTL_EPOLLMSG EPOLLMSG
#define CRTL_EPOLLERR EPOLLERR  //Error  condition happened on the associated file descriptor.  
                                //epoll_wait(2) will always wait for this event; it is
                                //not necessary to set it in events.
#define CRTL_EPOLLHUP EPOLLHUP  //Hang up happened on the associated file descriptor.  
                                //epoll_wait(2) will always wait for this event; it is not nec‐
                                //essary to set it in events.
#define CRTL_EPOLLRDHUP EPOLLRDHUP  //Stream socket peer closed connection, or shut down writing half of connection.  
                                    //(This flag  is  especially  useful for writing simple code to detect peer 
                                    //shutdown when using Edge Triggered monitoring.)
#define CRTL_EPOLLEXCLUSIVE EPOLLEXCLUSIVE
#define CRTL_EPOLLWAKEUP EPOLLWAKEUP
#define CRTL_EPOLLONESHOT EPOLLONESHOT  //EPOLLONESHOT (since Linux 2.6.2)
                                        //Sets  the one-shot behavior for the associated file descriptor.  
                                        //This means that after an event is pulled out with
                                        //epoll_wait(2) the associated file descriptor is internally disabled 
                                        //and no other events will be  reported  by  the
                                        //epoll  interface.  The user must call epoll_ctl() with EPOLL_CTL_MOD 
                                        //to rearm the file descriptor with a new event mask.
#define CRTL_EPOLLET EPOLLET    //Sets  the  Edge  Triggered  behavior  for the associated file descriptor.  
                                //The default behavior for epoll is Level Triggered.  See epoll(7) for more 
                                //detailed information about Edge and Level Triggered event distribution architectures.


#define CRTL_EPOLL_CTL_ADD EPOLL_CTL_ADD	/* Add a file descriptor to the interface.  */
#define CRTL_EPOLL_CTL_DEL EPOLL_CTL_DEL	/* Remove a file descriptor from the interface.  */
#define CRTL_EPOLL_CTL_MOD EPOLL_CTL_MOD	/* Change file descriptor epoll_event structure.  */



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
int crtl_epoll_create(int size, int flag);



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
 * @return epoll instance number
 */
int crtl_epoll_ctl(int epfd, int operate, int fd, struct epoll_event *event);




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
 *                The  data  of  each  returned  structure  will  contain  
 *                  the   same   data   the   user   set   with   an   epoll_ctl(2)
*                   (EPOLL_CTL_ADD,EPOLL_CTL_MOD) while the events member 
*                   will contain the returned event bit field.
 * @param maxevents: Up to maxevents are returned by epoll_wait().
 * @param timeout: The  timeout  argument  specifies the minimum number of 
 *                  milliseconds that epoll_wait() will block.
 * @param sigmask: Same as epoll_wait, but the thread's signal mask is temporarily
 *                   and atomically replaced with the one provided as parameter.
 *
 * @return error number
 */
int crtl_epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout, const sigset_t *sigmask);



/**
 *  Close an epoll instance.  
 *
 * @param epfd: epoll instance.
 * @return
 */
int crtl_epoll_close(int epfd);



#endif /*<__CRTL_BITS_EPOLL_H>*/
