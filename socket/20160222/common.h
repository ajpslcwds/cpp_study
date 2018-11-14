#ifndef COMMON_H
#define COMMON_H

#include<time.h>
#include<unistd.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define SERVER_IP "192.168.41.138"
#define SERVER_PORT 6666
#define MAX_SIZE 1024
#define MY_MSG(msg)  printf("MSG=[%s]\n",msg);
#define MY_ERR(msg)  printf("%s,errno = %d,err_msg = %s\n",msg,errno,strerror(errno));

char* GetCurTime(time_t tTime)
{
	static char sTime[15];
    struct tm * tm;

    tm = localtime(&tTime);

    sprintf(sTime,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,
            tm->tm_hour,tm->tm_min,tm->tm_sec);
    return sTime;
	
}

#endif
