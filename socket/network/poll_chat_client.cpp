#include "common.h"


int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf(" usage: %s  ip port\n", basename(argv[0]));
		return -1;
	}

	char *ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in address;
	bzero(&address,sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, ip, &address.sin_addr);

	int sockfd =  socket(PF_INET, SOCK_STREAM, 0);
	assert(sockfd > 0);

	if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		printf("connect error!\n");
		close(sockfd);
		return -1;
	}

	pollfd  fds[2];
	fds[0].fd = 0;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	fds[1].fd = sockfd;
	fds[1].events = POLLIN | POLLRDHUP;
	fds[1].revents = 0;

	char read_buf[BUFFER_SIZE_MAX];
	int pipefd[2];
	int ret = pipe(pipefd);
	assert(ret != -1);

	while (1)
	{
		ret =  poll(fds, 2, -1);
		if (ret < 0)
		{
			printf("poll failure\n");
			break;
		}

		if (fds[1].revents & POLLRDHUP)
		{
			printf("server close the connection\n");
			break;
		}
		else if (fds[1].revents & POLLIN)
		{
			memset(read_buf, 0, BUFFER_SIZE_MAX);
			recv(fds[1].fd, read_buf, BUFFER_SIZE_MAX - 1, 0);
			printf("%s\n",read_buf);
		}

		if (fds[0].revents & POLLIN)
		{
			ret = splice(0, NULL, pipefd[1], NULL, 32768,
						  SPLICE_F_MORE | SPLICE_F_MOVE);
			ret = splice(pipefd[0], NULL, sockfd, NULL, 32768,
						 SPLICE_F_MORE | SPLICE_F_MOVE);
		}
	}

	close(sockfd);
	return 0;
}
