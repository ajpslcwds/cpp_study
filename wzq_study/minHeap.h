#ifndef _MIN_HEAP_H_
#define _MIN_HEAP_H_

#include "common.h"
void myswap(int *p1,int *p2)
{
	int temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}
class MinHeap
{
public:
	MinHeap();
	~MinHeap();

	void addItem(int value);
	int  topItem();
	void display();
	void reserver();
	void refactor(int index);

	int iSize;
	int iCapacity;
	int *pContent;
};

MinHeap::MinHeap()
{
	iSize = 0;
	iCapacity = 0;
	pContent = NULL;
}
MinHeap::~MinHeap()
{
	iSize = 0;
	iCapacity = 0;
	if (pContent != NULL)
	{	
		delete [] pContent;
		pContent = NULL;
	}
}

void MinHeap::addItem(int value)
{
	if (iSize+1>iCapacity)
	{
		reserver();
	}


	*(pContent+iSize) = value;
	iSize ++;

	int i,j  = iSize-1;
	while (j)
	{	i = (j-1)/2;
		if(*(pContent+j)  <   *(pContent+i))
			myswap(pContent+j,pContent+i);
		j = i;
	}
}
int  MinHeap::topItem()
{
	int top = -1;
	if (iSize >0)
	{
		top = *(pContent);
	}


	*(pContent) = *(pContent+iSize-1);
	iSize --;
	refactor(0);

	return top;
}
void MinHeap::display()
{
	int a  = 1;
	int k = 0;
	for (int i = 0; i < iSize; i++)
	{
		k++;
		cout << "\t" << *(pContent + i);
		if (k == a)
		{
			cout << endl;
			a *= 2;
			k = 0;
		}
	}
	cout << endl;
}
void MinHeap::reserver()
{
	if (iSize+1<=iCapacity)
	{
		return ;
	}

	int iCapacityOld = iCapacity;
	iCapacity = 2*iSize + 1;
	
	int *pContTemp = pContent;
	pContent = new int[iCapacity];
	if (pContent ==  NULL)
	{
		exit(0);
	}
	memset(pContent,0,sizeof(int)*iCapacity);

	if (pContTemp != NULL)
	{	
		memcpy(pContent,pContTemp,sizeof(int)*iCapacityOld);
		delete [] pContTemp;
		pContTemp = NULL;
	}
}
void MinHeap::refactor(int index)
{
	int lChild = index *2 + 1;
	int rChild = index *2 + 2;
	int small = index ;
	if (lChild <= iSize && *(pContent+lChild) <  *(pContent+small) )
	{
		small = lChild;
	}
	if (rChild <= iSize && *(pContent+rChild) <  *(pContent+small) )
	{
		small = rChild;
	}
	
	if (small != index )
	{
		swap(*(pContent+small),*(pContent+index));
		refactor(small);
	}

	return ;

}


void testMinHeap()
{

	MinHeap minHeap;
	minHeap.addItem(15);
	minHeap.addItem(11);
	minHeap.addItem(22);
	minHeap.addItem(10);
	minHeap.addItem(8);
	minHeap.addItem(9);
	minHeap.addItem(4);
	minHeap.addItem(12);
	minHeap.addItem(3);
	minHeap.addItem(19);

	minHeap.display();



	cout<<minHeap.topItem()<<endl;
	minHeap.display();
	cout<<minHeap.topItem()<<endl;
	minHeap.display();
}
#endif 
