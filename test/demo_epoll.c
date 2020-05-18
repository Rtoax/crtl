
#include "crtl/bits/crtl_epoll.h"
#include "crtl/crtl_log.h"

#define SIZE 10

void demo_epoll_test1()
{
    int i, nfds;
    int epfd = crtl_epoll_create(SIZE, CRTL_EPOLL_CLOEXEC);

    /*声明epoll_event结构体变量，ev用于注册事件，数组用于处理要回传的事件*/
	struct epoll_event ev, events[SIZE];

	/*设置监听描述符*/
	ev.data.fd = 0;

	/*设置处理事件类型*/	
	ev.events = CRTL_EPOLLIN | CRTL_EPOLLET;
    
	/*注册事件*/
	crtl_epoll_ctl(epfd, CRTL_EPOLL_CTL_ADD, 1, &ev);

    /*4.进入服务器接收请求死循环*/
	while(1)
	{
		/*等待事情发生*/
		nfds = crtl_epoll_wait(epfd, events, SIZE, 2, NULL);
		if(nfds <= 0) {
			continue;
		}

//		printf("nfds = %d\n", nfds);
        
		/*处理发生的事件*/
		for(i=0; i<nfds; i++) {
            printf("nfds = %d, events = %x, event.data.fd = %d\n", nfds,events[i].events, events[i].data.fd);

        }
    }
}


int main()
{
    demo_epoll_test1();

    return 0;
}

