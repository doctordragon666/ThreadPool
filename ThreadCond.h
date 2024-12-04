/*************************************************
Date: 2024年12月1日19点27分
Description:
定义了所有线程条件变量
*************************************************/
#ifndef _THREAD_COND_H_
#define _THREAD_COND_H_
#include "thread_pool_def.h"

class ThreadCond
{
public:
	/// <summary>
	/// 封装条件变量，返回信号量
	/// </summary>
	/// <param name="cond">初始化的信号量</param>
/// <returns>成功或失败</returns>
	int thread_cond_create(pthread_cond_t* cond);

	/// <summary>
	/// 销毁信号量
	/// </summary>
	/// <param name="cond">要销毁的信号量</param>
	/// <returns>成功或失败</returns>
	int thread_cond_destroy(pthread_cond_t* cond);

	/// <summary>
	/// 发送信号量，唤醒线程
	/// </summary>
	/// <param name="cond">信号量</param>
	/// <returns>成功或失败</returns>
	int thread_cond_signal(pthread_cond_t* cond);

	/// <summary>
	/// 等待信号量
	/// </summary>
	/// <param name="cond">信号量</param>
	/// <returns>成功或失败</returns>
	static int thread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mtx);
};
#endif