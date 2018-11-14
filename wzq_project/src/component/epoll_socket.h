#ifndef _EPOLL_SOCKET_H_
#define _EPOLL_SOCKET_H_

#include "common.h"

#define EVENT_MAX_SIZE 256

enum PROCESS_TYPE
{
    PROC_SERVER,
    PROC_CLIENT
};

class EpollSocket
{
public:
    EpollSocket();
    ~EpollSocket();

    int Init(char *Ip, int Port, int ProcType, int EpollEventSize = EVENT_MAX_SIZE);
    int SetNonblock(int fd);
    int SetReUseAddr(int fd);
    int Write(char *szBuffer, int iLen);
    int Read(char *szBuffer, int iLen);
    int EpollWait();


    int RunAsServer();
    int RunAsClient();
    int Execute();
private:
    char    m_Ip[40];
    int     m_Port;
    
    int     m_ProcType;
    int     m_ServerSockId;
    int     m_ClientSockId;
    int     m_EpollFd;

};

#endif
