#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc,char *argv[])
{
    char hostname[32]={};
    char ipaddr[16]={};
    struct hostent*  phostent;

    if(argc<3)
        return -1;

    if(atoi(argv[1]) == 0)
    {
        strcpy(hostname,argv[2]);
        phostent = gethostbyname(hostname);
    }
    else
    {
        strcpy(ipaddr,argv[2]);
        phostent = gethostbyaddr(ipaddr,sizeof(ipaddr),AF_INET);
    }

    printf("h_name[%s]\n",phostent->h_name);
	printf("h_addr_list[%s]\n",inet_ntoa(*( struct in_addr*)(*phostent->h_addr_list)));

    return 0;
}
