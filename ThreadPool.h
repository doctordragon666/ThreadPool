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
	/// ������亯��
	/// </summary>
	/// <param name="size">�����Ľṹ���С</param>
	/// <returns>thread_task_t*</returns>
	thread_task_t* thread_task_alloc(size_t size);

	/// <summary>
	/// �߳�Ͷ�ݺ���
	/// </summary>
	/// <param name="tp">�̳߳�</param>
	/// <param name="task"></param>
	/// <returns>�ɹ���ʧ��</returns>
	int_t thread_task_post(thread_pool_t* tp, thread_task_t* task);

	/// <summary>
	/// �̳߳س�ʼ��
	/// </summary>
	/// <returns>�̳߳�</returns>
	thread_pool_t* thread_pool_init();

	/// <summary>
	/// �̳߳�����
	/// </summary>
	/// <param name="tp">���ٵ��̳߳�</param>
	void thread_pool_destroy(thread_pool_t* tp);
	static void thread_pool_exit_handler(void* data)
	{
		uint_t* lock = (uint_t*)data;

		*lock = 0;

		pthread_exit(0);//��ɱ����
	}
	static void* thread_pool_cycle(void* data)
	{
		thread_pool_t* tp = (thread_pool_t*)data;

		int                 err;
		thread_task_t* task;

		if (debug)fprintf(stderr, "thread in pool \"%s\" started\n", tp->name);

		for (;; ) {
			if (ThreadMutex::thread_mutex_lock(&tp->mtx) != OK) {
				return NULL;
			}//����

			tp->waiting--;//�ȴ�����

			while (tp->queue.first == NULL) {
				if (ThreadCond::thread_cond_wait(&tp->cond, &tp->mtx)
					!= OK)
				{
					(void)ThreadMutex::thread_mutex_unlock(&tp->mtx);
					return NULL;
				}//û�������Թ����߳�
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

			task->handler(task->ctx);//ִ�к���

			if (debug) fprintf(stderr, "complete task #%lu in thread pool \"%s\"\n", task->id, tp->name);

			task->next = NULL;

			//�ͷ�task
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
	static uint_t thread_pool_task_id;//���ʼ��Ϊ0
private:
	ThreadCond* m_tc;
	ThreadMutex* m_tm;
};

#endif // !_THREAD_POOL_H_