// #include "sort.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

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
    print(3, 5, "abc\n");
    int* a = malloc(sizeof(int));
    call_add(a, 5, 7);
    printf("%d\n", *a);
    return 0;
}

