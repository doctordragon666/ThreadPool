#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include "thread_pool_def.h"
#include "ThreadCond.h"
#include "ThreadMutex.h"

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();
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
	static void thread_pool_exit_handler(void* data)
	{
		uint_t* lock = (uint_t*)data;

		*lock = 0;

		pthread_exit(0);//自杀任务
	}
	static void* thread_pool_cycle(void* data)
	{
		thread_pool_t* tp = (thread_pool_t*)data;

		thread_task_t* task;

		if (debug)fprintf(stderr, "thread in pool \"%s\" started\n", tp->name);

		for (;; ) {
			if (ThreadMutex::thread_mutex_lock(&tp->mtx) != OK) {
				return NULL;
			}//上锁

			tp->waiting--;//等待减少

			while (tp->queue.first == NULL) {
				if (ThreadCond::thread_cond_wait(&tp->cond, &tp->mtx)
					!= OK)
				{
					(void)ThreadMutex::thread_mutex_unlock(&tp->mtx);
					return NULL;
				}//没有任务尝试挂起线程
			}

			task = tp->queue.first;
			tp->queue.first = task->next;

			if (tp->queue.first == NULL) {
				tp->queue.last = &tp->queue.first;
			}

			if (ThreadMutex::thread_mutex_unlock(&tp->mtx) != OK) {
				return NULL;
			}

			if (debug) fprintf(stderr, "run task #%lu in thread pool \"%s\"\n",
				task->id, tp->name);

			task->handler(task->ctx);//执行函数

			if (debug) fprintf(stderr, "complete task #%lu in thread pool \"%s\"\n", task->id, tp->name);

			task->next = NULL;

			//释放task
			free(task);
		}
	}
	static int_t thread_pool_init_default(thread_pool_t* tpp, char* name)
	{
		if (tpp)
		{
			tpp->threads = DEFAULT_THREADS_NUM;
			tpp->max_queue = DEFAULT_QUEUE_NUM;

			tpp->name = strdup(name ? name : "default");
			if (debug)fprintf(stderr,
				"thread_pool_init, name: %s ,threads: %lu max_queue: %ld\n",
				tpp->name, tpp->threads, tpp->max_queue);

			return OK;
		}
		return ERROR;
	}
	static uint_t thread_pool_task_id;//会初始化为0
private:
	ThreadCond* m_tc;
	ThreadMutex* m_tm;
};

#endif // !_THREAD_POOL_H_