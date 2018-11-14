#include "common.h"

int main()
{
	int iServFd,iClinFd;
	struct sockaddr_in servaddr;
	int iLen ;
	char sBuff[MAX_SIZE];
	
	if( (iServFd = socket(PF_INET,SOCK_STREAM,0))==-1)
	{
		MY_ERR("socket failed!");
		exit(-1);
	}
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);
	
	if( bind(iServFd ,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
	{
		MY_ERR("bind failed!");
		exit(-1);			
	}
	if(listen(iServFd,MAX_SIZE) == -1)
	{
		MY_ERR("listen failed!");
		exit(-1);	
	}
	while(1)
	{
		if((iClinFd=accept(iServFd,(struct sockaddr*)NULL, NULL))== -1)
		{
			MY_ERR("accept failed!");
			exit(-1);
		}
		
		iLen = recv(iClinFd,sBuff,MAX_SIZE,0);
		sBuff[iLen] = '\0';
		MY_MSG(sBuff);
		for(int i =0;i<iLen;i++)
		{
			sBuff[i]=toupper(sBuff[i]);	
		}
		
		if(send(iClinFd,GetCurTime(time(NULL)),15,0)<0)
		{
			MY_ERR("send failed!");
			exit(-1);
		}	
		if(send(iClinFd,sBuff,iLen,0)<0)
		{
			MY_ERR("send failed!");
			exit(-1);
		}		
		
		close(iClinFd);
		sleep(5);
	}
	close(iServFd);
	
	return 0;	
}


