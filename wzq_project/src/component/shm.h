#ifndef _SHM_H_
#define _SHM_H_

#include"common.h"

class Shm
{
public:
	Shm();
	virtual ~Shm();
	int init(int iKey, int iSize);
	int getShmId(int iFlag);
	char* getAddr();
	char* getShm(int iFlag);
	int detach(char *addr);
	int delShm();

private:
	int iShmId;
	int iShmKey;
	int iShmSize;
};



struct SHM_CONTROL
{
	int  iInitFlag;
	int  iDir;
	int  startPos;
	int  endPos;
	int  readPos;
	int  writePos;

	void display()
	{
		printf("iDir[%d],startPos[%d],endPos[%d],readPos[%d],writePos[%d]\n",
			   iDir,startPos,endPos,readPos,writePos);
	}
};

struct SHM_DATA_HEAD
{
	int iFlag;
	int iSize;
};

class ShareMemory
{
public:
	ShareMemory();
	virtual ~ShareMemory();

	int init(int iKey, int iSize);

	int readData(SHM_DATA_HEAD *head,void *content);
	int writeData(SHM_DATA_HEAD *head,void *content);
private:
	bool judegeWrite(SHM_DATA_HEAD *head);
	bool judegeRead();
private:
	Shm *m_shm;
	char *m_addr;
	SHM_CONTROL *m_shmCtrl;


};
#endif
