//
// Created by wzy on 24-4-9.
//

// 确保至多包含一次
#ifndef THREADPOOL_SORT_HPP
#define THREADPOOL_SORT_HPP
#include "utils.hpp"

// [left, right]
typedef struct {
    int *nums;
    int left, mid, right;
} merge_args;

typedef struct {
    int *nums, *buff;
    int left, mid, right;
} merge_buf_args;

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

void quick_sort_thread(ThreadPool *pool, int *nums, int length);
void merge_buf(int *arr, int *buf, int left, int mid, int right);
void quick_sort_task(void *arg);

void qsort_task(void *arg) {
    sort_args *a = arg;
    DEBUG_LOG_TIME("SORT: [%d, %d]", a->left, a->right);
    sort(a->nums, a->left, a->right);
}

void merge_task(void *arg) {
    merge_args *a = arg;
    DEBUG_LOG("merge [%d, %d]", a->left, a->right);
    merge(a->nums, a->left, a->mid, a->right);
}

void merge_buf_task(void *arg) {
    merge_buf_args *a = arg;
    DEBUG_LOG("merge [%d, %d] and [%d, %d]", a->left, a->mid, a->mid+1, a->right);
    merge_buf(a->nums, a->buff, a->left, a->mid, a->right);
}
#endif

// 不再需要添加sort.c编译项，避免重定义
#include "sort.c"

#endif //THREADPOOL_SORT_HPP
