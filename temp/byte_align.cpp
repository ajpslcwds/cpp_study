/*************************************************************************
	> File Name: byte_align.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年07月03日 星期二 15时46分39秒
 ************************************************************************/
#include<iostream>
using namespace std;

struct A
{
	    int a;
		    char b;
			    short c;
};

struct B
{
	    char a;
		    int b;
			    short c;
};

#pragma pack(2)
struct C
{
	    char a;
		    int b;
			    short c;
};

#pragma pack(1)
struct D
{
	    int a;
		    char b;
			    short c;
};

int main()
{
	cout<<"sizeof(int)="<<sizeof(int)<<endl;
	cout<<"sizeof(A)="<<sizeof(A)<<endl;
	cout<<"sizeof(B)="<<sizeof(B)<<endl;
	cout<<"sizeof(C)="<<sizeof(C)<<endl;
	cout<<"sizeof(D)="<<sizeof(D)<<endl;


	return 0;
}
