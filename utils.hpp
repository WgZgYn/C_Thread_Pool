//
// Created by wzy on 24-4-9.
//

#ifndef THREADPOOL_UTILS_HPP
#define THREADPOOL_UTILS_HPP

void gen_random(int nums[], int length, int upper);

void print_nums(int nums[], int length);

void swap(int *a, int *b);


// 定义DEBUG_LOG宏，用于打印DEBUG LOG
#ifdef DEBUG
#define TIME_FORMAT_STR "%Y-%m-%d %H:%M:%S"
#include <string.h>
#include <stdio.h>
#include <time.h>
// 提取文件名的文件名部分
#define SHORT_FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DEBUG_LOG_TIME(format, ...) do {    \
        time_t t = time(NULL);              \
        struct tm *tm_info = localtime(&t); \
        char time_str[20];                  \
        strftime(time_str, sizeof(time_str), TIME_FORMAT_STR, tm_info); \
        printf("[File: %s, Line: %d, Time: %s] " format "\n", SHORT_FILE, __LINE__, time_str, ##__VA_ARGS__); \
} while(0)

#define DEBUG_LOG(format, ...) printf("[File: %s, Line: %d] " format "\n", SHORT_FILE, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_LOG(format, ...) // 在Release模式下，DEBUG_LOG将会被忽略
#define DEBUG_LOG_TIME(format, ...) 
#endif
#include "utils.c"
#endif //THREADPOOL_UTILS_HPP
