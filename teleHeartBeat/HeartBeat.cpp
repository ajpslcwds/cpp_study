#include "HeartBeat.h"

ConfigInfo	conf;

HeartBeat::HeartBeat()
{
	m_iSocketId = -1;
	m_iOldSocketId = -1;
}
HeartBeat::~HeartBeat()
{
	//close(m_iSocketId);
	m_iSocketId = -1;
	m_iOldSocketId = -1;
}

int HeartBeat::Listen(char *sServerIp, int sServerPort, int iMaxConnNum)
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

int HeartBeat::Accept()
{
	int iNewSocketId;
	//struct sockaddr_in tNewAddr;
	//int iNewLen = sizeof(struct sockaddr);


	//if ((iNewSocketId = accept(m_iOldSocketId, (struct sockaddr *)&tNewAddr, (socklen_t *)&iNewLen)) < 0)
	if ((iNewSocketId = accept(m_iOldSocketId, (struct sockaddr *)NULL, (socklen_t *)NULL)) < 0)
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

int HeartBeat::Connect(char *sServerIp, int sServerPort)
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


int HeartBeat::Write(char *szBuffer, int iLen)
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

int HeartBeat::Read(char *szBuffer, int iLen)
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
////////////////////////////////////////////////////
////////////////////////////////////////////////////

int getConfigInfo()
{
	char *pRoot;
	char 	szName[256];

	strcpy(conf.sProcessName, "HeartBeat");
	memset(conf.sServerDir, 0x0, sizeof(conf.sServerDir));
	getServerPath(conf.sServerDir);

	memset(szName, 0x0, sizeof(szName));
	sprintf(szName, "%s/conf/%s.conf", conf.sServerDir, conf.sProcessName);
	pasCConfig cfg(szName);
	pRoot = NULL;
	conf.nLogType = 0xFFFFFFFF;
	cfg.cfgGetInt(pRoot, (char *)"LogLevel", &conf.nLogLevel, 4);
	cfg.cfgGetInt(pRoot, (char *)"bLog", &conf.bWapLog, 4);
	cfg.cfgGetInt(pRoot, (char *)"LogFileLen", &conf.nMaxLogSize, 4);
	cfg.cfgGetInt(pRoot, (char *)"LogIndex", &conf.nMaxLogIndex, 4);
	cfg.cfgGetInt(pRoot, (char *)"LogWType", &conf.bWriteType, 4);
	cfg.cfgGetString(pRoot, (char *)"LogDir", conf.szLogFile, 256);

	//memset(szName, 0x0, sizeof(szName));
	//sprintf(szName, "%s/%s", conf.szLogFile, conf.sProcessName);
	//net_log = new pasLLog(szName, conf.bWapLog, conf.nMaxLogSize, conf.nLogLevel, conf.nMaxLogIndex, conf.sProcessName);

	conf.stop = 0;
	conf.msgType = getpid();
	syn_global_config(&conf.stop, &conf.msgType,
					  conf.sProcessName,
					  conf.sServerDir,
					  &conf.nLogLevel,
					  conf.szLogFile,
					  &conf.nMaxLogSize);

	cfg.cfgGetInt(pRoot, (char *)"CSFlag", &conf.CSFlag, 4);
	cfg.cfgGetInt(pRoot, (char *)"SleepSec", &conf.SleepSec, 4);
	cfg.cfgGetInt(pRoot, (char *)"RetryTimes", &conf.RetryTimes, 4);

	cfg.cfgGetString(pRoot, (char *)"ServerIp1", conf.Server[0].ServerIp, IP_BUFFER_LEN);
	cfg.cfgGetInt(pRoot, (char *)"ServerPort1", &conf.Server[0].ServerPort, 4);
	cfg.cfgGetString(pRoot, (char *)"ServerIp2", conf.Server[1].ServerIp, IP_BUFFER_LEN);
	cfg.cfgGetInt(pRoot, (char *)"ServerPort2", &conf.Server[1].ServerPort, 4);

	cfg.cfgGetArray_string(pRoot, (char *)"Ethernet", conf.sEthernet[0], &conf.iEthernetNumber, sizeof(conf.sEthernet[0]), sizeof(conf.sEthernet) / sizeof(conf.sEthernet[0]));

	return 0;
}

int ExecServer()
{
	int iLen = 0;
	int iSocketId[2] = { 0 };
	int iNewSocketId[2] = { 0 };
	char sBuff[BUFF_SIZE] = { 0 };
	HeartBeat ServerDog[2];
	int maxfd;
	fd_set rfds;
	int i;

	for (i = 0; i < 2; i++)
	{
		iSocketId[i] = ServerDog[i].Listen(conf.Server[i].ServerIp, conf.Server[i].ServerPort);
		if (iSocketId[i] <= 0)
		{
			MyPrintfErr("Listen error:%s\n", strerror(errno));
			return -1;
		}
	}

	maxfd = (iSocketId[0] > iSocketId[1] ? iSocketId[0] : iSocketId[1]);
	while (1)
	{
		FD_ZERO(&rfds);
		FD_SET(iSocketId[0], &rfds);
		FD_SET(iSocketId[1], &rfds);

		select(maxfd + 1, &rfds, NULL, NULL, NULL);
		for (i = 0; i < 2; i++)
		{
			if (FD_ISSET(iSocketId[i], &rfds))
			{
				MyPrintfInf("iSocketId[%d] is readable\n", i);

				iNewSocketId[i] = ServerDog[i].Accept();
				if (iNewSocketId[i] <= 0)
				{
					MyPrintfErr("Accept error!\n");
					continue;

				}
				memset(sBuff, 0x0, BUFF_SIZE);
				iLen = ServerDog[i].Read(sBuff, BUFF_SIZE - 1);
				if (iLen <= 0)
				{
					MyPrintfErr("Read error!\n");
					continue;
				}
				MyPrintfInf("[%s:%d]recv from client:[%s] at[%s]\n", conf.Server[i].ServerIp, conf.Server[i].ServerPort, sBuff, GetCurTime());

				memset(sBuff, 0x0, BUFF_SIZE);
				strcpy(sBuff, "I AM OK !");
				iLen = ServerDog[i].Write(sBuff, strlen(sBuff));
				if (iLen <= 0)
				{
					MyPrintfErr("Write error!\n");
					continue;
				}
				MyPrintfInf("[%s:%d]send to client:[%s] at[%s]\n", conf.Server[i].ServerIp, conf.Server[i].ServerPort, sBuff, GetCurTime());

				close(iNewSocketId[i]);
				iNewSocketId[i] = -1;
			}
		}

	}
	return 0;
}

int ExecClient(ConnectInfo& connInfo)
{
	int iLen = 0;
	int iSocketId = 0;
	char sBuff[BUFF_SIZE] = { 0 };
	HeartBeat ClientDog;

	iSocketId = ClientDog.Connect(connInfo.ServerIp, connInfo.ServerPort);
	if (iSocketId <= 0)
	{
		MyPrintfErr("[%s:%d]Connect error:%s\n", connInfo.ServerIp, connInfo.ServerPort, strerror(errno));
		return -1;
	}
	memset(sBuff, 0x0, BUFF_SIZE);
	strcpy(sBuff, "ARE YOU OK ?");
	iLen = ClientDog.Write(sBuff, strlen(sBuff));
	if (iLen <= 0)
	{
		MyPrintfErr("Write error!\n");
		return -1;
	}
	MyPrintfInf("[%s:%d]send to server:[%s] at[%s]\n", connInfo.ServerIp, connInfo.ServerPort, sBuff, GetCurTime());

	memset(sBuff, 0x0, BUFF_SIZE);
	iLen = ClientDog.Read(sBuff, BUFF_SIZE - 1);
	if (iLen <= 0)
	{
		MyPrintfErr("Read error!\n");
		return -1;
	}
	MyPrintfInf("[%s:%d]recv from server:[%s] at[%s]\n", connInfo.ServerIp, connInfo.ServerPort, sBuff, GetCurTime());
	close(iSocketId);
	return 0;
}

int RunAsServer()
{
	int iRet = 0;
	iRet = ExecServer();
	if (0 != iRet)
	{
		MyPrintfErr("Exec ExecServer error!\n");
		return -1;
	}
	return 0;
}
int RunAsClient()
{
	int i = 0;
	int iRet1 = 0;
	int iRet2 = 0;
	int iRetryTimes = 0;
	int iErrorFlag = 0;  //connect error flag
	int iMainFlag = MAIN_FLAG;
	char sSys[BUFF_SIZE] = { 0 };
	FILE *fp = NULL;
	pid_t pidCnt = -1;


	while (1)
	{
		while (1)
		{ // RetryTimes
			iRet1 = ExecClient(conf.Server[0]);
			iRet2 = ExecClient(conf.Server[1]);
			if (0 == iRet1 || 0 == iRet2)
			{
				iRetryTimes = 0;
				iErrorFlag = 0;
				break;
			}
			else
			{
				iRetryTimes++;
				if (iRetryTimes > conf.RetryTimes)
				{
					MyPrintfErr("Exec ExecClient error[%d]!\n", iRetryTimes);
					iErrorFlag = 1;
					break;
				}
				else
				{
					MyPrintfErr("Exec ExecClient error[%d],will retry after[%ds]!\n", iRetryTimes, conf.SleepSec);
					sleep(conf.SleepSec);
					continue;
				}
			}
		}

		if (1 == iErrorFlag && iMainFlag == MAIN_FLAG)
		{
			for (i = 0; i < conf.iEthernetNumber; i++)
			{
				memset(sSys, 0x0, sizeof(sSys));
				//sprintf(sSys, "ifup %s", conf.sEthernet[i]);
				sprintf(sSys, "ifconfig %s up", conf.sEthernet[i]);
				MyPrintfInf("Exec cmd [%s]\n", sSys);
				system(sSys);
			}


			fp = popen(" ps -ef | grep  teleManager | grep start | grep -v grep |wc -l ", "r");
			if (NULL == fp)
			{
				MyPrintfErr("ps error!\n");
				continue;
			}

			memset(sSys, 0, sizeof(sSys) - 1);
			if (NULL == (fgets(sSys, sizeof(sSys) - 1, fp)))
			{
				MyPrintfErr("fgets ps error!\n");
				continue;
			}
			pclose(fp);
			fp = NULL;

			pidCnt = atol(sSys);
			if (pidCnt == 0)
			{
				memset(sSys, 0x0, sizeof(sSys));
				sprintf(sSys, "cd %s ; %s", conf.sServerDir, "./teleManager start");
				MyPrintfInf("Exec cmd [%s]\n", sSys);
				system(sSys);
			}
			iMainFlag = BACKUP_FLAG;//change flag
		}
		else if (0 == iErrorFlag && iMainFlag == BACKUP_FLAG)
		{
			for (i = 0; i < conf.iEthernetNumber; i++)
			{
				memset(sSys, 0x0, sizeof(sSys));
				//sprintf(sSys, "ifdown %s", conf.sEthernet[i]);
				sprintf(sSys, "ifconfig %s down", conf.sEthernet[i]);
				MyPrintfInf("Exec cmd [%s]\n", sSys);
				system(sSys);
			}
			memset(sSys, 0x0, sizeof(sSys));
			sprintf(sSys, "cd %s ; %s", conf.sServerDir, "./teleManager quick");
			MyPrintfInf("Exec cmd [%s]\n", sSys);
			system(sSys);

			iMainFlag = MAIN_FLAG;//change flag
		}

		sleep(conf.SleepSec); //sleep after success
	}
	return 0;
}


void iSignal_handler(int sig)
{
	if (sig == SIGINT)
	{
		//PAS_LOG_DEG("recv SIGINT");
		conf.iSignal = SIGINT;
	}
	else if (sig == SIGQUIT)
	{
		//PAS_LOG_DEG("recv SIGQUIT");
		conf.iSignal = SIGQUIT;
	}
	else if (sig == SIGTERM)
	{
		//PAS_LOG_DEG("recv SIGTERM");
		conf.iSignal = SIGTERM;
	}
	else if (sig == SIGTSTP)
	{
		//PAS_LOG_DEG("recv SIGTSTP");
		conf.iSignal = SIGTSTP;
	}
	else if (sig == SIGPIPE)
	{
		conf.iSignal = SIGPIPE;
	}
}



int main(int argc, char **argv)
{
	int iRet = 0;
	iRet = getConfigInfo();
	if (0 != iRet)
	{
		MyPrintfErr("Exec getConfigInfo error!\n");
		return -1;
	}

#ifdef DEBUG
	if (argc > 1)
	{
		conf.CSFlag = 0;
	}
#endif


	::signal(SIGPIPE, iSignal_handler);
	/*
	::signal(SIGINT, iSignal_handler);
	::signal(SIGQUIT, iSignal_handler);
	::signal(SIGTERM, iSignal_handler);
	::signal(SIGTSTP, iSignal_handler);


	::signal(SIGHUP, SIG_IGN);
	::signal(SIGUSR1, SIG_IGN);
	::signal(SIGUSR2, SIG_IGN);
	::signal(SIGCHLD, SIG_IGN);
	::signal(SIGPIPE, SIG_IGN);
  
	*/



	if (conf.CSFlag == SERVER_FLAG)
	{
		iRet = RunAsServer();
		if (0 != iRet)
		{
			MyPrintfErr("Exec RunAsServer error!\n");
			return -1;
		}
	}
	else // if (conf.CSFlag ==CLIENT_FLAG)
	{
		iRet = RunAsClient();
		if (0 != iRet)
		{
			MyPrintfErr("Exec RunAsClient error!\n");
			return -1;
		}
	}

	return 0;
}
