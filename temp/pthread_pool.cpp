/*************************************************************************
	> File Name: pthread_pool.cpp
	> Author: wzq
	> Mail:  
	> Created Time: 2018年08月20日 星期一 10时52分00秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<pthread.h>

#define LIST_ADD(item,list)   do {  \
	item->prev = NULL;  \
	item->next = list;  \
	if(list) list->prev = item;  \
	list = item;  \
} while(0)

#define LIST_REMOVE(item,list)  do {  \
	if(item->prev) item->prev->next = item->next;  \
	if(item->next) item->next->prev = item->prev;  \
	if(item == list) list=item->next;  \
	item->next = item->prev = NULL;  \
} while(0)

typedef struct NJOB
{
	void (*cb_fun)(void *arg);
	void  *data;

	struct	NJOB *prev;
	struct	NJOB *next;
} nJob;

typedef struct NWORKER
{
	pthread_t  thread_id;
	int terminate;
	
	struct	NWORKERMANAGER *pool;

	struct	NWORKER *prev;
	struct	NWORKER *next;
} nWorker;

typedef struct NWORKERMANAGER
{
	pthread_mutex_t jobs_mtx;
	pthread_cond_t  jobs_cond;

	struct NWORKER *workers;
	struct NJOB	*jobs;

} nWorkerManager;

void* workThread(void *arg)
{
	nWorker *worker = (nWorker*)arg;
	
	while(1)
	{
		pthread_mutex_lock(&worker->pool->jobs_mtx);

		while(NULL==worker->pool->jobs)
		{
			if(worker->terminate) break;
			pthread_cond_wait(&worker->pool->jobs_cond,&worker->pool->jobs_mtx);
		}

		if (worker->terminate )
		{
			pthread_mutex_unlock(&worker->pool->jobs_mtx);
			break;
		}

		nJob *job = worker->pool->jobs;
		LIST_REMOVE(job,worker->pool->jobs);

		pthread_mutex_unlock(&worker->pool->jobs_mtx);

		job->cb_fun(job);

	}

	free(worker);
	pthread_exit(NULL);


}
int  createPool(nWorkerManager *pool,int pool_size)
{
	if (pool_size <1) pool_size=1;

	memset(pool,0x00,sizeof(nWorkerManager));

	pthread_mutex_t blank_mutex =  PTHREAD_MUTEX_INITIALIZER;
	memcpy(&pool->jobs_mtx,&blank_mutex,sizeof(pool->jobs_mtx));
	
	pthread_cond_t blank_cond =  PTHREAD_COND_INITIALIZER;
	memcpy(&pool->jobs_cond,&blank_cond,sizeof(pool->jobs_cond));

	for (int i =0;i<pool_size;i++)
	{
		nWorker * worker = (nWorker *)malloc(sizeof(nWorker));	
		if (NULL == worker)
		{
			perror("malloc error\n");
			return 1;
		}

		memset(worker,0x00,sizeof(worker));
		worker->terminate = 0;
		worker->pool = pool;

		int ret = pthread_create(&worker->thread_id, NULL,workThread,(void*)worker);
		if(ret)
		{
			perror("pthread_create error\n");
			free(worker);
			return 1;
		}

		LIST_ADD(worker,worker->pool->workers);
	}

	return 0;
}

void addJobToPool(nWorkerManager *pool, nJob *job)
{
	pthread_mutex_lock(&pool->jobs_mtx);
	LIST_ADD(job,pool->jobs);
	pthread_cond_signal(&pool->jobs_cond);
	pthread_mutex_unlock(&pool->jobs_mtx);
}


////////////////client test///////////////////
#define WORKER_SIZE 10
#define JOB_SIZE 1000
void  myCbFun(void *arg)
{
	nJob * job  = (nJob *)arg;
	int data = *(int*) job->data;
	printf("%d\tselfid:%1u.......\n",data,pthread_self());
	free (job->data);
	free (job);
}

int main()
{
	nWorkerManager pool;

	if (createPool(&pool,WORKER_SIZE))
	{
		perror("create pool error \n");
		return 1;
	}

	for (int i = 0; i < JOB_SIZE; i++)
	{
		nJob *job = (nJob*)malloc(sizeof(nJob));
		if (NULL == job)
		{
			perror("malloc job error\n");
			return 1;
		}

		job->cb_fun = myCbFun;
		job->data = malloc(sizeof(int));
		*(int*)job->data = i;

		addJobToPool(&pool,job);
	}

	getchar();	
	return 0;
}
