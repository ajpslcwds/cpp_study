#ifndef _SOCKET_H_
#define _SOCKET_H_
#include"common.h"
#include<arpa/inet.h>
#include<netinet/in.h>


#define BUFF_SIZE  BUFFER_MAX_SIZE
#define CONNECT_NUM 5
#define MyPrintfInf(...) { \
	printf(__VA_ARGS__); \
}

#define MyPrintfErr(...) { \
	printf(__VA_ARGS__); \
}


const char SERVER_IP[32] =  "127.0.0.1";
const int SERVER_PORT = 6666;


void 	test_socket(int argc, char *args[]);
int 	AsServer();
int 	AsClient();


class MySocket
{
public:
	MySocket();
	~MySocket();
public:
	int Listen(const char *sServerIp, const int sServerPort, int iMaxConnNum = CONNECT_NUM);
	int Connect(const char *sServerIp, const int sServerPort);
	int Accept();
	int Write(char *szBuffer, int iLen);
	int Read(char *szBuffer, int iLen);
	int Write(int iSocketId,char *szBuffer, int iLen);
	int Read(int iSocketId,char *szBuffer, int iLen);

	int GetSocketID();
private:
	int m_iSocketId; //用于交互的id，是client端的id
	int m_iOldSocketId;//server 端 自己的id。
};

#endif
