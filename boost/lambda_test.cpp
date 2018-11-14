/*************************************************************************
	> File Name: lambda_test.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年06月25日 星期一 15时29分16秒
 ************************************************************************/
#include<iostream>
#include<boost/lambda/lambda.hpp>
#include<array>
#include<functional>

using namespace std;
using namespace boost::lambda;
int foo1(const int x)
{
	if (x%2 == 0)
		return x;
	else 
		return x*-1;
}

int main()
{
	array<int,10> arr;
	int i  =0;
	
	for_each(arr.begin(),arr.end(), _1 = 1);
	for_each(arr.begin(),arr.end(),cout<< _1<<" ");
	
	for_each(arr.begin(),arr.end(), _1 = bind(foo1,_1));
	for_each(arr.begin(),arr.end(),cout<< _1<<" ");


	return 0;
}
