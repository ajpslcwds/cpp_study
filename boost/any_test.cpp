/*************************************************************************
	> File Name: any_test.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年06月28日 星期四 10时04分22秒
 ************************************************************************/
#include<iostream>
#include<list>
#include<boost/any.hpp>
#include <boost/foreach.hpp>

using boost::any_cast;
using namespace std;

typedef list<boost::any> many;
void append(many &values,int value)
{
	boost::any to_append = value;
	values.push_back(to_append);
}
void append(many &values,const char* value)
{
	values.push_back(value);
}
void append(many &values)
{
	values.push_back(boost::any());
}

int main()
{
	many values;

	append(values,10);
	append(values,20);
//	append(values,"100");
//	append(values);

	}

	return 0;
}
