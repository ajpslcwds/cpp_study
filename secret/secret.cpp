/*
	complie: 	g++ secret.cpp  -g -o secret
	execute:	./secret -p ./20160311 -k 1**8
*/
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

#define MAX_SIZE 1024

char g_sPath[MAX_SIZE]={0};
int  g_iKey=-1;
int  g_iCnt=0;

//1.解析参数 
int ParasCmd(int argc,char*argv[])
{
	int iRet = 0;
	
	if(argc < 3)
		return -1;
		
	for(int i=1;i < argc; i++)
	{
		if(strcmp(argv[i],"-k") == 0)
		{
			i++;
			if(i >= argc)
			{
				cout<<"not exist key."<<endl;
				return -1;
			}
			g_iKey = atoi(argv[i]);
            if(g_iKey <= 0)
            {
                iRet = -1;
                cout<<"invalid key id "<<argv[i]<<endl;
                return iRet;
            }
		}
		else if(strcmp(argv[i],"-p") == 0)
		{
			i++;
			if(i >= argc)
			{
				cout<<"not exist path."<<endl;
				return -1;
			}
			strncpy(g_sPath,argv[i],sizeof(g_sPath)-1);
            if( strcmp(g_sPath,"")== 0)
            {
                iRet = -1;
                cout<<"invalid path "<<argv[i]<<endl;
                return iRet;
            }
		}
		else
		{
			cout<<"invalid param "<<argv[i]<<endl;
			return -1;
		}
	}
    if((g_iKey <= 0)||( strcmp(g_sPath,"")== 0))
    {
    	cout<<"path or key not input!"<<endl;
        return -1;
    }
        
	return iRet;
	
}

void ShowUse()
{
	cout<<"use help:"<<endl;
	cout<<"\tsecret -p path -k key " <<endl;
	cout<<"\tsecret -p ./20160311 -k 1234" <<endl;
}

int DoSecret(char *sPath,char *sFile)
{
	int iRet = 0;
	char sBuf[MAX_SIZE] = { 0 };
	char cTemp = 0;
	
	char sFileName[MAX_SIZE] = { 0 };
	char sFileNameNew[MAX_SIZE] = { 0 };
	char sPathNew[MAX_SIZE] = { 0 };
	strcpy(sPathNew, sPath);
	strcat(sPathNew, "_new");
	
	if( access(sPathNew,F_OK)!=0) 
	{
		if(mkdir(sPathNew,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)!=0)
		{
			printf("create path [%s] error!\n",sPathNew);
			return -1;	
		}	
	}
	
	strcpy(sFileName, sPath);
	strcat(sFileName, "/");
	strcat(sFileName, sFile);
	strcpy(sFileNameNew, sPathNew);
	strcat(sFileNameNew, "/");
	strcat(sFileNameNew, sFile);
	

	ifstream InFile(sFileName, ios::in | ios::binary);
	ofstream OutFile(sFileNameNew, ios::out | ios::binary|ios::trunc);

	if (!InFile || !OutFile)
	{
		cout << "InFile ||  OutFile  failed!" << endl;
		return -1;
	}

	while (!InFile.eof())
	{
		InFile.get(cTemp);
		//cout << cTemp << "->";
		cTemp ^= SEC_KEY;
		//cout << cTemp << endl;
		OutFile.put(cTemp);
	}
	
	InFile.close();
	OutFile.close();
	g_iCnt++;
	return iRet;
}


int DealDirent()
{
	int iRet =0;
	
	DIR* dir =NULL;
	struct dirent * ent = NULL;
	struct stat stFileStat;	
	vector<string> vecSubDir;
	char sPath[MAX_SIZE]={0};	
	char sFilename[MAX_SIZE] ={0};
	
	
	strcpy(sPath, g_sPath);
 
	dir = opendir(sPath);
	if(dir == NULL)
	{
		cout<< "open dir is not SUCCESS"<<sPath<<endl;;
		return -1;
	}
	while(true)
	{
		memset(sFilename,0,sizeof(sFilename));
		ent = readdir(dir);
		if(ent == NULL)
		{
			closedir(dir);
			dir=NULL;
			
			if(!vecSubDir.empty())
			{
				vector<string>::iterator it = vecSubDir.begin();
				strcpy(sPath,(*it).c_str());
				vecSubDir.erase(it);

				dir = opendir(sPath);
				if(dir == NULL)
				{
					printf("opendir failed:%s,%s\n",sPath,strerror(errno));
					return -1;
				}
				continue;
			}
			return 0;
		}
	 	
	 	if(strcmp(ent->d_name,"..") == 0 || strcmp(ent->d_name,".") == 0)
			continue;
		
		strcpy(sFilename,sPath);
		strcat(sFilename,"/");
		strcat(sFilename,ent->d_name);
		iRet = stat(sFilename,&stFileStat);
		if(iRet != 0)
		{
			continue;
		}		
		
		if(!S_ISREG(stFileStat.st_mode))
			continue;
				
		if(S_ISDIR(stFileStat.st_mode))
		{
			vecSubDir.push_back(sFilename );
			continue;
		}
		
		memset(sFilename,0,sizeof(sFilename));
		strcpy(sFilename,ent->d_name);
		printf("start deal file[%s/%s]\n",sPath,sFilename);
		iRet = DoSecret(sPath,sFilename);
		if (0 != iRet)
		{
			cout << "Exec Dosecret failed!" << endl;
			return iRet;
		}
	}
	return 0;	
}
int main(int argc,char*argv[])
{
	cout << "start!" << endl;
	
	int iRet =0;
	
	//1.解析参数
	iRet = ParasCmd(argc,argv);
	if(iRet !=0)
	{
		cout<<"ParamCmd error!"<<endl;
		ShowUse();
		return iRet;
	}
	//2.读取目录下的文件
	iRet = DealDirent();
	if(iRet !=0)
	{
		cout<<"DealDirent error!"<<endl;
		return iRet;
	}
	
	cout <<g_iCnt<<"个文件被处理！"<<endl;
	cout << "end!" << endl;
	
	return 0;
}

