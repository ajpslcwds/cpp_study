#include"shm.h"
#include"readIni.h"

int main( int argc, char* args[])
{
	int iRet =  RS_SUCCESS;
	int iType = 0;
	if (argc >1)
	{
		iType = atoi(args[1]);
		if (iType != 1 && iType != 2)
		{
			MY_LOG_ERR("iType = 1 : read!\niType = 2 : write!\n")
			return 0;
		}
	}

	TReadIni readIni;
	//readIni.display();
	int iSize = 0;
	readIni.readInt("shm","size",iSize);

	ShareMemory *shmInst = new ShareMemory();

	iRet = shmInst->init(0x001,iSize);
	if (iRet != RS_SUCCESS)
	{
		MY_LOG_ERR("exec shmInst->init failed!\n")
		return iRet;
	}
	char buffer[BUFFER_MAX_SIZE]={0};
	SHM_DATA_HEAD head;
	while (true)
	{
		if (iType == 1 || iType == 0)
		{	
			memset(buffer,0,sizeof(buffer));
			memset(&head,0,sizeof(head));

			strcpy(buffer,"test-data-wzq");
			head.iFlag = 0;
			head.iSize = strlen(buffer);
			iRet = shmInst->writeData(&head,buffer);
			if (iRet != RS_SUCCESS)
			{
				MY_LOG_ERR("exec shmInst->writeData failed!\n")
				return iRet;
			}
		}

		if (iType == 2 || iType == 0)
		{
			memset(buffer,0,sizeof(buffer));
			memset(&head,0,sizeof(head));
			iRet = shmInst->readData(&head,buffer);
			if (iRet != RS_SUCCESS)
			{
				MY_LOG_ERR("exec shmInst->readData failed!\n")
				return iRet;
			}
		}
	}

	cout<<"buffer:"<<buffer<<endl;
	return iRet;
}
