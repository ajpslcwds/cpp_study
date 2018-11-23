/*************************************************************************
      > File Name: httptest.cpp
      > Author: wzq
      > Created Time: Mon Nov 12 17:36:49 2018
 ************************************************************************/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include <netinet/in.h> /* sockaddr_in{}and other internet definitions */
#include <arpa/inet.h> /* inet(3) functions */

int main()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);

	socklen_t socklen;
	struct sockaddr_in sockaddr_s,sockaddr_c;
	sockaddr_s.sin_addr.s_addr  = htonl(INADDR_ANY);
	sockaddr_s.sin_family = AF_INET;
	sockaddr_s.sin_port = htons(8888);

	bind(sockfd,(struct sockaddr *)&sockaddr_s, sizeof(sockaddr_s));

	listen(sockfd,5);
	char buffer[1024];
	while(1)
	{
		memset(&sockaddr_c,0x00,sizeof(sockaddr_c));	
		int cfd = accept(sockfd,(struct sockaddr *)&sockaddr_c,&socklen);
		memset(buffer,0,sizeof(buffer));
		int n = recv(cfd,buffer,sizeof(buffer),0);

		printf("len:%d,buffer:%s",n,buffer);

		const char * header_html = "HTTP/1.0 200 OK\r\n" "Server: Phone-Test/1.0\r\n" "Content-Type: text/html;charset=UTF-8\r\n" "Content-Length: %d\r\n\r\n";

		const char *html = 	"<!doctype html>\r\n               "
					"<html>\r\n                        "
					"	<head>\r\n                     "
					"		<meta charset=UTF-8>\r\n   "
					"		<title>test</title>\r\n    "
					"	</head>\r\n                    "
					"	<body>\r\n                     "
					"		it  a  test  hhh hhh\r\n   "
					"	</body>\r\n                   "
					"</html>\r\n                      ";
		 
		memset(buffer,0,sizeof(buffer));
		sprintf(buffer,header_html,strlen(html));
		strcat(buffer,html);
		n =  write(cfd,buffer,strlen(buffer));
		printf("len:%d,buffer:%s",n,buffer);
		close(cfd);

	}
	close(sockfd);

	return 0;
}
