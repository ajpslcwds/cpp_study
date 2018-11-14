#include"process.h"


void  test_process()
{
	pid_t pid = 0;
	int iPipe[2]={0};
	char sBuff[64]={0};

	if (pipe(iPipe)<0)
	{
		cout<<"pipe error "<<endl;
		return;
	}
	pid = fork();
	if (pid <0 )
	{
		cout<<"fork error"<<endl;
	}
	else if (pid == 0 )
	{
		cout<<"this is child,pid="<<getpid()<<endl;
		close(iPipe[1]);
		sleep(5);
		if (read(iPipe[0],sBuff,64)>0)
		{
			cout<<"receive from pipe "<<sBuff<<endl;
		}
		close(iPipe[0]);
		cout<<"this is child end"<<endl;
	}
	else
	{
		cout<<"this is parent,pid="<<getpid()<<endl;
		close(iPipe[0]);
		if (-1 ==write(iPipe[1],"hello",strlen("hello")))
		{
			cout<<"error write"<<endl;
		}
		close(iPipe[1]);
		//wait(NULL);
		waitpid(pid,NULL,0);
		cout<<"this is parent end"<<endl;
	}

}

