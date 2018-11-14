#include"mystring.h"

mystring::mystring(char *str)
{
	cout<<"mystring"<<endl;
	if (NULL == str)
	{
		m_data = new char[1];
		assert(m_data);
		*m_data = '\0';
	}
	else
	{
		int iLen =  strlen(str);
		m_data = new char[iLen + 1];
		assert(m_data);
		strncpy(m_data, str, iLen);
		*(m_data + iLen) = '\0';
	}
}

mystring::mystring(const mystring& src)
{
	cout<<"mystring copy"<<endl;
	int iLen =  strlen(src.m_data);
	m_data = new char[iLen + 1];
	assert(m_data);
	strncpy(m_data, src.m_data, iLen);
	*(m_data + iLen) = '\0';
}

mystring& mystring::operator = (const mystring& src)
{
	cout<<"mystring operator"<<endl;
	if (this ==  & src)
	{
		return *this;
	}

	delete [] m_data;
	int iLen =  strlen(src.m_data);
	m_data = new char[iLen + 1];
	assert(m_data);
	strncpy(m_data, src.m_data, iLen);
	*(m_data + iLen) = '\0';

	return *this;
}

mystring::~mystring()
{
	if (NULL != m_data)
	{
		delete[] m_data;
		m_data = NULL;
	}
}


void test_mystring()
{
	mystring m1(NULL);
	m1.output();

	mystring m2((char*)"I am in SH!");
	m2.output();

	mystring m3(m2);//copy
	m3.output();

	mystring m4 = m2;//copy
	m4.output();

	m1 = m2;//operator
	m1.output();
}
