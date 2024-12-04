#ifndef _THREAD_POOL_DEF_H_
#define _THREAD_POOL_DEF_H_

//调试开关
static int debug = 1;

//系统头文件包含
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>	//类型包含
#include <pthread.h>	//线程函数
#include <errno.h>
#include <string.h>
#include <unistd.h>

//跨平台类型定义
typedef intptr_t        int_t;
typedef uintptr_t       uint_t;
typedef unsigned long         atomic_uint_t;

//全局线程变量定义
#define  OK          0
#define  ERROR      -1
#define DEFAULT_THREADS_NUM 4     //默认线程数
#define DEFAULT_QUEUE_NUM  65535  //最大任务队列长度
#define thread_pool_queue_init(q)                                         \
    (q)->first = NULL;                                                    \
    (q)->last = &(q)->first

//线程任务
typedef struct thread_task_s {
	struct thread_task_s* next;	//下一个任务
	uint_t  id;					//具体任务的id
	void* ctx;					//参数，上下文
	void (*handler)(void* data);//处理函数
}thread_task_t;

//线程任务队列
typedef struct {
	thread_task_t* first;//头指针
	thread_task_t** last;//尾指针
} thread_pool_queue_t;//单链表

//线程池
struct thread_pool_t {
	pthread_mutex_t        mtx;		//锁
	thread_pool_queue_t   queue;	//线程池队列
	int_t                 waiting;	//等待的任务数量
	pthread_cond_t         cond;	//条件变量

	char* name;						//线程池的名字
	uint_t                threads;	//线程的数量
	int_t                 max_queue;//最大队列长度
};

#endif // !_THREAD_POOL_DEF_H_