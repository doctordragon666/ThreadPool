#include "thread_pool_def.h"
#include "ThreadMutex.h"
#include "ThreadPool.h"
#include "ThreadCond.h"

/// <summary>
/// 测试使用
/// </summary>

struct test {
	int arg1;
	int arg2;
};

void task_handler1(void* data) {
	static int index = 0;
	printf("Hello, this is 1th test.index=%d\r\n", index++);
}

void task_handler2(void* data) {
	static int index = 0;
	printf("Hello, this is 2th test.index=%d\r\n", index++);
}

void task_handler3(void* data) {
	static int index = 0;
	struct test* t = (struct test*)data;

	printf("Hello, this is 3th test.index=%d\r\n", index++);
	printf("arg1: %d, arg2: %d\n", t->arg1, t->arg2);
}

int
main(int argc, char** argv)
{
	ThreadPool* tp = new ThreadPool();
	thread_pool_t* init_tp = tp->thread_pool_init();
	//int i = 0;
	//sleep(1);
	thread_task_t* test1 = tp->thread_task_alloc(0);
	thread_task_t* test2 = tp->thread_task_alloc(0);
	thread_task_t* test3 = tp->thread_task_alloc(sizeof(struct test));//分配参数
	test1->handler = task_handler1;
	test2->handler = task_handler2;
	test3->handler = task_handler3;
	((struct test*)test3->ctx)->arg1 = 666;
	((struct test*)test3->ctx)->arg2 = 888;
	//for(i=0; i<10;i++){
	tp->thread_task_post(init_tp, test1);
	tp->thread_task_post(init_tp, test2);
	tp->thread_task_post(init_tp, test3);
	//}
	sleep(10);
	tp->thread_pool_destroy(init_tp);
}