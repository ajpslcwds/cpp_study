#include "common.h"

#define MAX_EVENT_NUMBER 1024


int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option =  old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

void addfd(int epollfd, int fd)
{
	epoll_event  epevent;
	epevent.data.fd = fd;
	epevent.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &epevent);
	setnonblocking(fd);
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf(" usage: %s  ip port\n", basename(argv[0]));
		return -1;
	}
	int ret = 0;
	char *ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, ip, &address.sin_addr);

	int tcpfd =  socket(PF_INET, SOCK_STREAM, 0);
	assert(tcpfd > 0);

	ret =  bind(tcpfd, (struct sockaddr *)&address, sizeof(address));
	assert(ret != -1);

	ret =  listen(tcpfd, 5);
	assert(ret != -1);

	//udp
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, ip, &address.sin_addr);

	int udpfd =  socket(PF_INET, SOCK_DGRAM, 0);
	assert(udpfd > 0);

	ret =  bind(udpfd, (struct sockaddr *)&address, sizeof(address));
	assert(ret != -1);

	int epollfd = epoll_create(5);
	assert(epollfd != -1);

	addfd(epollfd, tcpfd);
	addfd(epollfd, udpfd);

	epoll_event events[MAX_EVENT_NUMBER];
	while (1)
	{
		int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
		if (number < 0)
		{
			printf("epoll failure\n");
			break;
		}

		for (int i  = 0; i < number; i++)
		{
			int sockfd =  events[i].data.fd;
			if (sockfd ==  tcpfd)
			{
				struct sockaddr_in client_address;
				socklen_t  client_addrlength = sizeof(client_address);
				int connfd = accept(tcpfd, (struct sockaddr *)&client_address, &client_addrlength);
				addfd(epollfd, connfd);
			}
			else if (sockfd == udpfd)
			{
				char buf[BUFFER_SIZE_MAX];
				memset(buf, 0, BUFFER_SIZE_MAX);

				struct sockaddr_in client_address;
				socklen_t  client_addrlength = sizeof(client_address);

				ret = recvfrom(udpfd, buf, BUFFER_SIZE_MAX - 1, 0,
							   (struct sockaddr *)&client_address, &client_addrlength);
				if (ret > 0)
				{
					sendto(udpfd, buf, BUFFER_SIZE_MAX - 1, 0,
							 (struct sockaddr *)&client_address, client_addrlength);

				}
			}
			else if (events[i].events & EPOLLIN)
			{
				char buf[BUFFER_SIZE_MAX];
				while (1)
				{
					memset(buf, 0, BUFFER_SIZE_MAX);
					ret  = recv(sockfd, buf, BUFFER_SIZE_MAX - 1, 0);
					if (ret < 0)
					{
						if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
						{
							break;
						}
						close(sockfd);
						break;
					}
					else if (ret == 0)
					{
						close(sockfd);
					}
					else
					{
						printf("get %d bytes of client data %s from %d\n",ret,  buf, sockfd);
						send(sockfd, buf, ret, 0);
					}
				}
			}
			else
			{
				printf("something else happened\n");
			}
		}
	}


	close(tcpfd);
	close(udpfd);
	return 0;
}
