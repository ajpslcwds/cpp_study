#ifndef _MYSTRING_H_
#define _MYSTRING_H_


#include "common.h"

class mystring
{
public:
	mystring(char *str = NULL);
	mystring(const mystring& src);
	mystring& operator = (const mystring& src);

	virtual ~mystring();

	void output()
	{
		cout<<m_data<<endl;
	}
private:
	char *m_data;
};

void test_mystring();
#endif //_MYSTRING_H_
