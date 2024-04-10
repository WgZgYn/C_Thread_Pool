// #include "sort.hpp"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <threads.h>
#include "utils.hpp"

void print(int a, ...) {
    va_list arg_list;
    va_start(arg_list, a);
    int val2 = va_arg(arg_list, int);
    char* val3 = va_arg(arg_list, char*);
    printf("%d %d %s", a, val2, val3);
    va_end(arg_list);
}

int add(int a, int b) {
    return a + b;
}

void call_add(int* res, ...) {
    va_list a;
    va_start(a, res);
    int v1 = va_arg(a, int);
    int v2 = va_arg(a, int);
    va_end(a);
    *res = add(v1, v2);
}

int main() {
    int a = 5, b = 7;
    double c = 6.6, d = 7.8;
    
    SWAP(&a, &b);
    SWAP(&c, &d);

    printf("a: %d, b: %d, c: %lf, d: %lf\n", a, b, c, d);
    return 0;
}

