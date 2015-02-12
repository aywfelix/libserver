#include <pthread.h>
// 0 defult thread
// 1 the detach thread
int lib_thread_create(pthread_t *threadid, void *pfunction, void *arg, int flag);
void lib_thread_exit(void);
int lib_thread_cancel(pthread_t threadid);
pthread_t lib_thread_getself(void);
int lib_thread_join(pthread_t threadid);
//静态分配：pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//动态分配：int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutex_attr_t *mutexattr);
int lib_mutex_init(pthread_mutex_t *mutex);
int lib_mutex_lock(pthread_mutex_t *mutex);
int lib_mutex_unlock(pthread_mutex_t *mutex);
int lib_mutex_destroy(pthread_mutex_t *mutex);












