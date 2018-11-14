#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define MAX_SIZE 1024
#define USE_SIZE 100

#define FILE_PATH "/home/hu_file"	//待处理的文件路径
#define SAVE_FILE_PATH "/home/saveFile"	//处理后的文件路径

//MRE文件头
#define MRETableHead "objectid,MmeUeS1apId,MmeGroupId,MmeCode,TimeStamp,EventType,MR.LteScRSRP,MR.LteScRSRQ,MR.LteScEarfcn,MR.LteScPci,MR.LteNcRSRP,MR.LteNcRSRQ,MR.LteNcEarfcn,MR.LteNcPci,MR.TdsPccpchRSCP,MR.TdsNcellUarfcn,MR.TdsCellParameterId,MR.GsmNcellBcch,MR.GsmNcellCarrierRSSI,MR.GsmNcellNcc,MR.GsmNcellBcc"
//MRO文件头1
#define MROTableHead "objectid,MmeUeS1apId,MmeGroupId,MmeCode,TimeStamp,MR.LteScRSRP,MR.LteScRSRQ,MR.LteScTadv,MR.LteSceNBRxTxTimeDiff,MR.LteScPHR,MR.LteScAOA,MR.LteScSinrUL,MR.LteScEarfcn,MR.LteScPci,MR.LteNcRSRP,MR.LteNcRSRQ,MR.LteNcEarfcn,MR.LteNcPci,MR.TdsPccpchRSCP,MR.TdsNcellUarfcn,MR.TdsCellParameterId,MR.GsmNcellBcch,MR.GsmNcellCarrierRSSI,MR.GsmNcellNcc,MR.GsmNcellBcc,MR.LteScPUSCHPRBNum,MR.LteScPDSCHPRBNum,MR.LteScBSR,MR.LteScRI1,MR.LteScRI2,MR.LteScRI4,MR.LteScRI8,OVERLAPP_COVERAGE,MOD3_NCELLS,OVERSHOOT_AOA"

#define MRE	0
#define MRO 1

int OVERLAPP_COVERAGE = 0;
int MOD3_NCELLS = 0;
int OVERSHOOT_AOA = 0;

//MRE文件结构体
struct MREfile
{
char strNoUse1[USE_SIZE]; 
char strNoUse2[USE_SIZE];
char objectId1[USE_SIZE];
char strNoUse3[USE_SIZE];
char MmeUeS1apId2[USE_SIZE];
char strNoUse4[USE_SIZE];
char MmeGroupId3[USE_SIZE];
char strNoUse5[USE_SIZE];
char MmeCode4[USE_SIZE];
char strNoUse6[USE_SIZE];
char TimeStamp5[USE_SIZE];
char strNoUse7[USE_SIZE];
char EventType6[USE_SIZE];

//MRE无标题字段
char strNoUse8[USE_SIZE];
char MR_LteScRSRP1[USE_SIZE];
char MR_LteScRSRQ2[USE_SIZE];
char MR_LteScEarfcn3[USE_SIZE];
char MR_LteScPci4[USE_SIZE];
char MR_LteNcRSRP5[USE_SIZE];
char MR_LteNcRSRQ6[USE_SIZE];
char MR_LteNcEarfcn7[USE_SIZE];
char MR_LteNcPci8[USE_SIZE];
char MR_TdsPccpchRSCP9[USE_SIZE];
char MR_TdsNcellUarfcn10[USE_SIZE];
char MR_TdsCellParameterId11[USE_SIZE];
char MR_GsmNcellBcch12[USE_SIZE];
char MR_GsmNcellCarrierRSSI13[USE_SIZE];
char MR_GsmNcellNcc14[USE_SIZE];
char MR_GsmNcellBcc15[USE_SIZE];
char strNoUse9[USE_SIZE];
};

//MRO文件结构体
struct MROfile
{
char strNouse1[USE_SIZE];
char strNouse2[USE_SIZE];
char objectid[USE_SIZE];
char strNouse3[USE_SIZE];
char MmeUeS1apId[USE_SIZE];
char strNouse4[USE_SIZE];
char MmeGroupId[USE_SIZE];
char strNouse5[USE_SIZE];
char MmeCode[USE_SIZE];
char strNouse6[USE_SIZE];
char TimeStamp[USE_SIZE];

char strNouse7[USE_SIZE];
char LteScRSRP[USE_SIZE];
char LteScRSRQ[USE_SIZE];
char LteScTadv[USE_SIZE];
char LteSceNBRxTxTimeDiff[USE_SIZE];
char LteScPHR[USE_SIZE];
char LteScAOA[USE_SIZE];
char LteScSinrUL[USE_SIZE];
char LteScEarfcn[USE_SIZE];
char LteScPci[USE_SIZE];
char LteNcRSRP[USE_SIZE];
char LteNcRSRQ[USE_SIZE];
char LteNcEarfcn[USE_SIZE];
char LteNcPci[USE_SIZE];
char TdsPccpchRSCP[USE_SIZE];
char TdsNcellUarfcn[USE_SIZE];
char TdsCellParameterId[USE_SIZE];
char GsmNcellBcch[USE_SIZE];
char GsmNcellCarrierRSSI[USE_SIZE];
char GsmNcellNcc[USE_SIZE];
char GsmNcellBcc[USE_SIZE];
char LteScPUSCHPRBNum[USE_SIZE];
char LteScPDSCHPRBNum[USE_SIZE];
char LteScBSR[USE_SIZE];
char LteScRI1[USE_SIZE];
char LteScRI2[USE_SIZE];
char LteScRI4[USE_SIZE];
char LteScRI8[USE_SIZE];
char strNouse8[USE_SIZE];
int OVERLAPP_COVERAGE;
int MOD3_NCELLS;
int OVERSHOOT_AOA;
};

DIR* dir =NULL;
vector<MREfile> vecMRE;
vector<MROfile> vecMRO;

//解压文件
void gunZipFile(void)
{
	char commandStr[MAX_SIZE] = {0};
	
	//构造命令
	strcpy(commandStr, "gunzip ");
	strcat(commandStr,FILE_PATH);
	strcat(commandStr,"/*.gz");
	
	//执行命令
	system(commandStr);
}

//获取目录列表
int getDirectoryList(void)
{
	char sPath[MAX_SIZE]={0};	

	strcpy(sPath, FILE_PATH);
	dir = opendir(sPath);
	
	if(dir == NULL)
	{
		cout<< "open dir is not SUCCESS"<<sPath<<endl;;
		exit(0);
	}
	
	return 0;	
}
//判断文件类型
int judgeFileType(char * fileName)
{
	char *p = fileName;
	
	while((*p) != 'R')
		p++;
		
	p++;
	if((*p) == 'E')
		return MRE;
		
	if((*p) == 'O')	
		return MRO;
		
	return -1;
}
//微处理文件
int deleteSomething(char *sFilename)
{
	char commandStr[MAX_SIZE] = {0};
	
	//构造命令,去掉”
	strcpy(commandStr, "sed -i 's#\"# #g' ");
	strcat(commandStr,sFilename);
	system(commandStr);
	
	//去掉行空格
	memset(commandStr,0,sizeof(commandStr));
	strcpy(commandStr, "sed -i 's#^[[:space:]]*##g' ");
	strcat(commandStr,sFilename);
	system(commandStr);
	
	//把=号替换成空格
	memset(commandStr,0,sizeof(commandStr));
	strcpy(commandStr, "sed -i 's#=# #g' ");
	strcat(commandStr,sFilename);
	system(commandStr);
	
	//把>都去掉
	memset(commandStr,0,sizeof(commandStr));
	strcpy(commandStr, "sed -i 's#># #g' ");
	strcat(commandStr,sFilename);
	system(commandStr);
}

//产生csv文件
int createCsvFile(int flag,char *sFilename)
{
	char *p1 = sFilename ;
	char *p2 = p1 + 1;
	char *p3 = NULL;
	char csvPathAddName[MAX_SIZE] = {0};
	char csvName[MAX_SIZE] = {0};
	char *p4 = csvName;
	char MREHead[MAX_SIZE] = {0};
	char MROHead[MAX_SIZE] = {0};

	//构造文件名
	while(!((*p1) == '/' && (*p2) == 'T'))
	{
			p1++;
			p2++;
	}
	p3 = p2;
	while((*p3) != '.')
		p3++;
	while(p2 != p3)
	{
		*p4 = *p2;
		p4++;
		p2++;
	}
	strcat(p4,".csv");
	strcat(csvPathAddName,SAVE_FILE_PATH);
	strcat(csvPathAddName,"/");
	strcat(csvPathAddName,csvName);
	
	//创建并打开文件
	FILE *fp  = fopen(csvPathAddName,"w");
	if(NULL == fp)
	{
		cout<<"create "<< csvPathAddName << "err" <<endl;
		return 0;
	}
	
	//写入文件
	if(flag == MRE)
	{
		strcpy(MREHead,MRETableHead);
		fprintf(fp,"%s\n",MREHead);
		
		vector<MREfile>::iterator iter;  
    for (iter=vecMRE.begin();iter!=vecMRE.end();iter++)  
    {  
          fprintf(fp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",iter->objectId1,iter->MmeUeS1apId2,iter->MmeGroupId3,iter->MmeCode4,iter->TimeStamp5,iter->EventType6,iter->MR_LteScRSRP1,iter->MR_LteScRSRQ2,iter->MR_LteScEarfcn3,iter->MR_LteScPci4,iter->MR_LteNcRSRP5,iter->MR_LteNcRSRQ6,iter->MR_LteNcEarfcn7,iter->MR_LteNcPci8,iter->MR_TdsPccpchRSCP9,iter->MR_TdsNcellUarfcn10,iter->MR_TdsCellParameterId11,iter->MR_GsmNcellBcch12,iter->MR_GsmNcellCarrierRSSI13,iter->MR_GsmNcellNcc14,iter->MR_GsmNcellBcc15);
    }  
	}
	else
	{
		strcpy(MROHead,MROTableHead);
		fprintf(fp,"%s\n",MROHead);
		
		vector<MROfile>::iterator iter;  
    for (iter=vecMRO.begin();iter!=vecMRO.end();iter++)  
    {  
    	fprintf(fp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%d,%d\n",iter->objectid,iter->MmeUeS1apId,iter->MmeGroupId,iter->MmeCode,iter->TimeStamp,iter->LteScRSRP,iter->LteScRSRQ,iter->LteScTadv,iter->LteSceNBRxTxTimeDiff,iter->LteScPHR,iter->LteScAOA,iter->LteScSinrUL,iter->LteScEarfcn,iter->LteScPci,iter->LteNcRSRP,iter->LteNcRSRQ,iter->LteNcEarfcn,iter->LteNcPci,iter->TdsPccpchRSCP,iter->TdsNcellUarfcn,iter->TdsCellParameterId,iter->GsmNcellBcch,iter->GsmNcellCarrierRSSI,iter->GsmNcellNcc,iter->GsmNcellBcc,iter->LteScPUSCHPRBNum,iter->LteScPDSCHPRBNum,iter->LteScBSR,iter->LteScRI1,iter->LteScRI2,iter->LteScRI4,iter->LteScRI8,iter->OVERLAPP_COVERAGE,iter->MOD3_NCELLS,iter->OVERSHOOT_AOA);
    }  
	}
	fclose(fp);
}

//处理文件
int dealingFile(char *sFilename,int flag)
{
	if(flag == -1)
	{
		cout << "the file's type " << sFilename << " is not needed deal" << endl;
		return 0;
	}
	char sBuff[MAX_SIZE] = {0};
	MREfile strcMREBuf;
	MROfile strcMROBuf;
	
	//微处理文件
	deleteSomething(sFilename);
	
	//打开文件
	FILE *fp  = fopen(sFilename,"r");
	if(NULL == fp)
	{
		cout<<"fopen error "<< sFilename <<endl;
		return 0;
	}		

	//处理文件
	if(flag == MRE)
	{
			while(NULL!=fgets(sBuff,MAX_SIZE,fp) )
			{
				if(strstr(sBuff,"<object") != NULL)
				{
						memset(&strcMREBuf,0,sizeof(strcMREBuf));
						sscanf(sBuff,"%s %s %s %s %s %s %s %s %s %s %s %s %s",strcMREBuf.strNoUse1,strcMREBuf.strNoUse2,strcMREBuf.objectId1,strcMREBuf.strNoUse3,strcMREBuf.MmeUeS1apId2,strcMREBuf.strNoUse4,strcMREBuf.MmeGroupId3,strcMREBuf.strNoUse5,strcMREBuf.MmeCode4,strcMREBuf.strNoUse6,strcMREBuf.TimeStamp5,strcMREBuf.strNoUse7,strcMREBuf.EventType6);
				}
				else if(strstr(sBuff,"<v") != NULL)
				{
							MREfile strcMrePushed;
							memcpy(&strcMrePushed,&strcMREBuf,sizeof(strcMREBuf));
							sscanf(sBuff,"%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",strcMrePushed.strNoUse8,strcMrePushed.MR_LteScRSRP1,strcMrePushed.MR_LteScRSRQ2,strcMrePushed.MR_LteScEarfcn3,strcMrePushed.MR_LteScPci4,strcMrePushed.MR_LteNcRSRP5,strcMrePushed.MR_LteNcRSRQ6,strcMrePushed.MR_LteNcEarfcn7,strcMrePushed.MR_LteNcPci8,strcMrePushed.MR_TdsPccpchRSCP9,strcMrePushed.MR_TdsNcellUarfcn10,strcMrePushed.MR_TdsCellParameterId11,strcMrePushed.MR_GsmNcellBcch12,strcMrePushed.MR_GsmNcellCarrierRSSI13,strcMrePushed.MR_GsmNcellNcc14,strcMrePushed.MR_GsmNcellBcc15,strcMrePushed.strNoUse9);
							vecMRE.push_back(strcMrePushed);
				}
				else
				{
						memset(sBuff,0,sizeof(sBuff));
				}
			}
			createCsvFile(MRE,sFilename);
	}
	
	if(flag == MRO)
	{
			while(NULL!=fgets(sBuff,MAX_SIZE,fp) )
			{
				if(strstr(sBuff,"<object") != NULL)
				{
						memset(&strcMROBuf,0,sizeof(strcMROBuf));
						OVERLAPP_COVERAGE = 0;
						MOD3_NCELLS = 0;
						sscanf(sBuff,"%s %s %s %s %s %s %s %s %s %s %s",strcMROBuf.strNouse1,strcMROBuf.strNouse2,strcMROBuf.objectid,strcMROBuf.strNouse3,strcMROBuf.MmeUeS1apId,strcMROBuf.strNouse4,strcMROBuf.MmeGroupId,strcMROBuf.strNouse5,strcMROBuf.MmeCode,strcMROBuf.strNouse6,strcMROBuf.TimeStamp);
				}
				else if(strstr(sBuff,"<v") != NULL)
				{
							MROfile strcMroPushed;
							memcpy(&strcMroPushed,&strcMROBuf,sizeof(strcMROBuf));
							sscanf(sBuff,"%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",strcMroPushed.strNouse7,strcMroPushed.LteScRSRP,strcMroPushed.LteScRSRQ,strcMroPushed.LteScTadv,strcMroPushed.LteSceNBRxTxTimeDiff,strcMroPushed.LteScPHR,strcMroPushed.LteScAOA,strcMroPushed.LteScSinrUL,strcMroPushed.LteScEarfcn,strcMroPushed.LteScPci,strcMroPushed.LteNcRSRP,strcMroPushed.LteNcRSRQ,strcMroPushed.LteNcEarfcn,strcMroPushed.LteNcPci,strcMroPushed.TdsPccpchRSCP,strcMroPushed.TdsNcellUarfcn,strcMroPushed.TdsCellParameterId,strcMroPushed.GsmNcellBcch,strcMroPushed.GsmNcellCarrierRSSI,strcMroPushed.GsmNcellNcc,strcMroPushed.GsmNcellBcc,strcMroPushed.LteScPUSCHPRBNum,strcMroPushed.LteScPDSCHPRBNum,strcMroPushed.LteScBSR,strcMroPushed.LteScRI1,strcMroPushed.LteScRI2,strcMroPushed.LteScRI4,strcMroPushed.LteScRI8,strcMroPushed.strNouse8);
							if((atoi(strcMroPushed.LteScRSRP) - atoi(strcMroPushed.LteNcRSRP) < 6) &&  (atoi(strcMroPushed.LteScEarfcn) == atoi(strcMroPushed.LteNcEarfcn)))
							{	
								OVERLAPP_COVERAGE++;
								if((atoi(strcMroPushed.LteScPci) % 3) == (atoi(strcMroPushed.LteNcPci) % 3))
								{
									MOD3_NCELLS++;
								}
							}
							if((atoi(strcMroPushed.LteScAOA)/2)<-45 || (atoi(strcMroPushed.LteScAOA)/2) > 45)
							{
								OVERSHOOT_AOA = 0;
							}
							else
							{
								OVERSHOOT_AOA = 1;
							}
							strcMroPushed.OVERLAPP_COVERAGE = OVERLAPP_COVERAGE;
							strcMroPushed.MOD3_NCELLS = MOD3_NCELLS;
							strcMroPushed.OVERSHOOT_AOA = OVERSHOOT_AOA;
							vecMRO.push_back(strcMroPushed);
				}
				else if(strstr(sBuff,"</measurement") != NULL)
				{
						break;
				}
				else
				{
						memset(sBuff,0,sizeof(sBuff));
				}
			}
			createCsvFile(MRO,sFilename);
	}
	fclose(fp);
}

//主函数	
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
			printf("Paramter err!!\n The format is ./dealFile filePath\n");
			exit(0);
	}
	char commandStr[MAX_SIZE] = {0};
	
	//构造命令,建立文件夹
	strcpy(commandStr, "mkdir -p ");
	strcat(commandStr,FILE_PATH);
	system(commandStr);
	
	strcpy(commandStr, "mkdir -p ");
	strcat(commandStr,SAVE_FILE_PATH);
	system(commandStr);
	
	//判断输入的路径的形式
	char *pBuff = argv[1];
	while(*pBuff != '\0')
			pBuff++;
	pBuff -=1;
	//copy 待处理文件
	strcpy(commandStr, "rm -rf ");
	strcat(commandStr,FILE_PATH);
	system(commandStr);
	memset(commandStr,0,sizeof(commandStr));

	strcpy(commandStr, "cp ");
	strcat(commandStr,argv[1]);
	if(*pBuff == '/')
	{
		strcat(commandStr,"* ");	
	}
	else
	{
		strcat(commandStr,"/* ");	
	}
	strcat(commandStr,FILE_PATH);
	system(commandStr);
	
	char sFilename[MAX_SIZE] ={0};
	struct dirent * ent = NULL;
	int fileType = 2;
	
	//解压文件
	gunZipFile();
	 
	//获取目录列表
	getDirectoryList();
	
	//循环处理文件
	while(true)
	{
		memset(sFilename,0,sizeof(sFilename));
		ent = readdir(dir);
		
		//判断文件是否已经处理完毕
		if(ent == NULL)
		{
			closedir(dir);
			dir=NULL;	
			cout << "have dealed all files " << endl;		
			break;
		}
	 	
	 	//跳过一些目录
	 	if(strcmp(ent->d_name,"..") == 0 || strcmp(ent->d_name,".") == 0)
			continue;
		
		//构造要处理的文件名
		strcpy(sFilename,FILE_PATH);
		strcat(sFilename,"/");
		strcat(sFilename,ent->d_name);
		
		//判断文件类型
		fileType = judgeFileType(ent->d_name);
		//处理文件
		dealingFile(sFilename,fileType);
	}
	
	closedir(dir);
	
	//删除文件处理完的源文件
	printf("delete hu_file.........\n");
	strcpy(commandStr, "cd /home");
	system(commandStr);
	strcpy(commandStr, "rm -rf hu_file");
	system(commandStr);
	return 0;	
}
	
