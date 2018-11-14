#include"thread_add_minus.h"

int g_sum = 0;
pthread_mutex_t mutex;
pthread_cond_t  cond;

class Hello
{
public:
	static void* add(void *args)
	{
		int i  = *((int *)args);
		bool bFlag = true;
		pthread_mutex_lock(&mutex);
		cout << "add..." << i<< "..begin.." << g_sum << endl;
		g_sum += i ;
		if (g_sum > BOUNDARY)
		{
			pthread_cond_signal(&cond);
		}
		cout << "add..." << i<< "..end.." << g_sum << endl;
		pthread_mutex_unlock(&mutex);
		sleep(1);
		pthread_exit((void*)&i);
	}

	static void* minus(void *args)
	{
		int i  = *((int *)args);

		pthread_mutex_lock(&mutex);
		cout << "minus..." << i<< "..begin.." << g_sum << endl;
		if (g_sum<=3)
		{
			pthread_cond_wait(&cond,&mutex);
		}
		g_sum--;
		cout << "minus..." << i<< "..end.." << g_sum << endl;
		pthread_mutex_unlock(&mutex);
		sleep(1);
		pthread_exit((void*)&i);
	}
};

void test_thread()
{
	int iRet  = 0;
	int i = 0;
	int indexes[NUM_THREADS];  
	pthread_t tids[NUM_THREADS];

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_mutex_init(&mutex,NULL);

	for (i = 0; i < NUM_THREADS; )
	{
		indexes[i] = i;
		iRet =  pthread_create(&tids[i], &attr, Hello::add, (void *)&(indexes[i]));
		if (iRet != 0)
		{
			cout << "pthread_create failed" << iRet << endl;
		}
		i++;

		indexes[i] = i;
		iRet =  pthread_create(&tids[i], &attr, Hello::minus, (void *)&(indexes[i]));
		if (iRet != 0)
		{
			cout << "pthread_create failed" << iRet << endl;
		}
		i++;
		//sleep(1);
	}


	void *status;
	for (i = 0; i < NUM_THREADS; i++)
	{
		iRet = pthread_join(tids[i], &status);
		if (iRet != 0)
		{
			cout<<"error pthread_join"<<iRet<<endl;
		}
		else
		{
			cout<<"result..."<<i<<"..."<< *((int*)status)<<endl;
		}
	}

	cout<<"end..."<<g_sum<<endl;
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	//pthread_exit(NULL);
}
