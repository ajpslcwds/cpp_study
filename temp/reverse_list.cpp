/*************************************************************************
	> File Name: reverse_list.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2019年01月31日 星期四 10时39分21秒
 ************************************************************************/
#include<iostream>
#include<stdio.h>

using namespace std;

struct Node
{
	int data;
	Node * next;

	Node(int d):data(d),next(NULL){}
};

void display(Node *p)
{
	while (p != NULL)
	{
		printf("%d\t",p->data);
		p = p->next;
	}
	printf("\n");

	return ;

}
Node * construct(int Cnt)
{
	Node *p, *pList = NULL;
	for (int i =0;i<Cnt;i++)
	{
		Node *np = new Node(i);
		//printf("%p\n",np);
		if (pList == NULL)
		{
			pList =np;
			p =  pList;
		}
		else 
		{
			pList->next = np;
			pList = pList->next;
		}
	}
	
	return p;
}
void  distory(Node *p)
{
	Node *pt;
	while (p != NULL)
	{
		pt = p;
		p = p->next;
		//printf("%p\n",pt);
		delete pt;
		pt =NULL;
	}
	return ;
}
Node* reverse(Node *p)
{
	if (p == NULL)
		return p;
	
	Node *p1,*p2 ;
	p1 = p;

	while ((p2=p->next)!=NULL)
	{
		p->next = p2->next;
		p2->next = p1;
		p1 = p2;
	}

	return p1;
}
int main()
{
	
	Node  *p = NULL;

	printf("sizeof(Node)=%d\n",sizeof(Node));
	p = construct(6);
	display(p);
	p = reverse(p);
	display(p);
//	distory(p);

	return 0;
}
