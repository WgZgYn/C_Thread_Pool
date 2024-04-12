//
// Created by wzy on 24-4-8.
//
// This file is just part of sort.hpp
#include "utils.hpp"
#include <stdlib.h>
#include <string.h>

int position(int *nums, int left, int right) {
    while (left < right) {
        while (left < right && nums[left] <= nums[right]) {
            right--;
        }
        if (left < right)
            swap(nums + right, nums + left);
        while (left < right && nums[left] <= nums[right]) {
            left++;
        }
        if (left < right)
            swap(nums + right, nums + left);
    }
    return left;
}

void quick_sort(int *nums, int left, int right) {
    if (left >= right)
        return;
    int p = position(nums, left, right);
    quick_sort(nums, left, p - 1);
    quick_sort(nums, p + 1, right);
}

#ifdef THREADPOOL_POOL_H

void quick_sort_task(void *arg) {
    sort_args *a = arg;
    DEBUG_LOG("qsort [%d, %d]\n", a->left, a->right);
    quick_sort(a->nums, a->left, a->right);
}

// TODO: fix odd threads
void pool_quick_sort(ThreadPool *pool, int *nums, int length) {
    int threads = pool_threads(pool);
    int batch = length / threads;
    DEBUG_LOG_TIME("POOL ADD TASK");
    sort_args args[threads];
    for (int i = 0; i < threads; i++) {
        args[i].nums = nums;
        args[i].left = batch * i;
        if (i != threads - 1)
            args[i].right = batch * (i + 1) - 1;
        else
            args[i].right = length - 1;
        pool_add_task(pool, qsort_task, &args[i]);
    }
    DEBUG_LOG_TIME("MALLOC MERGE BUFFER");
    int *buff = malloc(sizeof(int) * length);
    DEBUG_LOG_TIME("POOL WAIT");
    pool_wait(pool);
    DEBUG_LOG_TIME("BATCH SORT FINISH");
    merge_buf_args margs[threads - 1];
    int size = 2;
    int index = 0;
    while (size <= threads) {
        for (int i = 0; i < threads; i += size) {
            margs[index].buff = buff;
            margs[index].nums = nums;
            margs[index].left = args[i].left;
            margs[index].mid = args[i + size / 2 - 1].right;
            if (i + size - 1 < threads)
                margs[index].right = args[i + size - 1].right;
            else
                margs[index].right = args[threads - 1].right;
            pool_add_task(pool, merge_buf_task, &margs[index]);
            index += 1;
        }
        size *= 2;
        pool_wait(pool);
        DEBUG_LOG_TIME("WAIT");
    }
    DEBUG_LOG_TIME("Free MERGE BUFFER");
    free(buff);
}

#endif

// merge [left, right], [left, mid], [mid+1, right]
void merge(int *arr, int left, int mid, int right) {
    if(mid >= right || mid < left) return;
    int left_size = mid - left + 1;
    int right_size = right - mid;

    int *left_arr = (int *)malloc(sizeof(int) * left_size);
    int *right_arr = (int *)malloc(sizeof(int) * right_size);

    // 将原数组划分为左右两部分
    memcpy(left_arr, arr + left, sizeof(int) * left_size);
    memcpy(right_arr, arr + mid + 1, sizeof(int) * right_size);

    // 归并操作
    int i = 0, j = 0, k = left;
    while (i < left_size && j < right_size) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        } else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    // 处理剩余元素
    while (i < left_size) {
        arr[k++] = left_arr[i++];
    }
    while (j < right_size) {
        arr[k++] = right_arr[j++];
    }

    // 释放临时数组内存
    free(left_arr);
    free(right_arr);
}

// make sure the buf have the same size with arr
void merge_buf(int *arr, int *buf, int left, int mid, int right) {
    int left_size = mid - left + 1;
    int right_size = right - mid;

    memcpy(buf + left, arr + left, sizeof(int) * (right - left + 1));

    int *left_arr = buf + left;
    int *right_arr = buf + mid + 1;
    // 归并操作
    int i = 0, j = 0, k = left;
    while (i < left_size && j < right_size) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        } else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    // 处理剩余元素
    while (i < left_size) {
        arr[k++] = left_arr[i++];
    }
    while (j < right_size) {
        arr[k++] = right_arr[j++];
    }
}

int cmp(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

void sort(int *nums, int left, int right) {
    qsort(nums + left, right - left + 1, sizeof(int), cmp);
}

int is_sort(const int *nums, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        if (nums[i] < nums[i - 1]) {
            return 0;
        }
    }
    return 1;
}