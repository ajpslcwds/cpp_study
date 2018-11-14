#include "common.h"

int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option =  old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

int non_block_connect(const char *ip, int port, int time)
{
	int iRet = 0;

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port =  htons(port);

	int iSocketId = socket(PF_INET, SOCK_STREAM, 0);
	assert(iSocketId > 0);
	int fdopt = setnonblocking(iSocketId);

	iRet = connect(iSocketId, (struct sockaddr *)&address, sizeof(address));
	if (iRet == 0)
	{
		printf("connect with server immediately\n");
		fcntl(iSocketId, F_SETFL, fdopt);
		return iSocketId;
	}
	else if (errno != EINPROGRESS)
	{
		printf("unblock connect not support\n");
		return -1;
	}


	fd_set writefds;
	struct timeval timeout;

	FD_ZERO(&writefds);
	FD_SET(iSocketId, &writefds);
	timeout.tv_sec = time;
	timeout.tv_usec = 0;

	iRet = select(iSocketId + 1, NULL, &writefds, NULL, &timeout);
	if (iRet <= 0)
	{
		printf("select timeout!\n");
		close(iSocketId);
		return -1;
	}

	if (!FD_ISSET(iSocketId, &writefds))
	{
		printf("no event found on iSocketId\n");
		close(iSocketId);
		return -1;
	}

	int error = 0;
	socklen_t  length = sizeof(error);
	if (getsockopt(iSocketId, SOL_SOCKET, SO_ERROR, &error, &length) < 0)
	{
		printf("getsockopt failed\n");
		close(iSocketId);
		return -1;
	}

	if (error != 0)
	{
		printf("connection failed after select with error:%d\n", error);
		close(iSocketId);
		return -1;
	}

	printf("connection ready after select with socked:%d\n", iSocketId);
	fcntl(iSocketId, F_SETFL, fdopt);
	return iSocketId;
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf(" usage: %s  ip port\n", basename(argv[0]));
		return -1;
	}

	const char *ip = argv[1];
	int port = atoi(argv[2]);

	int iSocketId =  non_block_connect(ip, port, 10);
	if (iSocketId < 0)
	{
		printf("non_block_connect failed\n");
		return -1;
	}

	//do something
	char buf[BUFFER_SIZE_MAX];
	memset(buf, 0, BUFFER_SIZE_MAX);
	strcpy(buf,"asdfasdfasdfasdfasf");
	int ret = send(iSocketId, buf, BUFFER_SIZE_MAX - 1, 0);

	ret = recv(iSocketId, buf, BUFFER_SIZE_MAX - 1, 0);
	if (ret > 0)
	{
		printf("get %d bytes of client data %s from %d\n",
			   ret,  buf, iSocketId);
	}

	close(iSocketId);
	return 0;
}
