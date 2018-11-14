#include "common.h"

int main( int argc,char *argv[] )
{
	int maxFd, iServFd,iClinFd,sockfd,client[MAX_ATTARCH_SIZE];
	int nready,i,n,maxi;
	socklen_t cliaddr_len;
	struct sockaddr_in servaddr,cliaddr;
	int iLen ;
	char sBuff[MAX_SIZE];
	char sBuffTemp[MAX_SIZE];
	fd_set allset, rset;
	struct timeval tv;
	
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
	
	memset(sBuff,0,sizeof(sBuff));
	memset(sBuffTemp,0,sizeof(sBuffTemp));
	
	for(i=0;i<MAX_ATTARCH_SIZE;i++)
		client[i]= -1;
	maxFd = iServFd;
	maxi = -1;
	FD_ZERO(&allset);
	FD_SET(iServFd, &allset);
	
	for(;;)
	{
		tv.tv_sec  = 3;
		tv.tv_usec = 0;
		rset = allset;
		nready = select(maxFd+1,&rset,NULL,NULL,&tv);
		if (nready < 0)
			printf("select error");
		else 
			printf("nready = %d\n",nready);
		
		if(FD_ISSET(iServFd,&rset))
		{			
			cliaddr_len = sizeof(cliaddr);
			if((iClinFd=accept(iServFd,(struct sockaddr*)&cliaddr,&cliaddr_len))== -1)
			{
				MY_ERR("accept failed!");
				exit(-1);
			}
			printf("received from %s at PORT %d\n", 
				inet_ntop(AF_INET, &cliaddr.sin_addr, sBuffTemp, sizeof(sBuffTemp)),
				 ntohs(cliaddr.sin_port));
				 
			for (i = 0; i < MAX_ATTARCH_SIZE; i++) 
			{	
				if (client[i] < 0) 
					{
						client[i] = iClinFd; /* save descriptor */
					 	break;
					}
			}
			
			if (i == MAX_ATTARCH_SIZE) 
			{ 
				fputs("too many clients\n", stderr); 
				exit(1); 
			} 
			
			FD_SET(iClinFd, &allset); /* add new descriptor to set */ 
			if (iClinFd > maxFd) 
				maxFd = iClinFd; /* for select */
			if (i > maxi) 
				maxi = i; /* max index in client[] array */ 
			
			if (--nready == 0) 
				continue; /* no more readable descriptors */
		}
		
		for (i = 0; i <= maxi; i++) 
		{ /* check all clients for data */
			if ( (sockfd = client[i]) < 0) 
				continue; 
			
			write(sockfd, GetCurTime(time(NULL)), MAX_SIZE);
			
			if (FD_ISSET(sockfd, &rset)) 
			{ 
				if ( (n = read(sockfd, sBuff, MAX_SIZE)) == 0) 
				{ /* connection closed by client */ 
					close(sockfd); 
					FD_CLR(sockfd, &allset); 
					client[i] = -1; 
				} 
				else 
				{ 
					
					int j;
					for (j = 0; j < n; j++) 
						sBuff[j] = toupper(sBuff[j]); 
							write(sockfd, sBuff, n); 
				} 
				if (--nready == 0) 
					break; /* no more readable descriptors */ 
			} 
		}
	}

	close(iServFd);
	
	return 0;	
}


