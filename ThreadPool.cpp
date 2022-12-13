#include "ThreadPool.h"

uint_t ThreadPool::thread_pool_task_id = 0;

ThreadPool::ThreadPool()
{
	m_tc = new ThreadCond;
	m_tm = new ThreadMutex;
}

ThreadPool::~ThreadPool()
{
	if (m_tm)
	{
		delete m_tm;
		m_tm = nullptr;
	}
	if (m_tc)
	{
		delete m_tc;
		m_tc = nullptr;
	}
}

thread_task_t* ThreadPool::thread_task_alloc(size_t size)
{
	thread_task_t* task;

	task = (thread_task_t*)calloc(1, sizeof(thread_task_t) + size);
	if (task == NULL) {
		return NULL;
	}

	task->ctx = task + 1;//�����һ��ʵ�Ǽ����������ṹ�壬�������»�ȡ�������struct

	return task;
}

int_t ThreadPool::thread_task_post(thread_pool_t* tp, thread_task_t* task)
{
	if (m_tm->thread_mutex_lock(&tp->mtx) != OK) {
		return ERROR;
	}//ȡ��

	if (tp->waiting >= tp->max_queue) {
		(void)m_tm->thread_mutex_unlock(&tp->mtx);

		fprintf(stderr, "thread pool \"%s\" queue overflow: %ld tasks waiting\n",
			tp->name, tp->waiting);
		return ERROR;
	}//�����Ƿ��Ѿ�����

	task->id = thread_pool_task_id++;//����һ��id
	task->next = NULL;

	if (m_tc->thread_cond_signal(&tp->cond) != OK) {
		(void)m_tm->thread_mutex_unlock(&tp->mtx);
		return ERROR;
	}//�ѹ�����߳�ִ��

	*tp->queue.last = task;
	tp->queue.last = &task->next;//β��

	tp->waiting++;

	(void)m_tm->thread_mutex_unlock(&tp->mtx);

	if (debug)fprintf(stderr, "task #%lu added to thread pool \"%s\"\n",
		task->id, tp->name);
	return OK;
}

thread_pool_t* ThreadPool::thread_pool_init()
{
	int             err;

	thread_pool_t* tp = (thread_pool_t*)calloc(1, sizeof(thread_pool_t));
	if (tp == NULL) {
		fprintf(stderr, "thread_pool_init: calloc failed!\n");
	}

	ThreadPool::thread_pool_init_default(tp, NULL);

	thread_pool_queue_init(&tp->queue);//��ʼ���������

	if (m_tm->thread_mutex_create(&tp->mtx) != OK) {
		free(tp);
		return NULL;
	}

	if (m_tc->thread_cond_create(&tp->cond) != OK) {
		(void)m_tm->thread_mutex_destroy(&tp->mtx);
		free(tp);
		return NULL;
	}

	pthread_attr_t  attr;
	err = pthread_attr_init(&attr);
	if (err) {
		fprintf(stderr, "pthread_attr_init() failed, reason: %s\n", strerror(errno));
		free(tp);
		return NULL;
	}

	//PTHREAD_CREATE_DETACHED�����̴߳���ʱ���÷���״̬�����߳��޷��ȴ������������߳�
	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (err) {
		fprintf(stderr, "pthread_attr_setdetachstate() failed, reason: %s\n", strerror(errno));
		free(tp);
		return NULL;
	}

	//��ѭ��
	pthread_t  tid;
	for (uint_t n = 0; n < tp->threads; n++) {
		err = pthread_create(&tid, &attr, ThreadPool::thread_pool_cycle, tp);
		if (err) {
			fprintf(stderr, "pthread_create() failed, reason: %s\n", strerror(errno));
			free(tp);
			return NULL;
		}
	}

	(void)pthread_attr_destroy(&attr);//��������

	return tp;
}

void ThreadPool::thread_pool_destroy(thread_pool_t* tp)
{
	volatile uint_t  lock;
	thread_task_t    task;

	memset(&task, '\0', sizeof(thread_task_t));

	task.handler = thread_pool_exit_handler;
	task.ctx = (void*)&lock;

	for (uint_t n = 0; n < tp->threads; n++) {
		lock = 1;

		if (thread_task_post(tp, &task) != OK) {
			return;
		}

		while (lock) {
			sched_yield();//�ö�cpu��ִ��Ȩ
		}

		//task.event.active = 0;
	}

	(void)m_tc->thread_cond_destroy(&tp->cond);
	(void)m_tm->thread_mutex_destroy(&tp->mtx);

	free(tp);
}