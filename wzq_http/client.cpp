/*************************************************************************
	> File Name: client.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2019年01月16日 星期三 16时15分14秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <set>

using namespace std;

#define IP  "127.0.0.1"
#define PORT 8899
#define BUFFER_SIZE 1024
#define CONNECT_SIZE  32

static int SetNonblock(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0) return -1;
    return 0;
}

static int SetReUseAddr(int fd)
{
    int reuse = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
}


int Run()
{
    int epollfd;
    char buffer[BUFFER_SIZE] = { 0 };

    struct sockaddr_in addr;
    memset(&addr, 0x0, sizeof(struct sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(IP);
    addr.sin_port = htons(PORT);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket failed \n");
        exit(0);
    }

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("connect failed\n");
        exit(0);
    }
    SetNonblock(sockfd);


    epollfd = epoll_create(CONNECT_SIZE);
    if (-1 == epollfd)
    {
        perror("epoll_create failed\n");
        exit(0);
    }

    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLOUT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);



    int icnt = 0;
    struct epoll_event events[CONNECT_SIZE];
    while (1)
    {
        int nfds = epoll_wait(epollfd, events, CONNECT_SIZE, 1000);
        if (nfds < 0)
        {
            perror("epoll_wait\n");
            exit(0);
        }
        printf("nfds:%d\n",nfds);

        for (int i = 0; i < nfds; i++)
        {
            memset(buffer, 0, sizeof(buffer));
            int clientfd = events[i].data.fd;
			printf("fd:%d,events:0X%x\n",clientfd,events[i].events);
            if (events[i].events & EPOLLIN)
            {
                int len = recv(clientfd,buffer,BUFFER_SIZE,0);
                if (len<0)
                {
                    perror("recv");
                    exit(0);
                }

                printf("%d,%s\n",len,buffer);
            }
            else if (events[i].events & EPOLLOUT)
            {
                strcpy(buffer,"hello  server ,I'm a client");
                if (send(clientfd,buffer,BUFFER_SIZE,0)<0)
                {
                    perror("send");
                    exit(0);
                }
            }
        }

        usleep(1000);
    }
}




int main()
{
    Run();
    return 0;
}


