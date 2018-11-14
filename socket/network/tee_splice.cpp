#include "common.h"

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("usage: %s filename\n",argv[0]);
		return -1;
	}
	int ret = 0;
	int file_fd =  open(argv[1],O_CREAT|O_TRUNC|O_WRONLY,0666);
	assert(file_fd>0);

	int pipe_out[2];
	ret = pipe(pipe_out);
	assert(ret!=-1);
	
	int pipe_file[2];
	ret = pipe(pipe_file);
	assert(ret!=-1);
	
	ret = splice(STDIN_FILENO,NULL,pipe_out[1],NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
	assert(ret!=-1);
	
	ret = tee(pipe_out[0],pipe_file[1],32768,SPLICE_F_NONBLOCK);
	assert(ret!=-1);

	ret = splice(pipe_file[0],NULL,file_fd,NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
	assert(ret!=-1);

	ret = splice(pipe_out[0],NULL,STDOUT_FILENO,NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
	assert(ret!=-1);

	close(file_fd);
	close(pipe_out[0]);
	close(pipe_out[1]);
	close(pipe_file[0]);
	close(pipe_file[1]);

	return 0;
}
