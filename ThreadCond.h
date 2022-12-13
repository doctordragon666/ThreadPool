#ifndef _THREAD_COND_H_
#define _THREAD_COND_H_
#include "thread_pool_def.h"

/*********************
�����������߳�����������
�󲿷ֶ��Ǽ򵥷�װ
**********************/
class ThreadCond
{
public:
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
	static int thread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mtx);
private:
	
};
#endif