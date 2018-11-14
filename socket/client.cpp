#include "common.h"

int main(int argc,char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr;
	int iLen ;
	char sBuff[MAX_SIZE];
	
	if( (sockfd = socket(PF_INET,SOCK_STREAM,0))==-1)
	{
		MY_ERR("socket failed!");
		exit(-1);
	}
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERVER_PORT);
		
	if(connect(sockfd,(struct sockaddr *)&servaddr, sizeof(servaddr))<0)
	{
		MY_ERR("connect failed!");
		exit(-1);
	}
	
	if(argv[1]!=NULL)
	{
		strncpy(sBuff,argv[1],MAX_SIZE-1);
	}
	else
	{
		sprintf(sBuff,"%d",getpid());
	}

	if(send(sockfd,sBuff,MAX_SIZE,0)<0)
	{
		MY_ERR("send failed!");
		exit(-1);
	}
	
	while(1)
	{
		iLen = recv(sockfd,sBuff,MAX_SIZE,0);
		if(iLen>0)
		{
			sBuff[iLen] = '\0';
			MY_MSG(sBuff);
		}
		else
		{
			sleep(3);
		}
	}
	
	close(sockfd);
	return 0;	
}


