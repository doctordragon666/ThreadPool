#ifndef _THREAD_MUTEX_H_
#define _THREAD_MUTEX_H_

#include "thread_pool_def.h"

class ThreadMutex
{
public:
	/// <summary>
	/// 创建锁
	/// </summary>
	/// <param name="mtx">初始化的锁</param>
	/// <returns>成功或失败</returns>
	static int thread_mutex_create(pthread_mutex_t* mtx);

	/// <summary>
	/// 销毁锁
	/// </summary>
	/// <param name="mtx"></param>
	/// <returns></returns>
	static int thread_mutex_destroy(pthread_mutex_t* mtx);

	/// <summary>
	/// 上锁
	/// </summary>
	/// <param name="mtx"></param>
	/// <returns></returns>
	static int thread_mutex_lock(pthread_mutex_t* mtx);

	/// <summary>
	/// 解锁
	/// </summary>
	/// <param name="mtx"></param>
	/// <returns></returns>
	static int thread_mutex_unlock(pthread_mutex_t* mtx);
};

#endif // !_THREAD_MUTEX_H_