#ifndef _COMMON_H_
#define _COMMON_H_


#include<iostream>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include <fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<map>
#include<set>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<errno.h>
#include<math.h>


using namespace std;

#define BUFFER_MAX_SIZE 1024
#define BUFFER_TMP_SIZE 128

#define IPC_MODE  IPC_CREAT|0666
 

#if 1
#define MyPrintfInf(...) { \
	printf(__VA_ARGS__); \
}

#define MyPrintfErr(...) { \
	printf(__VA_ARGS__); \
}
#else
#define MyPrintfInf(...) { \
	PAS_LOG_INF(__VA_ARGS__); \
}
#define MyPrintfErr(...) { \
	PAS_LOG_ERR(__VA_ARGS__); \
}
#endif

enum ResultCode
{
	RS_SUCCESS = 0,
	RS_COMMON ,
	RS_ALLOC ,
	RS_NO_SHM,

};



inline char* GetCurTime(time_t tTime = time(NULL))
{
	static char sTime[15];
	struct tm *tm;

	tm = localtime(&tTime);

	sprintf(sTime, "%04d%02d%02d%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);
	return sTime;
}

#endif
