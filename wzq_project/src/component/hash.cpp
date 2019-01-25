/*************************************************************************
	> File Name: hash.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年11月14日 星期三 17时00分58秒
 ************************************************************************/
#include"hash.h"
#include<sys/time.h>
#include<time.h>
char *getRandStr(int len)
{
	static char temp[128]={0};
	static int seed  = 0;

	srand(++seed);
	for (int i =0;i<len-1;i++)
	{
		int r =rand() % 26+97;
		temp[i]=r;
	}
	temp[len-1]='\0';
	return temp;
}

#pragma pack(1)
int i  = 10000000;
struct TPerson
{
	int id;
	char name[32];
	char addr[32];
	double salary;

    TPerson()
    {
		id = i++;
		strcpy(name,getRandStr(32));
		strcpy(addr,name);
		salary =1000.0;
    }
};

struct TPersonKey
{
    char name[32];
    TPersonKey( )
    {
      
    }
    void operator =(TPerson &p)
    {
        strncpy(this->name,p.name,sizeof(this->name)-1);
    }
};
#pragma pack()

int HashFunc1(void *key,int kLen,int index)
{
	TPersonKey *data = (TPersonKey *)key;

    int res = 1;
    int len =  kLen-1;

    while (len-1 >=0)
    {
        res +=(data->name)[len]*len*len;
        len /=2  ;
    }


    return res ;
}

int test_hash()
{
    int iRet ;
		int iHashSize = 1000*100;
    timeval tm_start,tm_end;

    CHash *hashtab = new CHash(iHashSize) ;
    if (NULL == hashtab)
    {
        perror("new CHash");
        return HASH_MEMEORY_ALLOC_FAILED;
    }

    hashtab->BuildIndex(0,sizeof(TPersonKey),sizeof(TPerson),HashFunc1);

    gettimeofday(&tm_start, NULL);
    TPerson arr_p[iHashSize];
    TPersonKey arr_pk[iHashSize];
    for (int i =0;i<iHashSize;i++)
    {
        arr_pk[i] = arr_p[i];
    }
    gettimeofday(&tm_end, NULL);
    printf("construct cost %ld\n", (tm_end.tv_sec - tm_start.tv_sec) *1000*1000 + (tm_end.tv_usec - tm_start.tv_usec) );

    

    gettimeofday(&tm_start, NULL);
    for (int i =0;i<iHashSize;i++)
    {
        TPerson *p = &arr_p[i];
        TPersonKey *pk = &arr_pk[i];
		TPerson *pp = NULL;
		hashtab->Find(0,pk,(void**)&pp);
		if(pp)
		{
            printf("%d:%s:%s:%f\n", pp->id, pp->name, pp->addr, pp->salary);
            printf("%d:%s:%s\n", p->id, p->name, pk->name);
			return 0;	
		}

        if ( p->id % (iHashSize/100)==0)
            printf("%d:%s:%s\n", p->id, p->name, pk->name);
        hashtab->Insert(0,pk,p);
    }
    gettimeofday(&tm_end, NULL);
    printf("insert cost %ld\n", (tm_end.tv_sec - tm_start.tv_sec) *1000*1000 + (tm_end.tv_usec - tm_start.tv_usec) );




    gettimeofday(&tm_start, NULL);
    iRet = hashtab->Scan();
    if (iRet != HASH_SUCCESS)
    {
        return iRet ;
    }

	while (1)
    {
        TPerson *pT  = NULL;
        iRet = hashtab->Next((void**)&pT);
        
        if (pT)
        {
			if (pT->id %(iHashSize/100)==0)
            printf("%d:%s:%s:%f\n", pT->id, pT->name, pT->addr, pT->salary);
        }
        else
        {
            printf("%d\n",iRet);
            break;
        }
    }
    gettimeofday(&tm_end, NULL);
    printf("scan cost %ld\n", (tm_end.tv_sec - tm_start.tv_sec) *1000*1000 + (tm_end.tv_usec - tm_start.tv_usec) );

    sleep(100);
	return 0;
}

int main()
{
	test_hash();
   
    return 0;
}

