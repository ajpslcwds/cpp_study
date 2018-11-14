/*************************************************************************
	> File Name: function_object.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年07月05日 星期四 10时40分28秒
 ************************************************************************/
#include<iostream>
#include<functional>

using namespace std;

//typedef int(*FunPtr)(int);

int  fun1(int a)
{
	return a*a;
}


class CF
{
public:
	int operator()(int a)
	{
		return a*a*a;
	}

	int doub(int a)
	{
		return a*2;
	}
};

int main()
{
	// function pointer 
	function<int(int)>p1 = fun1;
	cout<<p1(4)<<endl;

	//function object
	CF c;
	function<int(int)> p2 =c;
	cout<<p2(3)<<endl;

	//function  member function
	function<int(CF&,int)> p3=&CF::doub  ;
	cout<<p3(c,10)<<endl;
}
