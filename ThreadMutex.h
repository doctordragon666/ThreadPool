#ifndef _THREAD_MUTEX_H_
#define _THREAD_MUTEX_H_

#include "thread_pool_def.h"

class ThreadMutex
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="mtx">��ʼ������</param>
	/// <returns>�ɹ���ʧ��</returns>
	static int thread_mutex_create(pthread_mutex_t* mtx);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="mtx"></param>
	/// <returns></returns>
	static int thread_mutex_destroy(pthread_mutex_t* mtx);

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="mtx"></param>
	/// <returns></returns>
	static int thread_mutex_lock(pthread_mutex_t* mtx);

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="mtx"></param>
	/// <returns></returns>
	static int thread_mutex_unlock(pthread_mutex_t* mtx);
};

#endif // !_THREAD_MUTEX_H_