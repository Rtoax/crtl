#include <errno.h>

#include "crtl/crtl_task.h"
#include "crtl/crtl_log.h"

#include "crtl/tcp-ip/crtl_socket.h"


#define TCP_PORT    6021


/**********************************************************************************************************************/
static void *tcp_server(void*arg)
{
    sockaddr_in_t srvaddr, cliaddr;
    int listenfd = crtl_socket_server_tcp(&srvaddr, TCP_PORT, 20);
    while(1)
    {
        socklen_t len;
        int connfd = crtl_socket_accept_tcp(listenfd, (sockaddr_t*)&cliaddr, &len);
        int child_pid;
        char __msg[256] = {0};
        
        while(1)
        {
            int len = crtl_socket_read_tcp(connfd, __msg, sizeof(__msg));
            if(len <= 0)
                break;
            crtl_print_notice("server: recv msg: %s\n", __msg);
            crtl_socket_write_tcp(connfd, __msg, sizeof(__msg));
        }
        crtl_socket_close(connfd);

    }

    crtl_socket_close(listenfd);
    
}

static void *tcp_client(void*arg)
{
    sockaddr_in_t srvaddr, cliaddr;
    int sockfd = crtl_socket_client_tcp(&srvaddr, "127.0.0.1", TCP_PORT);
    while(1)
    {
        char __msg[256] = {"Rong Tao, Hello Wrold.\n"};

        sleep(1);
        
        crtl_socket_write_tcp(sockfd, __msg, sizeof(__msg));
        
        int len = crtl_socket_read_tcp(sockfd, __msg, sizeof(__msg));
        crtl_print_notice("client: recv msg: %s\n", __msg);
    }
}


static void demo_tcp_ip_socket_tcp_test1()
{
    crtl_thread_t thread, thread2;
    crtl_thread_normal(&thread, tcp_server, NULL);
    sleep(1);
    crtl_thread_normal(&thread2, tcp_client, NULL);

    while(1)
    {
        sleep(1);
    }
    crtl_thread_cancel(thread);
    crtl_thread_cancel(thread2);
}


/**********************************************************************************************************************/
static void *udp_server(void*arg)
{
    sockaddr_in_t srvaddr, cliaddr;

    int server_sockfd = crtl_socket_server_udp(&srvaddr, TCP_PORT);
    crtl_print_info("sockfd = %d.\n", server_sockfd);
    
//    socklen_t len;
    
    char __msg[256] = {0};
    
    while(1)
    {
        crtl_print_info("recvfrom.\n");
        int len = 0;
        len = crtl_socket_udp_recvfrom(server_sockfd, __msg, sizeof(__msg), 0, (sockaddr_t*)&cliaddr);
        crtl_print_info("recvfrom. len = %d\n", len);
        if(len <= 0)
            break;
        
        crtl_print_notice("server: recv msg: %s\n", __msg);
        len = crtl_socket_udp_sendto(server_sockfd, __msg, sizeof(__msg), 0, (sockaddr_t*)&cliaddr);
        crtl_print_info("sendto. len = %d\n", len);
    }
    crtl_socket_close(server_sockfd);
    crtl_print_info("close.\n");
    
}




static void *udp_client(void*arg)
{
    sockaddr_in_t srvaddr, cliaddr;
    
    int client_sockfd = crtl_socket_client_udp(&srvaddr, "127.0.0.1", TCP_PORT);
    crtl_print_info("sockfd = %d. %d. %d\n", client_sockfd, sizeof(sockaddr_in_t), sizeof(sockaddr_t));

    while(1)
    {
        char __msg[256] = {"Rong Tao, Hello Wrold.\n"};

        crtl_print_info("sendto.\n");
        int len = 0;
        sleep(1);
        len = crtl_socket_udp_sendto(client_sockfd, __msg, strlen(__msg), 0, (sockaddr_t*)&srvaddr);
        crtl_print_info("sendto. len = %d, %s\n", len, strerror(errno));
        
        len = crtl_socket_udp_recvfrom(client_sockfd, __msg, sizeof(__msg), 0, (sockaddr_t*)&cliaddr);
        crtl_print_info("recvfrom. len = %d\n", len);
        crtl_print_notice("client: recv msg: %s\n", __msg);
    }
    
    crtl_socket_close(client_sockfd);
    crtl_print_info("close.\n");
}


static void demo_tcp_ip_socket_udp_test1()
{
    crtl_print_info("\n");
    
    crtl_thread_t thread, thread2;
    crtl_thread_normal(&thread, udp_server, NULL);
    sleep(1);
    crtl_thread_normal(&thread2, udp_client, NULL);

    while(1)
    {
        sleep(1);
    }
    crtl_thread_cancel(thread);
    crtl_thread_cancel(thread2);
}



/**********************************************************************************************************************/
#define UNIX_PATH   "./unixsocket"

static void *unix_server(void*arg)
{
    sockaddr_un_t srvaddr, cliaddr;
    int listenfd = crtl_socket_server_unix(&srvaddr, UNIX_PATH, 20);
    while(1)
    {
        socklen_t len;
        int connfd = crtl_socket_accept_unix(listenfd, (sockaddr_t*)&cliaddr, &len);
        int child_pid;
        char __msg[256] = {0};
        
        while(1)
        {
            int len = crtl_socket_read_unix(connfd, __msg, sizeof(__msg));
            if(len <= 0)
                break;
            crtl_print_notice("server: recv msg: %s\n", __msg);
            crtl_socket_write_unix(connfd, __msg, sizeof(__msg));
        }
        crtl_socket_close(connfd);

    }

    crtl_socket_close(listenfd);
    
}

static void *unix_client(void*arg)
{
    sockaddr_un_t srvaddr, cliaddr;
    int sockfd = crtl_socket_client_unix(&srvaddr, UNIX_PATH);
    while(1)
    {
        char __msg[256] = {"Rong Tao, Hello Wrold.\n"};

        sleep(1);
        
        crtl_socket_write_unix(sockfd, __msg, sizeof(__msg));
        
        int len = crtl_socket_read_unix(sockfd, __msg, sizeof(__msg));
        crtl_print_notice("client: recv msg: %s\n", __msg);
    }
}


static void demo_tcp_ip_socket_unix_test1()
{
    crtl_thread_t thread, thread2;
    crtl_thread_normal(&thread, unix_server, NULL);
    sleep(1);
    crtl_thread_normal(&thread2, unix_client, NULL);

    while(1)
    {
        sleep(1);
    }
    crtl_thread_cancel(thread);
    crtl_thread_cancel(thread2);
}


/**********************************************************************************************************************/
int main()
{
//    demo_tcp_ip_socket_tcp_test1();
//    demo_tcp_ip_socket_udp_test1();
    demo_tcp_ip_socket_unix_test1();
    
    return 0;
}

