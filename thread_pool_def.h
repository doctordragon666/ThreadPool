#ifndef _THREAD_POOL_DEF_H_
#define _THREAD_POOL_DEF_H_

//���Կ���
static int debug = 1;

//ϵͳͷ�ļ�����
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>	//���Ͱ���
#include <pthread.h>	//�̺߳���
#include <errno.h>
#include <string.h>
#include <unistd.h>

//��ƽ̨���Ͷ���
typedef intptr_t        int_t;
typedef uintptr_t       uint_t;
typedef unsigned long         atomic_uint_t;

//ȫ���̱߳�������
#define  OK          0
#define  ERROR      -1
#define DEFAULT_THREADS_NUM 4     //Ĭ���߳���
#define DEFAULT_QUEUE_NUM  65535  //���������г���
#define thread_pool_queue_init(q)                                         \
    (q)->first = NULL;                                                    \
    (q)->last = &(q)->first

//�߳�����
typedef struct thread_task_s {
	struct thread_task_s* next;	//��һ������
	uint_t  id;					//���������id
	void* ctx;					//������������
	void (*handler)(void* data);//������
}thread_task_t;

//�߳��������
typedef struct {
	thread_task_t* first;//ͷָ��
	thread_task_t** last;//βָ��
} thread_pool_queue_t;//������

//�̳߳�
struct thread_pool_t {
	pthread_mutex_t        mtx;		//��
	thread_pool_queue_t   queue;	//�̳߳ض���
	int_t                 waiting;	//�ȴ�����������
	pthread_cond_t         cond;	//��������

	char* name;						//�̳߳ص�����
	uint_t                threads;	//�̵߳�����
	int_t                 max_queue;//�����г���
};

#endif // !_THREAD_POOL_DEF_H_