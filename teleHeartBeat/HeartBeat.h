#ifndef _HEART_BEAT_H_
#define _HEART_BEAT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <time.h>
#include <iostream>
#include <errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<signal.h>
#include "pasinstru.h"
#include "pasCommon.h"
#include "../teleCommon/mpGlobal.h"
#include "../teleCommon/teleCommon.h"
#include "pasToolsCpp.h"

using namespace std;


#define BUFF_SIZE  1024
#define IP_BUFFER_LEN  32

#if 0
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



enum
{
	MAIN_FLAG,
	BACKUP_FLAG
};
enum
{
	SERVER_FLAG,
	CLIENT_FLAG
};

struct ConnectInfo
{
	char ServerIp[IP_BUFFER_LEN];
	int  ServerPort;
};

struct ConfigInfo
{
	///////////////////configinfo
	int CSFlag;  // 0 server ; 1 client
	int iEthernetNumber;
	char sEthernet[16][32];

	ConnectInfo Server[2];

	int SleepSec;  //sleep seconds
	int RetryTimes;   //retry times

	int	bWapLog;
	int	bWriteType;
	int	nLogType;
	int	nMaxLogSize;
	int	nMaxLogIndex;
	char szLogFile[256];
	int	nLogLevel;

	/////////////////run info
	int stop;
	int msgType;
	int iSignal;
	char sServerDir[256];
	char sProcessName[32];

	ConfigInfo()
	{
		memset(this, 0x0, sizeof(ConfigInfo));
	}
};

class HeartBeat
{
public:
	HeartBeat();
	~HeartBeat();

	int Listen(char *sServerIp, int sServerPort, int iMaxConnNum = 5);
	int Connect(char *sServerIp, int sServerPort);
	int Accept();
	int Write(char *szBuffer, int iLen);
	int Read(char *szBuffer, int iLen);
	int GetSocketID() { return m_iSocketId;};
private:
	int	m_iSocketId; //传输数据用的
	int m_iOldSocketId; //服务端

};


char* GetCurTime(time_t tTime = time(NULL))
{
	static char sTime[15];
	struct tm *tm;

	tm = localtime(&tTime);

	sprintf(sTime, "%04d%02d%02d%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);
	return sTime;
}

char* Trim(char *pszStr)
{
	char *pszBegin = NULL;
	char *pszEnd = NULL;
	char *pszTmp = NULL;

	if (pszStr == NULL) return NULL;

	pszBegin = pszStr;
	while (*pszBegin == ' ' || *pszBegin == '\t')  pszBegin++;

	pszTmp = pszBegin;
	while (*pszTmp)
	{
		switch (*pszTmp)
		{
		case ' ':
		case '\t':
			if (pszEnd == NULL) pszEnd = pszTmp;
			break;
		default:
			if (pszEnd != NULL) pszEnd = NULL;
			break;
		}
		pszTmp++;
	}
	if (pszEnd == NULL) pszEnd = pszTmp;

	*pszEnd = '\0';

	memmove(pszStr, pszBegin, pszEnd - pszBegin + 1);

	return pszStr;
}

#endif // _HEART_BEAT_H_
