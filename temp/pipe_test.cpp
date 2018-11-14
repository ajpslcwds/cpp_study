/*************************************************************************
	> File Name: pipe_test.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年08月09日 星期四 15时35分55秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>

int main()
{
	int fd[2];

	if(pipe(fd))
	{
		perror("pipe failed\n");
		return -1;
	}	

	pid_t pid = fork();
	if (pid <0)
	{
		perror("fork failed\n");
		return -1;
	}	
	else if (pid > 0 )
	{
		close(fd[0]);
		char buff[32] = "this is father";

		for (int i =0;i<10;i++)
		{
			write(fd[1],buff,strlen(buff)+1);
			sleep(2);
		}

	}
	else
	{
		close(fd[1]);
		char buff[32] ={};
		
		for (int i =0;i<10;i++)
		{
			memset(buff,0,sizeof(buff));
			ssize_t len  = read(fd[0],buff,sizeof(buff)-1);
			if (len>0)
				printf("%s\n",buff);
		}

	}

	return 0;

}
