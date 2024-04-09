//
// Created by wzy on 24-4-9.
//

// 确保至多包含一次
#ifndef THREADPOOL_SORT_HPP
#define THREADPOOL_SORT_HPP

// [left, right]
typedef struct {
    int *nums;
    int left, mid, right;
} merge_args;

// [left, right]
typedef struct {
    int *nums;
    int left, right;
} sort_args;

// [left, right]
int position(int *nums, int left, int right);

// [left, right]
void quick_sort(int *nums, int left, int right);

// [left, right]
void merge(int *arr, int left, int mid, int right);

int cmp(const void *a, const void *b);

// [left, right]
void sort(int *nums, int left, int right);

// [left, right]
int is_sort(const int *nums, int left, int right);

// 实现扩展函数功能，不影响正常使用
#ifdef THREADPOOL_POOL_H
struct quick_sort_args {
    ThreadPool *pool;
    int *nums, left, right;
};

void quick_sort_thread(ThreadPool *pool, int *nums, int left, int right);

void quick_sort_wrap_thread(void *arg);

void quick_sort_task(void *arg);

void qsort_task(void *arg) {
    sort_args *a = arg;
    sort(a->nums, a->left, a->right);
}

void merge_task(void *arg) {
    merge_args *a = arg;
    DEBUG_LOG("merge [%d %d]", a->left, a->right);
    merge(a->nums, a->left, a->mid, a->right);
}
#endif

// 不再需要添加sort.c编译项，避免重定义
#include "sort.c"

#endif //THREADPOOL_SORT_HPP
