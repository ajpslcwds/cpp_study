#include"epoll_socket.h"


EpollSocket::EpollSocket()
{
    memset(this, 0, sizeof(EpollSocket));
}
EpollSocket::~EpollSocket()
{

}
int EpollSocket::SetNonblock(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0) return -1;
    return 0;
}

int EpollSocket::SetReUseAddr(int fd)
{
    int reuse = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
}


int EpollSocket::Write(char *szBuffer, int iLen)
{
    int iRet;

    if (m_ServerSockId <= 0)
    {
        MyPrintfErr("Socket not connected\n");
        return -1;
    }

    iRet = write(m_ServerSockId, szBuffer, iLen);
    return iRet;
}

int EpollSocket::Read(char *szBuffer, int iLen)
{
    int iRet;

    if (m_ServerSockId <= 0)
    {
        MyPrintfErr("Socket not connected\n");
        return -1;
    }

    iRet = read(m_ServerSockId, szBuffer, iLen);
    return iRet;
}

int EpollSocket::Init(char *Ip, int Port, int ProcType, int EpollEventSize)
{
    strncpy(m_Ip, Ip, sizeof(m_Ip));
    m_Port = Port;
    m_ProcType = ProcType;

    m_EpollFd = epoll_create(EpollEventSize);
    if (m_EpollFd < 0)
    {
        MyPrintfErr("epoll_create failed,%s", strerror(errno));
        return  RS_COMMON;
    }

    return RS_SUCCESS;
}


int EpollSocket::Execute()
{
    int iRet = RS_SUCCESS;

    if (m_ProcType == PROC_SERVER)
    {
        MyPrintfInf("RunAsServer start!\n");
        iRet = RunAsServer();
        if (0 != iRet)
        {
            MyPrintfErr("Exec RunAsServer error!\n");
            return iRet;
        }
    }
    else // if (m_ProcType == PROC_CLIENT)
    {
        MyPrintfInf("RunAsClient start!\n");
        iRet = RunAsClient();
        if (0 != iRet)
        {
            MyPrintfErr("Exec RunAsClient error!\n");
            return iRet;
        }
    }

    return iRet;
}

int EpollSocket::RunAsServer()
{
    int iRet = RS_SUCCESS;

    m_ServerSockId = socket(AF_INET, SOCK_STREAM, 0);
    if (m_ServerSockId < 0)
    {
        MyPrintfErr("socket failed,%s!\n", strerror(errno));
        return RS_COMMON;
    }

    struct sockaddr_in addr;
    memset(&addr, 0x0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(m_Ip);
    addr.sin_port = htons(m_Port);
    if (bind(m_ServerSockId, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0)
    {
        MyPrintfErr("bind failed,%s!\n", strerror(errno));
        return RS_COMMON;
    }

    if (listen(m_ServerSockId, EVENT_MAX_SIZE - 1) < 0)
    {
        MyPrintfErr("listen failed,%s!\n", strerror(errno));
        return RS_COMMON;
    }

    SetNonblock(m_ServerSockId);
    struct epoll_event ev;
    ev.data.fd = m_ServerSockId;
    ev.events = EPOLLIN | EPOLLOUT;
    epoll_ctl(m_EpollFd, EPOLL_CTL_ADD, m_ServerSockId, &ev);

    while (1)
    {
        iRet = EpollWait();
        if (iRet != RS_SUCCESS)
        {
            MyPrintfErr("EpollWait failed,iRet = %d!\n", iRet);
            return iRet;
        }
    }

    return iRet;
}

int EpollSocket::RunAsClient()
{
    int iRet = RS_SUCCESS;

    m_ServerSockId = socket(AF_INET, SOCK_STREAM, 0);
    if (m_ServerSockId < 0)
    {
        MyPrintfErr("socket failed,%s!\n", strerror(errno));
        return RS_COMMON;
    }

    struct sockaddr_in addr;
    memset(&addr, 0x0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(m_Ip);
    addr.sin_port = htons(m_Port);
    if (connect(m_ServerSockId, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0)
    {
        MyPrintfErr("connect failed,%s!\n", strerror(errno));
        return RS_COMMON;
    }
	MyPrintfInf("%d connecting to %s:%d\n",m_ServerSockId , inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    
	
	SetReUseAddr(m_ServerSockId);
    SetNonblock(m_ServerSockId);
    struct epoll_event ev;
    ev.data.fd = m_ServerSockId;
    ev.events = EPOLLIN | EPOLLOUT;
    epoll_ctl(m_EpollFd, EPOLL_CTL_ADD, m_ServerSockId, &ev);

    while (1)
    {
        iRet = EpollWait();
        if (iRet != RS_SUCCESS)
        {
            MyPrintfErr("EpollWait failed,iRet = %d!\n", iRet);
            return iRet;
        }
    }

    return iRet;
}

int EpollSocket::EpollWait()
{
    int iRet = RS_SUCCESS;
    int iCnt = 0;
    char buffer[BUFFER_TMP_SIZE] = { };
    struct epoll_event events[EVENT_MAX_SIZE];
    do
    {
        int iFds = epoll_wait(m_EpollFd, events, EVENT_MAX_SIZE, 1000);
        if (iFds < 0)
        {
            MyPrintfErr("epoll_wait failed,%s!\n", strerror(errno));
            return RS_COMMON;
        }
        else if (iFds == 0)
        {
            usleep(1000);
        }
        else
        {
            for (int i = 0; i < iFds; i++)
            {
                int sockfd = events[i].data.fd;
				MyPrintfInf("NO:%d,sockfd:%d,events:%x\n",i,sockfd,events[i].events);
                if (m_ProcType == PROC_SERVER && sockfd == m_ServerSockId)
                {
                    int new_clientfd;
                    struct sockaddr_in addr;
                    memset(&addr, 0x0, sizeof(struct sockaddr_in));
                    socklen_t socklen = sizeof(struct sockaddr_in);

                    if ((new_clientfd = accept(sockfd, (struct sockaddr *)&addr, &socklen)) < 0)
                    {
                        MyPrintfErr("accept failed,%s!\n", strerror(errno));
                        return RS_COMMON;
                    }
                    MyPrintfInf("%d get [%d] new client[%d],from %s:%d\n", sockfd, iCnt, new_clientfd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

                    memset(buffer, 0, sizeof(buffer));
                    sprintf(buffer, "connection establishment successful!", sockfd);
                    send(new_clientfd, buffer, sizeof(buffer), 0);
                    struct epoll_event ev;
                    ev.data.fd = new_clientfd;
                    ev.events = EPOLLIN | EPOLLOUT;
                    epoll_ctl(m_EpollFd, EPOLL_CTL_ADD, new_clientfd, &ev);
                    iCnt++;
                }
				else
				{
					if (events[i].events & EPOLLIN)
					{
						memset(buffer, 0, sizeof(buffer));
						int len = recv(sockfd, buffer, sizeof(buffer), 0);
						if (len > 0)
						{
							MyPrintfInf("recv from %d [%s]\n", sockfd, buffer);
						}
						else if (len == 0)
						{
							MyPrintfInf("disconnect %d\n", sockfd);
							iCnt--;
							epoll_ctl(m_EpollFd, EPOLL_CTL_DEL, sockfd, NULL);
							close(sockfd);
						}
						else
						{
							if (errno  == EINTR) continue;

							MyPrintfInf(" Error sockfd:%d, errno:%d,msg:\n", sockfd, errno, strerror(errno));
							close(sockfd);
						}
					}
					if (events[i].events & EPOLLOUT)
					{
						static int iMsgNO = 0;
						memset(buffer, 0, sizeof(buffer));
						sprintf(buffer, "hello ,I am %d_%d", m_ServerSockId,iMsgNO++ );
						MyPrintfInf("send to %d [%s]\n", sockfd, buffer);
						send(sockfd, buffer, sizeof(buffer), 0);
					}
				}
            }
        }
    }while (0);

    return iRet;
}


int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        MyPrintfErr("USAGE: %s ip port flag \n", argv[0]);
        MyPrintfErr("run as server: %s 127.0.0.1 8890 0 \n", argv[0]);
        MyPrintfErr("run as client: %s 127.0.0.1 8890 1 \n", argv[0]);
        return 0;
    }

    EpollSocket es;
    es.Init(argv[1], atoi(argv[2]), atoi(argv[3]));
    es.Execute();
    return 0;
}









