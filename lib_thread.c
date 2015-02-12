#include "lib_thread.h"

int lib_thread_create(pthread_t *threadid, void *pfunction, void *arg, int flag)
{
	int ret = 0;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if(0 == flag)
	{
		pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);		
	}
	else if(1 == flag)
	{
		ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		if(ret < 0)
		{
			pthread_attr_destroy(&attr);
		    return ret;
		}
	}
 	pthread_create(threadid, &attr, (void*)pfunction, arg);
	pthread_attr_destroy(&attr);

	return ret;
}

void lib_thread_exit(void)
{
	pthread_exit((void*)0);
}

int lib_thread_cancel(pthread_t threadid)
{
	int ret = 0;
	ret = pthread_cancel(threadid);

	return ret;
}

pthread_t lib_thread_getself(void)
{
	pthread_t tid;
	tid = pthread_self();

	return tid;
}

int lib_thread_join(pthread_t threadid)
{
	int ret = 0;
	ret = pthread_join(threadid, NULL);

	return (ret == 0) ? 0 : -1;
}

int lib_mutex_init(pthread_mutex_t *mutex)
{
	int ret = 0;
	
	ret = pthread_mutex_init(mutex, NULL);

	return (ret == 0) ? 0 : -1;
}

int lib_mutex_lock(pthread_mutex_t *mutex)
{
	int ret = 0;
	ret = pthread_mutex_lock(mutex);

	return (ret == 0) ? 0 : -1;
}

int lib_mutex_unlock(pthread_mutex_t *mutex)
{
	int ret =0;
	ret = pthread_mutex_unlock( mutex);

	return (ret == 0) ? 0 : -1;
}

int lib_mutex_destroy(pthread_mutex_t *mutex)
{
	int ret = 0;
	ret = pthread_mutex_destroy(mutex);

	return (ret == 0) ? 0 : -1;
}
