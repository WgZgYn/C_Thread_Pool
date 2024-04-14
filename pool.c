//
// Created by wzy on 24-4-5.
//

#include "pool.h"
#include <stdlib.h>
#include <assert.h>

// struct放入.c中实现私有封装，但是这样使用时只能创建指针型变量，通过传递指针的引用进行创建，
struct ThreadPool {
    thrd_t threads[MAX_THREADS];
    Task task_queue[MAX_TASKS]; // 任务队列
    int front, rear;            // 任务队列的前端和后端

    int thread_count;           // 当前线程数量
    _Atomic int task_count; // 当前执行中与待执行的任务数量，需要为原子类型防止数据竞争进而导致pool_wait永久阻塞

    mtx_t p_mutex; // 生产者锁
    mtx_t c_mutex; // 消费者锁

    sem_t empty_count;          // 信号量，用于表示队列剩余缓冲区大小
    sem_t task_count_sem; // 信号量，用于表示可执行的任务数量

    mtx_t mutex; // 互斥锁，用于保护任务队列
    cnd_t wait_cond;// 等待任务执行完成
};

// 初始化线程池
void pool_init(ThreadPool *pool) {
    assert(pool != NULL);
    pool->task_count = 0;
    pool->thread_count = 0;
    pool->front = pool->rear = 0;

#ifdef THREADPOOL_POSIX
    pthread_mutex_init(&pool->c_mutex, NULL);
    pthread_mutex_init(&pool->p_mutex, NULL);
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->wait_cond, NULL);
#else
    mtx_init(&pool->p_mutex, mtx_plain);
    mtx_init(&pool->c_mutex, mtx_plain);
    mtx_init(&pool->mutex, mtx_plain);
    cnd_init(&pool->wait_cond);
#endif
    sem_init(&pool->task_count_sem, 0, 0);
    sem_init(&pool->empty_count, 0, MAX_TASKS);
}

// TODO: wait时应阻塞任务添加
void add_task(ThreadPool *pool, Func *function, void *args) {
    pool->task_count++;// 原子类型
    /// 临界区
    sem_wait(&pool->empty_count);
    mtx_lock(&pool->p_mutex);
    pool->task_queue[pool->rear].function = function;
    pool->task_queue[pool->rear].args = args;
    pool->rear = (pool->rear + 1) % MAX_TASKS;
//    pool->task_count++;
    mtx_unlock(&pool->p_mutex);
    sem_post(&pool->task_count_sem);
}

// 添加任务到线程池，function不能为NULL，参数args需要有足够的生命周期，若为堆参数则需要调用者手动free内存
void pool_add_task(ThreadPool *pool, Func *function, void *args) {
    assert(function != NULL && pool != NULL);
    add_task(pool, function, args);
}

// 线程执行任务的函数
#ifdef THREADPOOL_POSIX
void *worker(void *arg) {
#else

int worker(void *arg) {
#endif
    ThreadPool *pool = (ThreadPool *) arg;
    Loop {
        sem_wait(&pool->task_count_sem); // 消费任务申请

        mtx_lock(&pool->c_mutex); // 消费者锁
        Task task = pool->task_queue[pool->front];
        pool->front = (pool->front + 1) % MAX_TASKS;
        mtx_unlock(&pool->c_mutex);

        sem_post(&pool->empty_count);
        if (task.function == NULL)
            return 0; // 收到结束通知， 仅会在destroy时触发
        task.function(task.args);

        pool->task_count--;
        cnd_signal(&pool->wait_cond);
    }
}

// 启动线程池
void pool_start(ThreadPool *pool, int thread_count) {
    assert(pool != NULL && thread_count + pool->thread_count <= MAX_THREADS);
    for (int i = 0; i < thread_count; i++) {
#ifdef THREADPOOL_POSIX
        pthread_create(&pool->threads[pool->thread_count], NULL, worker, pool);
#else
        thrd_create(&pool->threads[pool->thread_count], worker, pool);
#endif
        pool->thread_count++;
    }
}

// 销毁线程池
void pool_destroy(ThreadPool *pool) {
    assert(pool != NULL);
    for (int i = 0; i < pool->thread_count; i++) {
        add_task(pool, NULL, NULL); // 发送结束通知
    }

    for (int i = 0; i < pool->thread_count; i++) {
        thrd_join(pool->threads[i], NULL);
    }

    sem_destroy(&pool->task_count_sem);
    sem_destroy(&pool->empty_count);

    mtx_destroy(&pool->c_mutex);
    mtx_destroy(&pool->p_mutex);
    mtx_destroy(&pool->mutex);

    cnd_destroy(&pool->wait_cond);
}

void pool_wait(ThreadPool *pool) {
    assert(pool != NULL);
    mtx_lock(&pool->mutex);
    while (pool->task_count != 0) {
        cnd_wait(&pool->wait_cond, &pool->mutex);
    }
    mtx_unlock(&pool->mutex);
}

void pool_create(ThreadPool **pool) {
    *pool = malloc(sizeof(ThreadPool));
    pool_init(*pool);
}

void pool_free(ThreadPool **pool) {
    assert(pool != NULL && *pool != NULL);
    free(*pool);
    *pool = NULL;
}

int pool_tasks(ThreadPool *pool) {
    return pool->task_count;
}

int pool_threads(ThreadPool *pool) {
    return pool->thread_count;
}