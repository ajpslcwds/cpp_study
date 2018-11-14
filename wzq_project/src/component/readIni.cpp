/*************************************************************************
	  > File Name: readIni.cpp
	  > Author: wzq
	  > Created Time: Wed Oct 18 20:52:10 2017
 ************************************************************************/
#include "readIni.h"

TReadIni::TReadIni()
{
	iInitFlag = 0;
	strcpy(sFileName, "/home/wzq/src/bin/app.conf");

}

TReadIni::TReadIni(char *filename)
{
	iInitFlag = 0;
	strncpy(sFileName, filename, sizeof(sFileName) - 1);
}

TReadIni::~TReadIni()
{
}

int TReadIni::init()
{
	int iRet = RS_SUCCESS;
	iRet = readConfigFile();
	if (iRet != RS_SUCCESS)
	{
		MY_LOG_ERR("exec readConfigFile failed,iRet[%d]\n", iRet);
		return iRet;
	}
	iInitFlag = 1;
	return iRet;
}

int TReadIni::readConfigFile()
{
	int iRet = RS_SUCCESS;
	char sBuff[BUFFER_MAX_SIZE] = { 0 };
	char sSection[BUFFER_TMP_SIZE] = { 0 };
	char sName[BUFFER_TMP_SIZE] = { 0 };
	char sValue[BUFFER_TMP_SIZE] = { 0 };
	char *sTemp = NULL;

	mapData.clear();

	FILE *fp  = fopen(sFileName, "r");
	if (NULL == fp)
	{
		MY_LOG_ERR("fopen file[%s]error[%d][%s]\n", sFileName, errno, strerror(errno));
		return RS_COMMON;
	}

	while (true)
	{
		memset(sBuff, 0, sizeof(sBuff) - 1);
		if (fgets(sBuff, sizeof(sBuff) - 1, fp) == NULL) break;

		//cout<<"sBuff:"<<sBuff<<endl;
		if (sBuff[0] == '0' && sBuff[0] == '#')
		{
			continue;
		}

		if (sBuff[0] == '[')
		{
			memset(sSection, 0, sizeof(sSection) - 1);
			sTemp = strstr(sBuff, "]");
			if (NULL==sTemp)
			{
				MY_LOG_ERR("config file err when read [%s],not ']'!\n", sBuff);
				return RS_COMMON;
			}
			strncpy(sSection, sBuff + 1, sTemp - sBuff - 1);
		}
		else
		{
			if (sSection[0] == '0')
			{
				MY_LOG_ERR("config file err when read [%s],not section!\n", sBuff);
				return RS_COMMON;
			}

			sTemp = strstr(sBuff, "=");
			if (NULL == sTemp || sTemp == sBuff)
			{
				MY_LOG_ERR("config file err when read [%s],not '='!\n", sBuff);
				return RS_COMMON;
			}

			memset(sName, 0, sizeof(sName) - 1);
			memset(sValue, 0, sizeof(sValue) - 1);
			strncpy(sName, sBuff, sTemp - sBuff);
			strncpy(sValue, sTemp + 1, sizeof(sValue));

			iRet = putData(sSection, sName, sValue);
			if (iRet != RS_SUCCESS)
			{
				MY_LOG_ERR("exec insertData failed,iRet[%d]\n", iRet);
				return iRet;
			}
		}
	}

	return iRet;
}
int TReadIni::putData(char *section, char *name, char *value)
{
	int iRet = RS_SUCCESS;

	TSectionName tData;
	tData.section = string(section);
	tData.name = string(name);
	mapData[tData]=string(value);
	return iRet;
}

int TReadIni::readInt(char *section, char *name, int& value)
{
	int iRet = RS_SUCCESS;
	if (iInitFlag != 1)
	{
		iRet = init();
		if (iRet != RS_SUCCESS)
		{
			MY_LOG_ERR("exec init error ,iRet[%]\n", iRet);
			return iRet;
		}
	}

	TSectionName tData;
	tData.section = string(section);
	tData.name = string(name);
	string strValue = mapData[tData];
	value = atoi(strValue.c_str());

	return iRet;
}
int TReadIni::readDouble(char *section, char *name, double& value)
{
	int iRet = RS_SUCCESS;
	if (iInitFlag != 1)
	{
		iRet = init();
		if (iRet != RS_SUCCESS)
		{
			MY_LOG_ERR("exec init error ,iRet[%]\n", iRet);
			return iRet;
		}
	}

	TSectionName tData;
	tData.section = string(section);
	tData.name = string(name);
	string strValue = mapData[tData];
	value = atof(strValue.c_str());

	return iRet;
}
int TReadIni::readString(char *section, char *name, string& value)
{
	int iRet = RS_SUCCESS;
	if (iInitFlag != 1)
	{
		iRet = init();
		if (iRet != RS_SUCCESS)
		{
			MY_LOG_ERR("exec init error ,iRet[%]\n", iRet);
			return iRet;
		}
	}

	TSectionName tData;
	tData.section = string(section);
	tData.name = string(name);
	string strValue = mapData[tData];
	value = strValue;

	return iRet;
}

void TReadIni::display()
{
	if (iInitFlag != 1)
	{
		int iRet = init();
		if (iRet != RS_SUCCESS)
		{
			MY_LOG_ERR("exec init error ,iRet[%]\n", iRet);
		}
	}
	MAP_SECTION::iterator iter_map = mapData.begin();
	for (;iter_map!=mapData.end();iter_map++)
	{
		cout<<iter_map->first.section<<"\t"<<iter_map->first.name<<"\t"<<iter_map->second<<endl;
	}
}
