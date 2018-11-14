#include "shm.h"


Shm::Shm()
{
	iShmId = 0;
	iShmKey = 0;
	iShmSize = 0;
}
Shm::~Shm()
{
}

int Shm::init(int iKey, int iSize)
{
	iShmKey = iKey;
	iShmSize = iSize;
	return RS_SUCCESS;
}

int Shm::getShmId(int iFlag)
{
	if (iShmId > 0)
	{
		return iShmId;
	}

	iShmId = shmget(iShmKey, iShmSize, iFlag);
	if (iShmId < 0)
	{
		MY_LOG_ERR("shmget error[%d][%s]\n", errno, strerror(errno));
		return RS_SUCCESS;
	}

	return iShmId;
}
char* Shm::getAddr()
{
	if (iShmId <= 0)
	{
		MY_LOG_ERR("iShmId is  error \n");
		return NULL;
	}

	char *sShm = (char *)shmat(iShmId, NULL, 0);
	if ((char *)-1 ==  sShm)
	{
		MY_LOG_ERR("shmat error[%d][%s]\n", errno, strerror(errno));
		return NULL;
	}

	return sShm;
}

char* Shm::getShm(int iFlag)
{
	if (getShmId(iFlag)<=0)
	{
		MY_LOG_ERR("getShmId is  error \n");
		return NULL;
	}

	return getAddr();
}
int Shm::detach(char *addr)
{
	int iRet = RS_SUCCESS;
	if (NULL == addr)
	{
		return RS_COMMON;
	}

	iRet = shmdt(addr);
	if (0!=iRet)
	{
		MY_LOG_ERR("shmdt error[%d][%s]\n", errno, strerror(errno));
		return iRet;
	}

	return iRet;
}

int Shm::delShm()
{
	if (iShmId <=0)
	{
		MY_LOG_ERR("iShmId is  error \n");
		return RS_COMMON;
	}

	int iRet = shmctl(iShmId, IPC_RMID, NULL);
	if (iShmId <=0)
	{
		MY_LOG_ERR("shmctl error[%d][%s]\n", errno, strerror(errno));
		return NULL;
	}

	return RS_SUCCESS;
}





ShareMemory::ShareMemory():
	m_addr(NULL),
	m_shm(NULL),
	m_shmCtrl(NULL)
{
}

ShareMemory::~ShareMemory()
{
	if (m_shm != NULL )
	{
		delete m_shm;
		m_shm = NULL;
	}
}

int ShareMemory::init(int iKey, int iSize)
{
	int iRet = RS_SUCCESS;
	if (m_shm == NULL )
	{
		m_shm  = new Shm();
		if (NULL == m_shm )
		{
			MY_LOG_ERR("alloc m_shm error[%d][%s]\n", errno, strerror(errno));
			return RS_ALLOC;
		}
	}

	m_shm->init(iKey,iSize);

	m_addr =  m_shm->getShm( IPC_MODE );
	if (m_addr == NULL)
	{
		MY_LOG_ERR("exec getShm failed!\n");
		return RS_COMMON;
	}

	m_shmCtrl = (SHM_CONTROL *)m_addr;
	if (m_shmCtrl->iInitFlag != 1 )
	{
		m_shmCtrl->startPos = sizeof(SHM_CONTROL);
		m_shmCtrl->endPos =iSize;
		m_shmCtrl->readPos = m_shmCtrl->startPos;
		m_shmCtrl->writePos = m_shmCtrl->startPos;
		m_shmCtrl->iInitFlag = 1;
		m_shmCtrl->iDir = 1;
	}

	iRet = m_shm->detach(m_addr);
	if (iRet != RS_SUCCESS)
	{
		MY_LOG_ERR("exec detach failed!\n");
		return RS_COMMON;
	}
	return RS_SUCCESS;
}

int ShareMemory::readData(SHM_DATA_HEAD *head,void *content)
{
	int iRet = RS_SUCCESS;
	m_addr =  m_shm->getShm( IPC_MODE );
	if (NULL == m_addr)
	{
		MY_LOG_ERR("m_shm->getShm failed\n");
		return 	RS_COMMON;
	}

	cout<<"ReadBefore:\t";
	m_shmCtrl->display();
	m_shmCtrl = (SHM_CONTROL *)m_addr;
	if (judegeRead())
	{
		memcpy((char*)head,m_addr + m_shmCtrl->readPos,sizeof(SHM_DATA_HEAD));
		memcpy((char*)content,m_addr + m_shmCtrl->readPos+sizeof(SHM_DATA_HEAD),head->iSize);
		memset(m_addr + m_shmCtrl->readPos,0,sizeof(SHM_DATA_HEAD) + head->iSize);
		m_shmCtrl->readPos += sizeof(SHM_DATA_HEAD) + head->iSize;
	}
	else
	{	
		MY_LOG_ERR("exec judegeRead failed!\n");
		return RS_NO_SHM;
	}
	cout<<"ReadEnd:\t";
	m_shmCtrl->display();

	iRet = m_shm->detach(m_addr);
	if (iRet != RS_SUCCESS)
	{
		MY_LOG_ERR("exec detach failed!\n");
		return RS_COMMON;
	}
	return RS_SUCCESS;
}

int ShareMemory::writeData(SHM_DATA_HEAD *head,void *content)
{
	int iRet = RS_SUCCESS;
	m_addr =  m_shm->getShm( IPC_MODE );
	if (NULL == m_addr)
	{
		MY_LOG_ERR("m_shm->getShm failed\n");
		return 	RS_COMMON;
	}

	m_shmCtrl = (SHM_CONTROL *)m_addr;

	cout<<"WriteBefore:\t";
	m_shmCtrl->display();
	if (judegeWrite(head))
	{
		memcpy(m_addr + m_shmCtrl->writePos,(char*)head,sizeof(SHM_DATA_HEAD));
		memcpy(m_addr + m_shmCtrl->writePos+sizeof(SHM_DATA_HEAD),(char*)content,head->iSize);
		m_shmCtrl->writePos += sizeof(SHM_DATA_HEAD) + head->iSize;
	}
	else
	{
		MY_LOG_ERR("exec judgeWrite failed!\n");
		return RS_NO_SHM;
	}
	cout<<"WriteEnd:\t";
	m_shmCtrl->display();

	iRet = m_shm->detach(m_addr);
	if (iRet != RS_SUCCESS)
	{
		MY_LOG_ERR("exec detach failed!\n");
		return RS_COMMON;
	}
	return RS_SUCCESS;
}


bool ShareMemory::judegeWrite(SHM_DATA_HEAD *head)
{
	int iSize = head->iSize + sizeof(SHM_DATA_HEAD);
	if ((m_shmCtrl->writePos > m_shmCtrl->readPos )|| (m_shmCtrl->iDir > 0 && m_shmCtrl->writePos ==  m_shmCtrl->readPos ))
	{
		if (m_shmCtrl->writePos + iSize <=  m_shmCtrl->endPos)
		{
			return true;
		}
		else
		{
			m_shmCtrl->writePos = m_shmCtrl->startPos;
			m_shmCtrl->iDir *= -1;
		}
	}

	if (m_shmCtrl->writePos < m_shmCtrl->readPos)
	{
		if (m_shmCtrl->writePos + iSize <= m_shmCtrl->readPos)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
		return false;
}
bool ShareMemory::judegeRead()
{
	SHM_DATA_HEAD *head;
	if ((m_shmCtrl->writePos < m_shmCtrl->readPos) || (m_shmCtrl->iDir < 0 && m_shmCtrl->writePos ==  m_shmCtrl->readPos ))
	{
		if (m_shmCtrl->readPos + sizeof(SHM_DATA_HEAD) <  m_shmCtrl->endPos)
		{
			SHM_DATA_HEAD *head = (SHM_DATA_HEAD*)(m_addr + m_shmCtrl->readPos);
			if (head->iSize >0 && m_shmCtrl->readPos + sizeof(SHM_DATA_HEAD)+ head->iSize <=  m_shmCtrl->endPos)
			{
				return true;
			}
			else
			{
				m_shmCtrl->readPos = m_shmCtrl->startPos;
				m_shmCtrl->iDir *= -1;
			}
		}
		else
		{
			m_shmCtrl->readPos = m_shmCtrl->startPos;
			m_shmCtrl->iDir *= -1;
		}
	}

	if (m_shmCtrl->writePos > m_shmCtrl->readPos)
	{
		return true;
	}
	else
	{
		return false;
	}
}
