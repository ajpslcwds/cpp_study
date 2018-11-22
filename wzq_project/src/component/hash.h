/*************************************************************************
    > File Name: hash.h
    > Author: wzq
    > Mail:  
    > Created Time: 2018年11月14日 星期三 17时00分24秒
 ************************************************************************/
#ifndef __HASH_H__
#define __HASH_H__

#include "common.h"

enum HashResCode
{
    HASH_SUCCESS   = 0,
    HASH_MEMEORY_ALLOC_FAILED,
    HASH_INDEX_HAS_EXISTED,
    HASH_INDEX_NOT_EXIST,
};

class TVecChar
{
public:
    TVecChar() : m_size(0), m_data(NULL) { }
    ~TVecChar()
    {
        if (m_data)
        {
            delete[] m_data;
        }
    }

    int Init(int iSize)
    {
        if (m_data)
        {
            return HASH_SUCCESS;
        }

        m_size = iSize;
        m_data = new char[m_size];
        if (NULL == m_data)
        {
            return HASH_MEMEORY_ALLOC_FAILED;
        }
        return HASH_SUCCESS;
    }

    TVecChar& operator=(void *data)
    {
        memcpy(this->m_data, data, m_size);
        return *this;
    }
/*
    bool operator() (const TVecChar *vecChar1, const TVecChar *vecChar2 )
    {
        int size = vecChar1->m_size < vecChar2->m_size ? vecChar1->m_size : vecChar2->m_size;
        bool ret =  memcmp(vecChar1->m_data, vecChar2->m_data, size) < 0;
        printf("ret:%d\n",ret);
        return ret;
    }


    bool operator==(TVecChar &vecChar)
    {
        if (this->m_size == vecChar.m_size)
        {
            return memcmp(this->m_data, vecChar.m_data, this->m_size) == 0;
        }
        else return false;
    }

    bool operator<(TVecChar &vecChar)
    {
        int size = this->m_size < vecChar.m_size ? this->m_size : vecChar.m_size;
        bool ret =  memcmp(this->m_data, vecChar.m_data, size) < 0;
        printf("ret:%d\n",ret);
        return ret;
    }

    char operator [](unsigned int index)
    {
        if (index >= this->m_size) return 0;
        else return  this->m_data[index];
    }
*/
public:
    int m_size;
    char *m_data;
};

class TVecCharCmp
{
public:
    bool operator ()(const TVecChar *vecChar1, const TVecChar *vecChar2)
    {
        int size = vecChar1->m_size < vecChar2->m_size ? vecChar1->m_size : vecChar2->m_size;
        bool ret =  memcmp(vecChar1->m_data, vecChar2->m_data, size) < 0;
        return ret;
    }

};



//typedef vector<char>  TVecChar;
typedef map<TVecChar *, void *, TVecCharCmp>  TMap;
typedef vector<TMap *>  TVecMap;
typedef int (*HashFunc)(void *key, int kLen, int index);


class CIndex
{
public:
    CIndex(int iSize, int iKeyLen, int iValLen, HashFunc hashFunc);
    ~CIndex();

    int Insert(void *key, void *val);
    int Find(void *key, void **val);
    int Scan();
    int Next(void **val);
public:
    int     m_Size;  //vector size
    int		m_Count;  //count of item
    int		m_keyLen;
    int		m_ValLen;

    HashFunc    m_HashFunc;
    TVecMap     m_VecMap;

    
    int     m_SearchHashVal;
    TMap::iterator  m_begin_iter;
    TMap::iterator  m_end_iter;
};


CIndex::CIndex(int iSize, int iKeyLen, int iValLen, HashFunc hashFunc) :
    m_Size(iSize),
    m_Count(0),
    m_keyLen(iKeyLen),
    m_ValLen(iValLen),
    m_HashFunc(hashFunc)
{
    m_VecMap.reserve(iSize);

    for (int i = 0; i < iSize; i++)
    {
        m_VecMap[i] = new TMap;

    }
}

CIndex::~CIndex()
{

}

int CIndex::Find(void *key, void **val)
{
    int	iRet = HASH_SUCCESS;

    int hashCode = m_HashFunc(key, m_keyLen, 0);
    int hashVal = hashCode % m_Size;

    TVecChar *vecKey = new TVecChar;
    if (NULL == vecKey)
    {
        return HASH_MEMEORY_ALLOC_FAILED;
    }
    vecKey->Init(m_keyLen);
    *vecKey = key;


    TMap::iterator iter = m_VecMap[hashVal]->find(vecKey);
    if (iter != m_VecMap[hashVal]->end())
    {
        *val = iter->second;
        iRet = 0; //ok
    }
    else
    {
        *val = NULL;
        iRet = -1;
    }

    return iRet;
}

int CIndex::Insert(void *key, void *val)
{
    int	iRet = HASH_SUCCESS;

    int hashCode = m_HashFunc(key, m_keyLen, 0);
    int hashVal = hashCode % m_Size;

    TVecChar *vecKey = new TVecChar;
    if (NULL == vecKey)
    {
        return HASH_MEMEORY_ALLOC_FAILED;
    }
    vecKey->Init(m_keyLen);
    *vecKey = key;


    char *buffer =  new char[m_ValLen];
    if (NULL == buffer)
    {
        return HASH_MEMEORY_ALLOC_FAILED;
    }
    memcpy(buffer, val, m_ValLen);

    m_VecMap[hashVal]->insert(TMap::value_type(vecKey, buffer));
}

int CIndex::Scan()
{
    int iRet =  HASH_SUCCESS;
    m_SearchHashVal = 0;
    m_begin_iter = m_VecMap[m_SearchHashVal]->begin();
    m_end_iter=  m_VecMap[m_SearchHashVal]->end();
    return iRet;
}

int CIndex::Next(void **val)
{
    int iRet =  HASH_SUCCESS;

    while (m_begin_iter == m_end_iter)
    {
        m_SearchHashVal ++;
        if (m_SearchHashVal == m_Size)
        {
            return HASH_SUCCESS; //finish
        }
        printf("m_SearchHashVal:%d,size:%d\n",m_SearchHashVal,m_VecMap[m_SearchHashVal]->size());
        m_begin_iter = m_VecMap[m_SearchHashVal]->begin();
        m_end_iter=  m_VecMap[m_SearchHashVal]->end();
    }
     
    *val = m_begin_iter->second;
    m_begin_iter++;

    return iRet;
}


//CHash define
class CHash
{
public:
    CHash(unsigned int iHashSize);
    ~CHash();

    int BuildIndex(int iIndex, int iKeyLen, int iValLen, HashFunc hashFunc);

    int Insert(int iIndex, void  *key, void *val);
    int Find(int iIndex, void  *key, void **val);
    int Scan();
    int Next(void **val);

private:
    int     m_IndexCount;
    int     m_HashSize;
    CIndex  **m_HashIndex;

    int     m_iSearchIndex;

};


CHash::CHash(unsigned int iHashSize) :
    m_IndexCount(0),
    m_HashSize(iHashSize/1000+1),
    m_HashIndex(NULL)
{
}

CHash::~CHash()
{
}

int CHash::BuildIndex(int iIndex, int iKeyLen, int iValLen, HashFunc hashFunc)
{
    int iRet = HASH_SUCCESS;
    if (m_IndexCount <= iIndex)
    {
        CIndex  **ppHashIndex = (CIndex **)malloc(sizeof(CIndex  **) * (iIndex + 1));
        if (NULL == ppHashIndex)
        {
            return HASH_MEMEORY_ALLOC_FAILED;
        }

        memset(ppHashIndex, 0x0, sizeof(CIndex  **) * (iIndex + 1));

        if (m_HashIndex)
        {
            memcpy(ppHashIndex, m_HashIndex, sizeof(CIndex  **) * m_IndexCount);
            free(m_HashIndex);
        }

        m_HashIndex = ppHashIndex;
        m_IndexCount = iIndex + 1;

    }
    else
    {
        if (m_HashIndex[iIndex])
        {
            return HASH_INDEX_HAS_EXISTED;
        }
    }

    CIndex  *pHashIndex = new CIndex(m_HashSize, iKeyLen, iValLen, hashFunc);
    if (NULL == pHashIndex)
    {
        return  HASH_MEMEORY_ALLOC_FAILED;
    }

    m_HashIndex[iIndex] = pHashIndex;

    return iRet;
}


int CHash::Insert(int iIndex, void  *key, void *val)
{
    int iRet = HASH_SUCCESS;
    if (iIndex >= m_IndexCount)
    {
        return HASH_INDEX_NOT_EXIST;
    }

    CIndex  *pHashIndex = m_HashIndex[iIndex];
    if (NULL == pHashIndex)
    {
        return  HASH_INDEX_NOT_EXIST;
    }

    iRet = pHashIndex->Insert(key, val);

    return iRet;
}


int CHash::Find(int iIndex, void  *key, void **val)
{
    int iRet = HASH_SUCCESS;
    if (iIndex >= m_IndexCount)
    {
        return HASH_INDEX_NOT_EXIST;
    }

    CIndex  *pHashIndex = m_HashIndex[iIndex];
    if (NULL == pHashIndex)
    {
        return  HASH_INDEX_NOT_EXIST;
    }

    iRet = pHashIndex->Find(key, val);

    return iRet;
}


int CHash::Scan()
{
    int iRet = HASH_SUCCESS;
    m_iSearchIndex = 0;

    CIndex  *pHashIndex = m_HashIndex[m_iSearchIndex];
    if (NULL == pHashIndex)
    {
        return  HASH_INDEX_NOT_EXIST;
    }

    iRet = pHashIndex->Scan();
    return iRet;
}

int CHash::Next(void **val)
{
    int iRet = HASH_SUCCESS;
    m_iSearchIndex = 0;

    CIndex  *pHashIndex = m_HashIndex[m_iSearchIndex];
    if (NULL == pHashIndex)
    {
        return  HASH_INDEX_NOT_EXIST;
    }

    iRet = pHashIndex->Next(val);
    return iRet;
}



#endif //#__HASH_H__


