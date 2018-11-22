/*************************************************************************
	> File Name: vector_test.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年11月21日 星期三 11时05分59秒
 ************************************************************************/
#include<stdio.h>
#include<iostream>
#include<vector>
#include<string>
#include<stdlib.h>
#include<string.h>
#include<time.h>
using namespace std;

char *getRandStr(int len)
{
	static char temp[128]={0};

	srand(rand());
	for (int i =0;i<len-1;i++)
	{
		int r =rand() % 26+97;
		temp[i]=r;
	}
	temp[len-1]='\0';
	return temp;
}


int main()
{
	for (int c = 0;c<10;c++)

	{
		char temp[100];
		strcpy(temp,getRandStr(10));
		printf("%s\n",temp);

	}
	return 0;
}
