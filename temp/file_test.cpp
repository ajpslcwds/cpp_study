/*************************************************************************
	> File Name: file_test.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2019年01月25日 星期五 15时19分12秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>

#define BUF_SIZE 4096

void create_orig_file()
{
	int fd = open("wzq.txt",O_CREAT|O_TRUNC|O_WRONLY,744);
	if(fd < 0)
	{
		printf("open %s err,errno:%d,errmsg:%s\n","wzq.txt",errno,strerror(errno));
		exit(0);
	}

	char sBuffer[1024] = {0};
	for (int i = 0;i<1000000;i++)
	{
		sprintf(sBuffer,"%d:%s\n",i,"wzq test hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhllh");		
		int iCnt = write(fd,sBuffer,strlen(sBuffer));
		if(iCnt != strlen(sBuffer))
		{
			printf("write,errno:%d,errmsg:%s\n",errno,strerror(errno));
			exit(0);
		}
	}

	close(fd);

}

void copy_file()
{
	char sBuffer[1024] = {0};
	int fd = open("wzq.txt",O_RDONLY,744);
	if(fd < 0)
	{
		printf("open %s err,errno:%d,errmsg:%s\n","wzq.txt",errno,strerror(errno));
		exit(0);
	}

	int fd2 = open("wzq_new.txt",O_CREAT|O_TRUNC|O_WRONLY,744);
	if(fd2 < 0)
	{
		printf("open %s err,errno:%d,errmsg:%s\n","wzq_new.txt",errno,strerror(errno));
		exit(0);
	}
	
	int iCnt=0;
	int iCnt2=0;
	while ((iCnt = read(fd,sBuffer,sizeof(sBuffer)))>0)
	{
		iCnt2 = write(fd2,sBuffer,iCnt);
		if(iCnt2 != iCnt)
		{
			printf("write err %d != %d,errno:%d,errmsg:%s\n",iCnt,iCnt2,errno,strerror(errno));
			exit(0);
		}
	}

	if(iCnt == 0)
	{
		close(fd2);
		close(fd);
	}
	else
	{
		printf("read %d err,errno:%d,errmsg:%s\n",iCnt,errno,strerror(errno));
		exit(0);
	}
}


void mmap_copy_file()
{
	int fd = open("wzq.txt",O_RDWR,744);
	if(fd < 0)
	{
		printf("open %s err,errno:%d,errmsg:%s\n","wzq.txt",errno,strerror(errno));
		exit(0);
	}

	int fd2 = open("wzq_mmap_new.txt",O_CREAT|O_TRUNC|O_RDWR,744);
	if(fd2 < 0)
	{
		printf("open %s err,errno:%d,errmsg:%s\n","wzq_mmap_new.txt",errno,strerror(errno));
		exit(0);
	}
	
	int iLen = lseek(fd,0,SEEK_END);
	int iLen2 = lseek(fd2,iLen-1,SEEK_CUR);
	write(fd2,"0",1);
	lseek(fd,0,SEEK_SET);
	lseek(fd2,0,SEEK_SET);
	
	int each_size = BUF_SIZE;//iLen
	int off_size = 0;
	char *p = NULL;
	char *p2 = NULL;
	while(1 )
	{
		each_size = iLen - off_size >BUF_SIZE?BUF_SIZE: iLen - off_size;
		if (each_size == 0)
			break;	
		p = (char*)mmap(NULL,each_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,off_size);
		if(p==(char*) -1)
		{
			printf("mmap err,errno:%d,errmsg:%s\n",errno,strerror(errno));
			exit(0);
		}
		p2 = (char*)mmap(NULL,each_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd2,off_size);
		if(p2==(char*) -1)
		{
			printf("mmap err,errno:%d,errmsg:%s\n",errno,strerror(errno));
			exit(0);
		}
		
		

		//memcpy(p2,p,each_size);	
		off_size += each_size;		
		
		if (each_size<BUF_SIZE)
		{
			munmap(p,each_size);
			munmap(p2,each_size);
			break;
		}
	}

	close(fd);
	close(fd2);
}
int exec_time(void (*func)(void),char *funcname)
{
	time_t  tb = time(NULL);
	func();
	time_t  te = time(NULL);
	printf("exec func %s cost %lld Seconds\n",funcname,te-tb);
}

int main()
{
	//exec_time(create_orig_file,(char*)"create_orig_file");
	exec_time(copy_file,(char*)"copy_file");
	exec_time(mmap_copy_file,(char*)"mmap_copy_file");
	return 0;
}



