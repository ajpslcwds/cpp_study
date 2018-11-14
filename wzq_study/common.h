#ifndef _COMMON_H_
#define _COMMON_H_


#include<iostream>
#include<algorithm>
#include<string.h>
#include<string>
#include<assert.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<map>
#include<set>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<errno.h>
#include<math.h>

using namespace std;

#define BUFFER_MAX_SIZE 1024



inline char* GetCurTime(time_t tTime = time(NULL))
{
	static char sTime[15];
	struct tm *tm;

	tm = localtime(&tTime);

	sprintf(sTime, "%04d%02d%02d%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);
	return sTime;
}

inline  char* Trim(char *pszStr)
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

#endif//_COMMON_H_
