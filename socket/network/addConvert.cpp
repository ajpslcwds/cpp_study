#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>


int main(int argc,char* args[])
{
	if(argc <2)
		return -1;

	char add[32] ={0};
	strncpy(add,args[1],sizeof(add)-1);	
	printf("src add[%s]\n",add);

	in_addr_t add_t = inet_addr(add);
	printf("add_t[%u]\n",add_t);

	struct in_addr inaddr;
	inet_aton(add,&inaddr);
	printf("addin[%u]\n",inaddr.s_addr);

	memset(add,0,32);
	strcpy(add,inet_ntoa(inaddr));
	printf("src add[%s]\n",add);


	return 0;
}
