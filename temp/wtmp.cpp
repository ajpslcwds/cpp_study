#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <utmp.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;

vector<string> vUsersDelete;
vector<string> vIpsDelete;
uint32_t	   timeDelete;

struct TwoString
{
	string str1;
	string str2;
};

vector<TwoString> vUsersReplace;
vector<TwoString> vIpsReplace;

vector<struct utmp> vUtmps;

void printUtmp(struct utmp &content)
{
	printf("-------------------------------\n");
	printf("ut_type = %u\n", content.ut_type);
	printf("ut_pid = %u\n", content.ut_pid);
	printf("ut_line = %s\n", content.ut_line);
	printf("ut_id = %s\n", content.ut_id);
	printf("ut_user = %s\n", content.ut_user);
	printf("ut_host = %s\n", content.ut_host);
	printf("tv_sec = %u\n", content.ut_tv.tv_sec);
	printf("tv_usec = %u\n", content.ut_tv.tv_usec);
}

void readConf(const char *conf)
{
	FILE *fpRead;
	if (conf == NULL)
		fpRead = fopen("wtmp.conf", "rb");
	else
		fpRead = fopen(conf, "rb");

	if (fpRead == NULL)
	{
		printf("%m\n");
		exit(1);
	}

	struct TwoString twoString;
	char *ptr, *pTemp;
	char *pEqual;
	string confName;
	char buff[1024];
	fgets(buff, sizeof(buff), fpRead);

	while (feof(fpRead) == 0)
	{
		if (buff[0] == '#')
		{
			fgets(buff, sizeof(buff), fpRead);
			continue;
		}
			
		if (buff[strlen(buff) - 1] == '\n')
			buff[strlen(buff) - 1] = '\0';
		if ((ptr = strstr(buff, "USER_DELETE_LIST=")))
		{
			ptr += strlen("USER_DELETE_LIST=");
			if (ptr != '\0')
			{
				ptr = strtok(ptr, ",");
				vUsersDelete.push_back(ptr);
				while ((ptr = strtok(NULL, ",")))
					vUsersDelete.push_back(ptr);
			}
		}
		else if ((ptr = strstr(buff, "IP_DELETE_LIST=")))
		{
			ptr += strlen("IP_DELETE_LIST=");
			if (ptr != '\0')
			{
				ptr = strtok(ptr, ",");
				while (ptr)
				{
					pTemp = strchr(ptr, '*');
					if (pTemp)
						*pTemp = '\0';
					vIpsDelete.push_back(ptr);
					ptr = strtok(NULL, ",");
				}
			}
		}
		else if ((ptr = strstr(buff, "TIME_DELETE=")))
		{
			ptr += strlen("TIME_DELETE=");
			if (ptr != '\0')
			{
				timeDelete = strtoul(ptr, NULL, 10);
				int day = timeDelete % 100;
				timeDelete = timeDelete / 100;
				int month = timeDelete % 100;
				int year = timeDelete / 100;
				struct tm TM = {0};
				TM.tm_year = year - 1900;
				TM.tm_mon = month - 1;
				TM.tm_mday = day;
				timeDelete = mktime(&TM);
			}
		}
		else if ((ptr = strstr(buff, "USER_REPLACE_LIST=")))
		{
			ptr += strlen("USER_REPLACE_LIST=");
			if (ptr != '\0')
			{
				ptr = strtok(ptr, ",");
				while (ptr)
				{
					pEqual = strchr(ptr, '=');
					if (pEqual)
					{
						*pEqual = '\0';
						++pEqual;
						twoString.str1 = ptr;
						twoString.str2 = pEqual;
						vUsersReplace.push_back(twoString);
					}
					ptr = strtok(NULL, ",");
				}				
			}
		}
		else if ((ptr = strstr(buff, "IP_REPLACE_LIST=")))
		{
			ptr += strlen("IP_REPLACE_LIST=");
			if (ptr != '\0')
			{
				ptr = strtok(ptr, ",");
				while (ptr)
				{
					pEqual = strchr(ptr, '=');
					if (pEqual)
					{
						*pEqual = '\0';
						++pEqual;
						twoString.str1 = ptr;
						twoString.str2 = pEqual;
						vIpsReplace.push_back(twoString);
					}
					ptr = strtok(NULL, ",");
				}				
			}
		}
		
		fgets(buff, sizeof(buff), fpRead);
	}

	fclose(fpRead);
}

int main(int argc, char **argv)
{
	printf("Version 1.0.2\n");

	readConf(argv[1]);

	FILE *fpRead = fopen("/var/log/wtmp", "rb");
	if (fpRead == NULL)
	{
		printf("%m\n");
		return 0;
	}

	struct utmp content;
	
	fread(&content, sizeof(content), 1, fpRead);

	while (feof(fpRead) == 0)
	{
		vUtmps.push_back(content);
		fread(&content, sizeof(content), 1, fpRead);
	}

	fclose(fpRead);
	
	string str;
	vector<struct utmp>::iterator iter = vUtmps.begin();
	for ( ; iter != vUtmps.end(); )
	{
        bool    bContinue = false;
		//delete
		if (iter->ut_tv.tv_sec < timeDelete)
		{
			iter = vUtmps.erase(iter);
			continue;
		}
		str = iter->ut_user;
		if (std::find(vUsersDelete.begin(), vUsersDelete.end(), str) != vUsersDelete.end())
		{
			iter = vUtmps.erase(iter);
			continue;
		}
		for (vector<string>::iterator iterIP = vIpsDelete.begin(); iterIP != vIpsDelete.end(); ++iterIP)
		{
			if (strncmp(iter->ut_host, iterIP->c_str(), iterIP->size()) == 0)
			{
				iter = vUtmps.erase(iter);
                bContinue = true;
				break;
			}
		}
        
        if (bContinue)
            continue;

		//replace
		for (vector<TwoString>::iterator iterU = vUsersReplace.begin(); iterU != vUsersReplace.end(); ++iterU)
		{
			str = iter->ut_user;
			if (iterU->str1 == str)
				strcpy(iter->ut_user, iterU->str2.c_str());
		}
		for (vector<TwoString>::iterator iterU = vIpsReplace.begin(); iterU != vIpsReplace.end(); ++iterU)
		{
			str = iter->ut_host;
			if (iterU->str1 == str)
				strcpy(iter->ut_host, iterU->str2.c_str());
		}
		++iter;
	}

	FILE *fpWrite = fopen("/var/log/wtmp", "wb");
	if (fpWrite == NULL)
	{
		printf("%m\n");
		exit(1);
	}

	for (int i = 0; i < vUtmps.size(); ++i)
		fwrite(&vUtmps[i], sizeof(vUtmps[i]), 1, fpWrite);

	fclose(fpWrite);
}
