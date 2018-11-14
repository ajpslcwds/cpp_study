/*************************************************************************
	> File Name: algorithm_test.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年06月26日 星期二 10时43分11秒
 ************************************************************************/
#include<iostream>
#include<array>
#include<boost/algorithm/cxx11/all_of.hpp>
#include<boost/algorithm/cxx11/any_of.hpp>
//#include<boost/algorithm/algorithm.hpp>
#include<boost/lambda/lambda.hpp>

using namespace std;
using namespace boost::algorithm;
using namespace boost::lambda;


void for_each_print(array<int,5>& arr)
{
	for_each(arr.begin(),arr.end(),cout<<_1<<"\t");
	cout<<endl;
}

bool f1(int x)
{
	if(x>5)
		return true;
	else 
		return false;
}
bool f2(int x)
{
	if(x%2==1)
		return true;
	else 
		return false;
}
void test_any_of()
{
	cout<<__func__<<endl;
	array<int,5> arr = {1,3,5,7,9};
	for_each_print(arr);
	bool res = false;

	res = any_of(arr.begin(),arr.end(),f1);
	cout<<res<<endl;
	res = any_of(arr.begin(),arr.end(),f2);
	cout<<res<<endl;
	res = any_of_equal(arr.begin(),arr.end(),5);
	cout<<res<<endl;
}
void test_all_of()
{
	cout<<__func__<<endl;
	array<int,7> arr = {1,3,5,7,9,9,9};
	for_each_print(arr);
	bool res = false;

	res = all_of(arr.begin(),arr.end(),f1);
	cout<<res<<endl;
	res = all_of(arr.begin(),arr.end(),f2);
	cout<<res<<endl;
	res = all_of_equal(arr.begin()+4,arr.end(),9);
	cout<<res<<endl;
}
int main()
{
	test_all_of();
	test_any_of();
	return 0;
}
