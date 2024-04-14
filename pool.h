//
// Created by wzy on 24-4-5.
//

#ifndef THREADPOOL_POOL_H
#define THREADPOOL_POOL_H

#include <semaphore.h>

// #define THREADPOOL_POSIX
#define DEBUG // 用于DEBUG_LOG调试

#define MAX_THREADS 16
#define MAX_TASKS 20


#ifdef THREADPOOL_POSIX
#include <pthread.h>
#else
#include <threads.h>
#endif


// 这里是非常懒而且危险的做法
#ifdef THREADPOOL_POSIX
#define mtx_t pthread_mutex_t
#define thrd_t pthread_t
#define cnd_t pthread_cond_t

#define mtx_destroy pthread_mutex_destroy
#define cnd_destroy pthread_cond_destroy

#define mtx_lock pthread_mutex_lock
#define mtx_unlock pthread_mutex_unlock

#define cnd_wait pthread_cond_wait
#define cnd_signal pthread_cond_signal

#define thrd_join pthread_join
#endif

// 函数参数传入结构体可以对多参数有返回值的函数进行包装，
// 参数需要有足够的生命周期，调用者自行决定是否需要free,
typedef void(Func)(void *);

// 任务结构体
typedef struct {
    Func *function; // 函数指针，用于执行任务
    void *args;     // 函数参数
} Task;

// 私有封装防止外部调用内部数据
typedef struct ThreadPool ThreadPool;

#ifdef THREADPOOL_POSIX
void *worker(void *arg);
#else
int worker(void *arg);

#endif
// TODO: 返回整数以判断是否出错
void pool_create(ThreadPool **pool);

void pool_free(ThreadPool **pool);

void pool_init(ThreadPool *pool);
// 将start合并至init
void pool_start(ThreadPool *pool, int thread_count);

void pool_add_task(ThreadPool *pool, Func *function, void *args);

void pool_wait(ThreadPool *pool);

void pool_destroy(ThreadPool *pool);

int pool_tasks(ThreadPool *pool);

int pool_threads(ThreadPool *pool);

#define Lock(mutex, block)  \
do {                        \
    mtx_lock(mutex);        \
    block;                  \
    mtx_unlock(mutex);      \
} while(0)                  \

#define Loop for(;;)
#define Loopn(n) for(int i = 0; i < n; i++)



#endif // THREADPOOL_POOL_H
