/*************************************************************************
	> File Name: epoll_client.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年08月24日 星期五 10时15分15秒
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

#define MAX_EPOLLSIZE 2*1
#define SERVER_SIZE 1
#define CLIENT_SIZE 10
static int SetNonblock(int fd) {
	int flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0) return flags;
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0) return -1;
	return 0;
}

static int SetReUseAddr(int fd) {
	int reuse = 1;
	return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
}

int main(int argc, char *argv[])
{
	int epollfd;
	char ip[64]="127.0.0.1";
	int port = 8080;
	char buffer[256]={0};
	struct epoll_event events[MAX_EPOLLSIZE];

	if(argc ==3)
	{
		strcpy(ip,argv[1]);
		port = atoi(argv[2]);
	}

	struct sockaddr_in addr;
	memset(&addr,0x0,sizeof(struct sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr =inet_addr(ip);


	epollfd = epoll_create(MAX_EPOLLSIZE);
	if (-1 == epollfd)
	{
		perror("epoll_create failed\n");
		exit(0);
	}
	

	int icnt = 0;
	while (1)
	{
		for (int i = 0; i < SERVER_SIZE; i++)
		{
			if (icnt >= MAX_EPOLLSIZE / SERVER_SIZE )
				break;
			int sockfd = socket(AF_INET,SOCK_STREAM,0);
			if (sockfd == -1)
			{
				perror("socket failed \n");
				exit(0);
			}
			
			addr.sin_port = htons(port+i);

			if(connect(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in)) <0 )
			{
				perror("connect failed\n");
				exit(0);
			}

			sprintf(buffer,"send construction from %d",sockfd);
			printf("send construction from %d\n",sockfd);
			send(sockfd,buffer,sizeof(buffer),0);

			SetNonblock(sockfd);
			SetReUseAddr(sockfd);

			struct epoll_event ev;
			ev.data.fd = sockfd;
			ev.events = EPOLLIN |EPOLLOUT;
			epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);
			icnt ++;
		}
	

		int nfds = epoll_wait(epollfd,events,icnt,100);
		if(nfds <0)
		{
			perror("epoll_wait\n");
			exit(0);
		}

		for (int i = 0;i< nfds;i++)
		{
			memset(buffer,0,sizeof(buffer));
			int clientfd = events[i].data.fd;

			if(events[i].events & EPOLLIN)
			{
				int len = recv(clientfd,buffer,sizeof(buffer),0);
				if(len >0)
				{
					printf("recv from %d [%s]\n",clientfd,buffer);
				}
				else if(len == 0)
				{
					printf("disconnect %d\n",clientfd);
					icnt --;
					epoll_ctl(epollfd,EPOLL_CTL_DEL,clientfd,NULL);
					close(clientfd);
				}
				else
				{
					if (errno  == EINTR) continue;

					printf(" Error clientfd:%d, errno:%d\n", clientfd, errno);
					close(clientfd);
				}
			}
			else if (events[i].events & EPOLLOUT)
			{
				sprintf(buffer,"send from %d",clientfd);
				send(clientfd,buffer,sizeof(buffer),0);
			}


		}
		usleep(1000);

	}
}
