/*************************************************************************
	> File Name: prime.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年11月19日 星期一 16时09分23秒
 ************************************************************************/
#include<stdio.h>
#include<math.h>

long JudgePrime(long num )
{
	long base = num;
	if (num %2 == 0)
		base +=1;


	for (;;base+=2)
	{
		int s = (long)sqrt((long double)base);
		int i =0;
		for (i =3;i<=s;i+=2)
		{
			if (base %i == 0)
				break;
		}

		if (i>s)
			break;
	}
	return base;
}

int main()
{
	long base= 1;
	for (int i =1;i<15;i++)
	{
		base *=10;
		long p = JudgePrime(base);
		printf("%ld,%ld\n",base,p);
	}
	
	return 0;
}



