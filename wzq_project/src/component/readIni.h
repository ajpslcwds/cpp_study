/*************************************************************************
      > File Name: readIni.h
      > Author: wzq
      > Created Time: Wed Oct 18 20:52:50 2017
 ************************************************************************/
#ifndef _READINI_H
#define _READINI_H

#include"common.h"

struct TSectionName
{
	string section;
	string name;
	TSectionName()
	{
		section = "";
		name = "";
	}

	bool operator <(const TSectionName &data) const
	{
		if (this->section == data.section)
		{
			return this->name < data.name;
		}
		else 
		{
			return this->section < data.section;
		}
	}
};

typedef map<TSectionName,string> MAP_SECTION;

class TReadIni
{
public:
	TReadIni();
	TReadIni(char *fileName);
	~TReadIni();

	int readInt(char *section,char *name ,int &value);
	int readDouble(char *section,char *name ,double  &value);
	int readString(char *section,char *name ,string &value);

	void display();

private:
	int init();
	int readConfigFile();
	int putData(char *section, char *name, char* value);
private:
	int iInitFlag;
	char sFileName[128];

	MAP_SECTION  mapData;	
};


#endif
