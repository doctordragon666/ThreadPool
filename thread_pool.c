#include "thread_pool.h"

static int debug = 0;

static void thread_pool_exit_handler(void* data)
{
    uint_t* lock = data;

    *lock = 0;

    pthread_exit(0);//��ɱ����
}
static void* thread_pool_cycle(void* data)
{
    thread_pool_t* tp = data;

    int                 err;
    thread_task_t* task;


    if (debug)fprintf(stderr, "thread in pool \"%s\" started\n", tp->name);



    for (;; ) {
        if (thread_mutex_lock(&tp->mtx) != OK) {
            return NULL;
        }//����


        tp->waiting--;//�ȴ�����

        while (tp->queue.first == NULL) {
            if (thread_cond_wait(&tp->cond, &tp->mtx)
                != OK)
            {
                (void)thread_mutex_unlock(&tp->mtx);
                return NULL;
            }//û�������Թ����߳�
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        if (tp->queue.first == NULL) {
            tp->queue.last = &tp->queue.first;
        }

        if (thread_mutex_unlock(&tp->mtx) != OK) {
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
static uint_t       thread_pool_task_id;//���ʼ��Ϊ0



thread_task_t* thread_task_alloc(size_t size)
{
    thread_task_t* task;

    task = calloc(1, sizeof(thread_task_t) + size);
    if (task == NULL) {
        return NULL;
    }

    task->ctx = task + 1;//�����һ��ʵ�Ǽ����������ṹ�壬�������»�ȡ�������struct

    return task;
}

int_t thread_task_post(thread_pool_t* tp, thread_task_t* task)
{
    if (thread_mutex_lock(&tp->mtx) != OK) {
        return ERROR;
    }//ȡ��

    if (tp->waiting >= tp->max_queue) {
        (void)thread_mutex_unlock(&tp->mtx);

        fprintf(stderr, "thread pool \"%s\" queue overflow: %ld tasks waiting\n",
            tp->name, tp->waiting);
        return ERROR;
    }//�����Ƿ��Ѿ�����

    task->id = thread_pool_task_id++;//����һ��id
    task->next = NULL;

    if (thread_cond_signal(&tp->cond) != OK) {
        (void)thread_mutex_unlock(&tp->mtx);
        return ERROR;
    }//�ѹ�����߳�ִ��

    *tp->queue.last = task;
    tp->queue.last = &task->next;//β��

    tp->waiting++;

    (void)thread_mutex_unlock(&tp->mtx);

    if (debug)fprintf(stderr, "task #%lu added to thread pool \"%s\"\n",
        task->id, tp->name);

    return OK;
}

thread_pool_t* thread_pool_init()
{
    int             err;
    
    thread_pool_t* tp = calloc(1, sizeof(thread_pool_t));
    if (tp == NULL) {
        fprintf(stderr, "thread_pool_init: calloc failed!\n");
    }

    thread_pool_init_default(tp, NULL);

    thread_pool_queue_init(&tp->queue);//��ʼ���������

    if (thread_mutex_create(&tp->mtx) != OK) {
        free(tp);
        return NULL;
    }

    if (thread_cond_create(&tp->cond) != OK) {
        (void)thread_mutex_destroy(&tp->mtx);
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
        err = pthread_create(&tid, &attr, thread_pool_cycle, tp);
        if (err) {
            fprintf(stderr, "pthread_create() failed, reason: %s\n", strerror(errno));
            free(tp);
            return NULL;
        }
    }

    (void)pthread_attr_destroy(&attr);//��������

    return tp;
}

void thread_pool_destroy(thread_pool_t* tp)
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

    (void)thread_cond_destroy(&tp->cond);
    (void)thread_mutex_destroy(&tp->mtx);

    free(tp);
}