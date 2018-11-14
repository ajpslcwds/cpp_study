#include"mysocket.h"


MySocket::MySocket()
{
	m_iOldSocketId = -1;
	m_iSocketId = -1;
};
MySocket::~MySocket()
{
	m_iOldSocketId = -1;
	m_iSocketId = -1;
};

int MySocket::Listen(const char *sServerIp, const int sServerPort, int iMaxConnNum)
{
	struct sockaddr_in tServerAddr;
	int iSocketId;
	int iNewLen = sizeof(struct sockaddr);

	memset((char *)&tServerAddr, 0, sizeof(tServerAddr));
	tServerAddr.sin_family      = AF_INET;
	tServerAddr.sin_addr.s_addr = inet_addr(sServerIp);
	tServerAddr.sin_port        = htons(sServerPort);

	if ((iSocketId = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		MyPrintfErr("socket error:%s\n", strerror(errno));
		return -1;
	}

	if (bind(iSocketId, (struct sockaddr *)&tServerAddr, iNewLen) < 0)
	{
		MyPrintfErr("bind error:%s\n", strerror(errno));
		return -1;
	}
	if (listen(iSocketId, iMaxConnNum) < 0)
	{
		MyPrintfErr("listen error:%s\n", strerror(errno));
		return -1;
	}

	m_iOldSocketId = iSocketId;

	return iSocketId;
}

int MySocket::Accept()
{
	int iNewSocketId;
	struct sockaddr_in tNewAddr;
	int iNewLen = sizeof(struct sockaddr);


	if ((iNewSocketId = accept(m_iOldSocketId, (struct sockaddr *)&tNewAddr, (socklen_t *)&iNewLen)) < 0)
	//if ((iNewSocketId = accept(m_iOldSocketId, (struct sockaddr *)NULL, (socklen_t *)NULL)) < 0)
	{
		MyPrintfErr("accept error:%s\n", strerror(errno));
		return -1;
	}
	else
	{
		m_iSocketId = iNewSocketId;
	}

	return iNewSocketId;
}

int MySocket::Connect(const char *sServerIp, const int sServerPort)
{
	struct sockaddr_in tServerAddr;
	int iSocketId;
	int iResult;

	tServerAddr.sin_family         = AF_INET;
	tServerAddr.sin_addr.s_addr    = inet_addr(sServerIp);
	tServerAddr.sin_port           = htons(sServerPort);

	if ((iSocketId = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		MyPrintfErr("socket error:%s\n", strerror(errno));
		return -1;
	}


	iResult = connect(iSocketId, (struct sockaddr *)&tServerAddr, sizeof(tServerAddr));
	if (iResult != 0)
	{
		MyPrintfErr("connect error:%s\n", strerror(errno));
		return -1;
	}
	else
	{
		m_iSocketId = iSocketId;
	}

	return iSocketId;
}

int MySocket::Write(char *szBuffer, int iLen)
{
	int iRet;

	if (m_iSocketId <= 0)
	{
		MyPrintfErr("Socket not connected\n");
		return -1;
	}

	iRet = write(m_iSocketId, szBuffer, iLen);
	return iRet;
}

int MySocket::Read(char *szBuffer, int iLen)
{
	int iRet;

	if (m_iSocketId <= 0)
	{
		MyPrintfErr("Socket not connected\n");
		return -1;
	}

	iRet = read(m_iSocketId, szBuffer, iLen);
	return iRet;
}

int MySocket::Write(int iSocketId,char *szBuffer, int iLen)
{
	int iRet;

	if (iSocketId <= 0)
	{
		MyPrintfErr("Socket not connected\n");
		return -1;
	}

	iRet = write(iSocketId, szBuffer, iLen);
	return iRet;
}
int MySocket::Read(int iSocketId,char *szBuffer, int iLen)
{
	int iRet;

	if (iSocketId <= 0)
	{
		MyPrintfErr("Socket not connected\n");
		return -1;
	}

	iRet = read(iSocketId, szBuffer, iLen);
	return iRet;
}

int MySocket::GetSocketID()
{
	return m_iSocketId;
}



//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
int AsServer()
{
	int iRet = 0;
	int iLen = 0;
	int iSocketId = 0;
	int iTempId = 0;
	int iClientId[CONNECT_NUM] = { 0 };
	char sBuff[BUFF_SIZE] = { 0 };
	int maxfd;
	int i;
	fd_set rfds;

	MySocket server;

	iSocketId = server.Listen(SERVER_IP, SERVER_PORT);
	if (iSocketId <= 0)
	{
		MyPrintfErr("[%s:%d]Listen error:%s\n", SERVER_IP, SERVER_PORT, strerror(errno));
		return -1;
	}

	while (1)
	{
		FD_ZERO(&rfds);
		FD_SET(iSocketId, &rfds);
		maxfd = iSocketId;
		for (i = 0; i < CONNECT_NUM; i++)
		{
			if (iClientId[i] > 0)
			{
				FD_SET(iClientId[i], &rfds);
				maxfd = maxfd > iClientId[i] ? maxfd : iClientId[i];
			}
		}

		select(maxfd + 1, &rfds, NULL, NULL, NULL);
		if (FD_ISSET(iSocketId, &rfds))
		{
			iTempId = server.Accept();
			if (iTempId <= 0)
			{
				MyPrintfErr("Accept error!\n");
				sleep(1);
				continue;
			}

			for (i = 0; i < CONNECT_NUM; i++)
			{
				if (iClientId[i] <= 0)
				{
					iClientId[i] = iTempId;
					break;
				}
			}
			if (i == CONNECT_NUM)
			{
				MyPrintfErr("the number of connections is full!\n");
				close(iTempId);
			}

			MyPrintfInf("Client [%d] connect success...\n",iTempId);
		}

		for (i = 0; i < CONNECT_NUM; i++)
		{
			if (iClientId[i] > 0 && FD_ISSET(iClientId[i], &rfds))
			{
				iTempId = iClientId[i];
				memset(sBuff, 0x0, BUFF_SIZE);
				iLen = server.Read(iTempId,sBuff, BUFF_SIZE - 1);
				if (iLen <= 0)
				{
					MyPrintfErr("Read error! close [%d]\n",iTempId);
					close(iTempId);
					iClientId[i] = -1;
					continue;
				}
				MyPrintfInf("[%s:%d %d]recv from client:[%s] at[%s]\n", SERVER_IP, SERVER_PORT, iTempId, sBuff, GetCurTime());

				memset(sBuff, 0x0, BUFF_SIZE);
				strcpy(sBuff, "I AM OK !");
				iLen = server.Write(iTempId,sBuff, strlen(sBuff));
				if (iLen <= 0)
				{
					MyPrintfErr("Write error!\n");
					continue;
				}
				MyPrintfInf("[%s:%d %d]send to client:[%s] at[%s]\n", SERVER_IP, SERVER_PORT, iTempId, sBuff, GetCurTime());
			}
		}
		sleep(1);
	}

	return iRet;
}

int AsClient()
{
	int iRet = 0;
	int iLen = 0;
	int iSocketId = 0;
	char sBuff[BUFF_SIZE] = { 0 };
	MySocket client;

	iSocketId = client.Connect(SERVER_IP, SERVER_PORT);
	if (iSocketId <= 0)
	{
		MyPrintfErr("[%s:%d]Connect error:%s\n", SERVER_IP, SERVER_PORT, strerror(errno));
		return -1;
	}
	MyPrintfInf("Client [%d] connect success...\n",iSocketId);

	while (1)
	{
		memset(sBuff, 0x0, BUFF_SIZE);
		strcpy(sBuff, "ARE YOU OK ?");
		iLen = client.Write(sBuff, strlen(sBuff));
		if (iLen <= 0)
		{
			MyPrintfErr("Write error!\n");
			return -1;	
		}
		MyPrintfInf("[%s:%d]send to server:[%s] at[%s]\n", SERVER_IP, SERVER_PORT, sBuff, GetCurTime());

		memset(sBuff, 0x0, BUFF_SIZE);
		iLen = client.Read(sBuff, BUFF_SIZE - 1);
		if (iLen <= 0)
		{
			MyPrintfErr("Read error!\n");
			return -1;
		}
		MyPrintfInf("[%s:%d]recv from server:[%s] at[%s]\n", SERVER_IP, SERVER_PORT, sBuff, GetCurTime());
		
		sleep(3);
	}

	close(iSocketId);
	return iRet;
}


void test_socket(int argc, char *args[])
{
	int iRet = 0;
	if (argc != 2)
	{
		MyPrintfErr("%s", "usage:\n\tserver: socket s\n\tclient: socket c\n");
		return;
	}

	if (args[1][0] == 'c')
	{
		iRet = AsClient();
	}
	else if (args[1][0] == 's')
	{
		iRet = AsServer();
	}
	else
	{
		MyPrintfErr("%s", "usage:\n\tserver: socket s\n\tclient: socket c\n");
	}

	if (0 != iRet)
	{
		MyPrintfErr("socket exec as [%s] failed...\n", args[1]);
	}

	return;
}

