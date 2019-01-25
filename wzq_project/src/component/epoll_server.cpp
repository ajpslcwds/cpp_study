/*************************************************************************
	> File Name: epoll_server.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年08月24日 星期五 14时55分15秒
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


#define SERVER_SIZE 10
#define MAX_EPOLLSIZE 256 * SERVER_SIZE

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
	set<int> server_sockfd;

	if(argc ==3)
	{
		strcpy(ip,argv[1]);
		port = atoi(argv[2]);
	}

	struct sockaddr_in addr;
	memset(&addr,0x0,sizeof(struct sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; //inet_addr(ip);


	epollfd = epoll_create(MAX_EPOLLSIZE);
	if (-1 == epollfd)
	{
		perror("epoll_create failed\n");
		exit(0);
	}
	

	for (int i = 0; i < SERVER_SIZE; i++)
	{
		int sockfd = socket(AF_INET,SOCK_STREAM,0);
		if (sockfd == -1)
		{
			perror("socket failed \n");
			exit(0);
		}
		
		addr.sin_port = htons(port+i);

		if (bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in))<0)
		{
			perror("bind failed\n");
			exit(0);
		}

		if (listen(sockfd,10)<0)
		{
			perror("listen failed\n");
			exit(0);
		}
		printf("build socket server %d\n",sockfd);
		server_sockfd.insert(sockfd);
		SetNonblock(sockfd);

		struct epoll_event ev;
		ev.data.fd = sockfd;
		ev.events = EPOLLIN |EPOLLOUT;
		epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);
	}


	int icnt = 0;
	while (1)
	{
		int nfds = epoll_wait(epollfd,events,MAX_EPOLLSIZE,100);
		if(nfds <0)
		{
			perror("epoll_wait\n");
			exit(0);
		}
		printf("nfds: %d\n",nfds);
		for (int i = 0;i< nfds;i++)
		{
			memset(buffer,0,sizeof(buffer));
			int clientfd = events[i].data.fd;
			printf("clientfd:%d,events:0X%x\n",clientfd,events[i].events);	
			
			if(events[i].events & EPOLLIN)
			{
				//deal server for new client
				if (server_sockfd.find(clientfd)!=server_sockfd.end())
				{
					int new_clientfd ;						
					struct sockaddr_in addr;
					memset(&addr,0x0,sizeof(struct sockaddr_in));
					socklen_t socklen = sizeof(struct sockaddr_in);

					if((new_clientfd = accept(clientfd,(struct sockaddr*)&addr,&socklen))<0)
					{
						perror("accept failed\n");
						continue;
					}
					
					printf("%d get [%d] new client[%d],from %s:%d\n",clientfd,icnt,new_clientfd,inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
					send(new_clientfd,"connected",10,0);
					struct epoll_event ev;
					ev.data.fd = new_clientfd;
					ev.events = EPOLLIN |EPOLLOUT;
					epoll_ctl(epollfd,EPOLL_CTL_ADD,new_clientfd,&ev);
					icnt ++;
					continue;
				}

				//deal client for msg
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

					printf(" Error clientfd:%d, errno:%d,errmsg:%s\n", clientfd, errno, strerror(errno));
					close(clientfd);
				}
			}
			if (events[i].events & EPOLLOUT)
			{
				sprintf(buffer,"hello ,I am server %d",clientfd);
				send(clientfd,buffer,sizeof(buffer),0);
				printf("send to:%d[%s]\n",clientfd,buffer);
			}
		}

		usleep(1000);
	}
}
