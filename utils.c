//
// Created by wzy on 24-4-9.
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void swap(int *a, int *b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}

#define SWAP_TYPE(a, b, type) \
do {                          \
    type temp = *b;           \
    *b = *a;                  \
    *a = temp;                \
} while(0)                    \

// maybe useful in GCC compiler
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress"
#pragma GCC diagnostic ignored "-Wunused-value"
#define SWAP(a, b)          \
do {                        \
    a && b && *a && *b;     \
    typeof(*a) temp = *b;   \
    *b = *a;                \
    *a = temp;              \
} while(0)                  
#pragma GCC diagnostic pop

void gen_random(int nums[], int length, int upper) {
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        nums[i] = rand() % upper;
    }
}

void print_nums(int nums[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%8d ", nums[i]);
        if (i % 32 == 31) {
            printf("\n");
        }
    }
    printf("\n");
}