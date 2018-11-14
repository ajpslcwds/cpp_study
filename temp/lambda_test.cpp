/*************************************************************************
	> File Name: test.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年07月04日 星期三 10时54分23秒
 ************************************************************************/
#include<iostream>
#include<vector>
#include<algorithm>
#include<functional>

using namespace std;

void test1()
{
	auto f  = [](int x,int y){return x + y;};
	cout<<f(3,4)<<endl;

	int n = [](int x,int y)->int{return x + y;}(5,4);
	cout<<n<<endl;

	int  m  = [](int x )
			{return [](int y){return y*2;}(x)+3;}(5);
	cout<<m<<endl;

	function<int(int,int) >f2 = [](int x,int y){return x + y;};
	cout<<f2(3,4)<<endl;
}
void test2()
{
	//return another lambda expression
	auto g = [](int x)->function<int(int)>
	{ return [=](int y){return x+y;};};

	//use lambda expression as an argument
	auto h = [](const function<int(int)>&f,int z)
	{ return f(z)+1;};

	auto a = h(g(7),8);
	cout<<a<<endl;
}

void test3()
{
	vector<int> vec;
	for (int i =0;i<10;i++)
		vec.push_back(i);

	for_each(vec.begin(),vec.end(),[](int i){cout<<"\t"<<i;});
	cout<<endl;	
	
	//change value
	for_each(vec.begin(),vec.end(),[](int &i){i*=i;});
	for_each(vec.begin(),vec.end(),[](int i){cout<<"\t"<<i;});

}

int main()
{
	test1();
	test2();
	test3();
	return 0;
}

