/*************************************************************************
	> File Name: new_test.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年07月03日 星期二 11时06分45秒
 ************************************************************************/
#include<iostream>
using namespace std;


class A
{
public:
	A (int n)
	{
		this->n = n;
		cout<<"constructor"<<endl;
	}
	~A()
	{
		cout<<"distructor"<<endl;
	}
#if 1
	void *operator new(size_t size,const char*file,int line)
	{
		cout<<"call operator new"<<file<<line<<endl;
		return malloc(size);
	}
	void *operator new(size_t size,char*file,int line,nothrow_t & nothrow_value)
	{
		cout<<"call operator new nothrow"<<file<<line<<endl;
		return malloc(size);
	}

	void* operator new (std::size_t size, void* ptr)
	{
		cout<<"call replace operator new "<<endl;
		return ptr;
	}
	void operator delete(void *ptr,const char*file,int line)
	{
		cout<<"call operator delete"<<file<<line<<endl;
		free(ptr);
	}
	void operator delete(void *ptr)
	{
		cout<<"call operator delete"<<endl;
		free(ptr);
	}
#endif 
	int n;
};
void test1()
{
	A *a  = new (__FILE__,__LINE__)A(1);
	//delete(__FILE__,__LINE__) a;
	delete a;
}

void test2()
{
	//A *p = (A*)::operator new(sizeof(A));
	A *p = (A*)A::operator new(sizeof(A),__FILE__,__LINE__);
	new(p) A(2);
	p->~A();
	//::operator delete(p);
	A::operator delete(p,__FILE__,__LINE__);
}

int main()
{
	test1();
	test2();
	return 0;
}
