/*************************************************************************
    > File Name: epoll_server.cpp
    > Author: wzq
    > Mail:  
    > Created Time: 2018年08月24日 星期五 14时55分15秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <set>

using namespace std;

#define PORT 8899
#define BUFFER_SIZE 1024
#define CONNECT_SIZE  32

static int SetNonblock(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0) return -1;
    return 0;
}

static int SetReUseAddr(int fd)
{
    int reuse = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
}

void DealNewConnect(int cfd)
{
    int n;
    char buffer[BUFFER_SIZE] = { 0 };
    printf("buffer:%s\n", buffer);

    const char *header_html = "HTTP/1.0 200 OK\r\n" "Server: Phone-Test/1.0\r\n" "Content-Type: text/html;charset=UTF-8\r\n" "Content-Length: %d\r\n\r\n";

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

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, header_html, strlen(html));
    strcat(buffer, html);
    n =  write(cfd, buffer, strlen(buffer));
    printf("len:%d,buffer:%s\n", n, buffer);
}
void DealRecv(int len, int cfd, char *buffer)
{
    printf("recvice from [%d],len:%d,buffer:\n%s\n", cfd, len, buffer);
    const char *header_html = "HTTP/1.0 200 OK\r\n" "Server: Phone-Test/1.0\r\n" "Content-Type: text/html;charset=UTF-8\r\n" "Content-Length: %d\r\n\r\n";

	if (NULL == strstr(buffer,"GET "))
	{
		memset(buffer,0,sizeof(buffer));
		strcpy(buffer, "hello .this is server!");
        int n =  write(cfd, buffer, strlen(buffer));
        printf("send to [%d],len:%d,buffer:%s\n", cfd, n, buffer);
	}
	else if (NULL==strstr(buffer,"msisdn"))
    {
        const char *default_html = "<!doctype html>\r\n" "<html>\r\n" "<head>\r\n" "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\r\n" "<title>phone number query</title>\r\n" "</head>\r\n"
        "<body>\r\n"
        "<h1>phone number query</h1>\r\n" "phone number information\r\n"
        "<form action=msisdn method=get><p>"
        "msisdn:head<input type=text name=head size=12 />"
        "time before(minutes)<input type=text name=before size=12 value=1 />"
        "size<input type=text name=size size=12 value=25 />"
        "</p>"
        "<p><input type=submit value=submit /></p>\r\n"
        "</form>" "<br/><br/>"
        "<a href=post_pdp>post pdp imformation</a><br/><br/><br/>" "</body>\r\n"
        "</html>\r\n";



        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, header_html, strlen(default_html));
        strcat(buffer, default_html);
        int n =  write(cfd, buffer, strlen(buffer));
        printf("send to [%d],len:%d,buffer:\n%s\n", cfd, n, buffer);
    }
    else
    {
        /*
        const char      result_fmt[] =                  //"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<result>\r\n"
        "msisdn,imei,imsi\r\n<br />"
        "18612345678,324567890123,6543217890\r\n<br />"
        "</result>";
        */
        const char result_fmt[] =
            "<result>\r\n"
            "<table border=\"1\">\r\n"
            "<tr>\r\n"
            "<th>msisdn</td>\r\n"
            "<th>imsi</td>\r\n"
            "<th>imei</td>\r\n"
            "</tr>\r\n"
            "<tr>\r\n"
            "<td>18712345678</td>\r\n"
            "<td>324567890123</td>\r\n"
            "<td>6543217890</td>\r\n"
            "</tr>\r\n"
            "<tr>\r\n"
            "<td>18787976558</td>\r\n"
            "<td>666667890123</td>\r\n"
            "<td>8655431789</td>\r\n"
            "</tr>\r\n"
            "</table>\r\n"
            "</result>\r\n";

        memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, header_html, strlen(result_fmt));
        strcat(buffer, result_fmt);
        int n =  write(cfd, buffer, strlen(buffer));
        printf("send to [%d],len:%d,buffer:\n%s\n", cfd, n, buffer);

    }
}
void DealSend()
{

}
int Run()
{
    int epollfd;
    char buffer[BUFFER_SIZE] = { 0 };

    struct sockaddr_in addr;
    memset(&addr, 0x0, sizeof(struct sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; //inet_addr(ip);
    addr.sin_port = htons(PORT);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket failed \n");
        exit(0);
    }
    SetReUseAddr(sockfd);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind failed\n");
        exit(0);
    }

    if (listen(sockfd, 10) < 0)
    {
        perror("listen failed\n");
        exit(0);
    }
    printf("build socket server %d\n", sockfd);
    SetNonblock(sockfd);


    epollfd = epoll_create(CONNECT_SIZE);
    if (-1 == epollfd)
    {
        perror("epoll_create failed\n");
        exit(0);
    }

    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLOUT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);



    int icnt = 0;
    struct epoll_event events[CONNECT_SIZE];
    while (1)
    {
        int nfds = epoll_wait(epollfd, events, CONNECT_SIZE, 1000);
        if (nfds < 0)
        {
            perror("epoll_wait\n");
            exit(0);
        }
        printf("nfds:%d\n",nfds);

        for (int i = 0; i < nfds; i++)
        {
            memset(buffer, 0, sizeof(buffer));
            int clientfd = events[i].data.fd;
			printf("fd:%d,events:0X%x\n",clientfd,events[i].events);
            if (events[i].events & EPOLLIN)
            {
                if (clientfd == sockfd)
                {
                    int new_clientfd;
                    struct sockaddr_in addr;
                    memset(&addr, 0x0, sizeof(struct sockaddr_in));
                    socklen_t socklen = sizeof(struct sockaddr_in);

                    if ((new_clientfd = accept(sockfd, (struct sockaddr *)&addr, &socklen)) < 0)
                    {
                        perror("accept failed\n");
                        continue;
                    }

                    printf("######%d get NO[%d] client[%d],from %s:%d\n", sockfd, icnt, new_clientfd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

					// DealNewConnect(new_clientfd);

                    struct epoll_event ev;
                    ev.data.fd = new_clientfd;
                    ev.events = EPOLLIN | EPOLLOUT;
                    epoll_ctl(epollfd, EPOLL_CTL_ADD, new_clientfd, &ev);
                    icnt++;
                }
                else
                {
                    int len = recv(clientfd, buffer, sizeof(buffer), 0);

                    if (len > 0)
                    {
                        DealRecv(len, clientfd, buffer);
                    }
                    else if (len == 0)
                    {
                        //printf("disconnect %d\n", clientfd);
                        //icnt--;
                        //epoll_ctl(epollfd, EPOLL_CTL_DEL, clientfd, NULL);
                        //close(clientfd);
                    }
                    else
                    {
                        if (errno  == EINTR) continue;

                        printf(" Error clientfd:%d, errno:%d\n", clientfd, errno);
                        close(clientfd);
                    }
                }
            }
            else if (events[i].events & EPOLLOUT)
            {
                DealSend();
            }
        }

        usleep(1000);
    }
}




int main()
{
    Run();
    return 0;
}


