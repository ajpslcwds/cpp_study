#include<iostream>
#include<fstream>
#include<string.h>
#include<vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

#define MAX_SIZE 1024

char g_sPath[MAX_SIZE] = { "." };
int  g_iCnt = 0;



int DealDirent()
{
	int iRet = 0;

	DIR *dir = NULL;
	struct dirent *ent = NULL;
	struct stat stFileStat;
	vector<string> vecSubDir;
	char sPath[MAX_SIZE] = { 0 };
	char sFilename[MAX_SIZE] = { 0 };
	char sFilenameOld[MAX_SIZE] = { 0 };
	char sFilenameNew[MAX_SIZE] = { 0 };



	strcpy(sPath, g_sPath);

	dir = opendir(sPath);
	if (dir == NULL)
	{
		cout << "open dir is not SUCCESS" << sPath << endl;;
		return -1;
	}
	while (true)
	{
		memset(sFilename, 0, sizeof(sFilename));
		memset(sFilenameOld, 0, sizeof(sFilenameOld));
		memset(sFilenameNew, 0, sizeof(sFilenameNew));

		ent = readdir(dir);
		if (ent == NULL)
		{
			closedir(dir);
			dir = NULL;

			return 0;
		}

		if (strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".") == 0) continue;

		strcpy(sFilenameNew, sPath);
		strcat(sFilenameNew, "/");

		strcpy(sFilenameOld, sPath);
		strcat(sFilenameOld, "/");
		strcat(sFilenameOld, ent->d_name);
		iRet = stat(sFilenameOld, &stFileStat);
		if (iRet != 0)
		{
			continue;
		}

		if (!S_ISREG(stFileStat.st_mode)) continue;

		string strName = string(ent->d_name);

		size_t iPos = strName.find('_');
		if (iPos != std::string::npos) 
			strName.erase(iPos, 2);

		size_t iPos2 = strName.find('.');
		if (iPos2 != std::string::npos) 
			strName.erase(iPos2);

		strcat(sFilenameNew, strName.c_str());
		strcat(sFilenameNew, ".pcap");

		printf("start deal file[%s]->[%s]\n", sFilenameOld, sFilenameNew);
		iRet = rename(sFilenameOld, sFilenameNew);
		if (iRet != 0)
		{
			cout << "error" << endl;
			return iRet;
		}

		g_iCnt++;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int iRet = 0;
	cout << "start!" << endl;
	if (argc > 1)
	{
		strcpy(g_sPath, argv[1]);
	}
	else
	{
		cout << "ldra path" << endl;
		cout << "\tldra /data/ldra/20170215" << endl;
		return 0;
	}


	iRet = DealDirent();
	if (iRet != 0)
	{
		cout << "DealDirent error!" << endl;
		return iRet;
	}

	cout << "total file:" << g_iCnt << endl;
	cout << "end!" << endl;

	return 0;
}
