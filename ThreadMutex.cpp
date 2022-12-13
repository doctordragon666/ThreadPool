#include "ThreadMutex.h"

int ThreadMutex::thread_mutex_create(pthread_mutex_t* mtx)
{
	int  err;
	pthread_mutexattr_t  attr;//锁属性

	err = pthread_mutexattr_init(&attr);//初始化
	if (err != 0) {
		fprintf(stderr, "pthread_mutexattr_init() failed, reason: %s\n", strerror(errno));
		return ERROR;
	}

	//检查锁，如果同一个线程请求一个锁，，不允许多次加锁，简单死锁现象的杜绝
	//PTHREAD_MUTEX_ERRORCHECK：普通锁，其他线程请求锁请求形成队列，保证资源分配公平
	err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
	if (err != 0) {
		fprintf(stderr, "pthread_mutexattr_settype(PTHREAD_MUTEX_ERRORCHECK) failed, reason: %s\n", strerror(errno));
		return ERROR;
	}

	err = pthread_mutex_init(mtx, &attr);//使用属性初始化
	if (err != 0) {
		fprintf(stderr, "pthread_mutex_init() failed, reason: %s\n", strerror(errno));
		return ERROR;
	}

	err = pthread_mutexattr_destroy(&attr);//释放
	if (err != 0) {
		fprintf(stderr, "pthread_mutexattr_destroy() failed, reason: %s\n", strerror(errno));
	}

	return OK;
}

int ThreadMutex::thread_mutex_destroy(pthread_mutex_t* mtx)
{
	int  err;

	err = pthread_mutex_destroy(mtx);
	if (err != 0) {
		fprintf(stderr, "pthread_mutex_destroy() failed, reason: %s\n", strerror(errno));
		return ERROR;
	}

	return OK;
}

int ThreadMutex::thread_mutex_lock(pthread_mutex_t* mtx)
{
	int  err;

	err = pthread_mutex_lock(mtx);
	if (err == 0) {
		return OK;
	}
	fprintf(stderr, "pthread_mutex_lock() failed, reason: %s\n", strerror(errno));

	return ERROR;
}

int ThreadMutex::thread_mutex_unlock(pthread_mutex_t* mtx)
{
	int  err;

	err = pthread_mutex_unlock(mtx);

#if 0
	ngx_time_update();
#endif

	if (err == 0) {
		return OK;
	}

	fprintf(stderr, "pthread_mutex_unlock() failed, reason: %s\n", strerror(errno));
	return ERROR;
}
