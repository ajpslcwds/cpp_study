#include<iostream>
#include<string.h>
#include<errno.h>
#include<map>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

int main(int argc,char*argv[])
{

	FILE *fp  = fopen(argv[1],"r");
	if(NULL == fp)
	{
		cout<<"fopen error"<<strerror(errno)<<endl;
	}		

	map<string,string> mapName,mapVal;
	mapName.clear();
	mapVal.clear();
	map<string,string>::iterator iter;
	char sBuff[256] = {0};
	string s1 ,s2;
	while(NULL!=fgets(sBuff,255,fp) )
	{	//WSCodeSec5E.conf:memWSExSec5E.id=0x033
		char *p1=strstr(sBuff,":");
		if(p1==NULL)
		{
			cout<<sBuff<<endl;
			continue;		
		}
		char *p2=strstr(p1+1,"=");
		if(p2 == NULL)
		{
			cout<<sBuff<<endl;
			continue;		
		}
		s1.clear();
		s2.clear();
		s1  =  string(p1+1,p2-p1-1);
		s2  =  string(p2+1);
		
		s2.erase(s2.find('\n'));
		
		if((iter = mapName.find(s1))!=mapName.end())
		{	
			if(strncmp(s2.c_str(),(iter->second).c_str(),6))
				cout<<"two value:"<<s1<<","<<s2<<","<<iter->second<<endl;
		}
		else
			mapName[s1] = s2;
			
		if((iter = mapVal.find(s2))!=mapVal.end())
		{	
			if(strncmp(s1.c_str(),(iter->second).c_str(),6))
				cout<<"two key:"<<s2<<","<<s1<<","<<iter->second<<endl;
		}
		else
			mapVal[s2] = s1;
	}
	
	fclose(fp);
#if 0
	iter=mapName.begin();
	for(;iter!=mapName.end();iter++)
	{
		cout<<iter->first<<"\t"<<iter->second<<endl;
	}
	
	
	iter=mapVal.begin();
	for(;iter!=mapVal.end();iter++)
	{
		cout<<iter->first<<"\t"<<iter->second<<endl;
	}
#endif
	return 0;
}
