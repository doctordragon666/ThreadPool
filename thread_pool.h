#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include "thread_pool_def.h"

/// <summary>
/// ��װ���������������ź���
/// </summary>
/// <param name="cond">��ʼ�����ź���</param>
/// <returns>�ɹ���ʧ��</returns>
int thread_cond_create(pthread_cond_t* cond);
/// <summary>
/// �����ź���
/// </summary>
/// <param name="cond">Ҫ���ٵ��ź���</param>
/// <returns>�ɹ���ʧ��</returns>
int thread_cond_destroy(pthread_cond_t* cond);
/// <summary>
/// �����ź����������߳�
/// </summary>
/// <param name="cond">�ź���</param>
/// <returns>�ɹ���ʧ��</returns>
int thread_cond_signal(pthread_cond_t* cond);
/// <summary>
/// �ȴ��ź���
/// </summary>
/// <param name="cond">�ź���</param>
/// <returns>�ɹ���ʧ��</returns>
int thread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mtx);

/// <summary>
/// ������
/// </summary>
/// <param name="mtx">��ʼ������</param>
/// <returns>�ɹ���ʧ��</returns>
int thread_mutex_create(pthread_mutex_t* mtx);
/// <summary>
/// ������
/// </summary>
/// <param name="mtx"></param>
/// <returns></returns>
int thread_mutex_destroy(pthread_mutex_t* mtx);
/// <summary>
/// ����
/// </summary>
/// <param name="mtx"></param>
/// <returns></returns>
int thread_mutex_lock(pthread_mutex_t* mtx);
/// <summary>
/// ����
/// </summary>
/// <param name="mtx"></param>
/// <returns></returns>
int thread_mutex_unlock(pthread_mutex_t* mtx);

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

#endif // !_THREAD_POOL_H_