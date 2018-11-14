#include<stdio.h>

int main()
{
	union 
	{
		short value;
		char  bytes[sizeof(short)];

	} test ;

	test.value = 0x0102;

	printf("%d,%d",test.bytes[0],test.bytes[1]);
	return 0;
}
