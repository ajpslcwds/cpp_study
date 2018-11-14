/*************************************************************************
	> File Name: bind_test.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年07月05日 星期四 11时46分52秒
 ************************************************************************/
#include<iostream>
#include<functional>

using namespace std;

int Fun(int a,int b)
{
	return a+b;
}


class A
{
public:
	int Fun(int a,int b)
	{
		return a+b;
	}
};

int main()
{
	auto b1 = bind(Fun,std::placeholders::_1,std::placeholders::_2);
	cout<<b1(10,20)<<endl;

	A a;
	auto b2 = bind(&A::Fun,a,std::placeholders::_1,std::placeholders::_2);
	cout<<b2(10,20)<<endl;

	function<int (int ,int)> f1 = bind(&A::Fun,a,std::placeholders::_1,std::placeholders::_2);
	cout<<f1(10,20)<<endl;
}
