/*************************************************************************
	> File Name: test.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2019年01月31日 星期四 17时23分16秒
 ************************************************************************/
#include<stdio.h>

class abc;
void del(abc *pobj){
 delete pobj;
}

class abc{
public:
 abc(){
  printf("abc\r\n");
 }
 ~abc(){
  printf("~abc\r\n");
 }
};


int main()
{
 abc *pobj = new abc;
 //del(pobj);
 delete pobj;
}