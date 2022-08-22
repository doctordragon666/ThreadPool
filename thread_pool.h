#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include "thread_pool_def.h"

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
int thread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mtx);

/// <summary>
/// 创建锁
/// </summary>
/// <param name="mtx">初始化的锁</param>
/// <returns>成功或失败</returns>
int thread_mutex_create(pthread_mutex_t* mtx);
/// <summary>
/// 销毁锁
/// </summary>
/// <param name="mtx"></param>
/// <returns></returns>
int thread_mutex_destroy(pthread_mutex_t* mtx);
/// <summary>
/// 上锁
/// </summary>
/// <param name="mtx"></param>
/// <returns></returns>
int thread_mutex_lock(pthread_mutex_t* mtx);
/// <summary>
/// 解锁
/// </summary>
/// <param name="mtx"></param>
/// <returns></returns>
int thread_mutex_unlock(pthread_mutex_t* mtx);

/// <summary>
/// 任务分配函数
/// </summary>
/// <param name="size">参数的结构体大小</param>
/// <returns>thread_task_t*</returns>
thread_task_t* thread_task_alloc(size_t size);
/// <summary>
/// 线程投递函数
/// </summary>
/// <param name="tp">线程池</param>
/// <param name="task"></param>
/// <returns>成功或失败</returns>
int_t thread_task_post(thread_pool_t* tp, thread_task_t* task);
/// <summary>
/// 线程池初始化
/// </summary>
/// <returns>线程池</returns>
thread_pool_t* thread_pool_init();
/// <summary>
/// 线程池销毁
/// </summary>
/// <param name="tp">销毁的线程池</param>
void thread_pool_destroy(thread_pool_t* tp);

#endif // !_THREAD_POOL_H_