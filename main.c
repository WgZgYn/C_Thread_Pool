#include "pool.h"
#include "utils.hpp"
#include "sort.hpp"


#define LENGTH 100000
#define UPPER (LENGTH)

int main() {
    const int len = LENGTH;
    DEBUG_LOG_TIME("memory alloc start");

    int *nums = malloc(sizeof(int) * len);
    gen_random(nums, len, UPPER);
    DEBUG_LOG_TIME("memory alloc ok");

    ThreadPool *pool = NULL;
    pool_create(&pool);
    pool_init(pool);
    pool_start(pool, 4);
    DEBUG_LOG_TIME("sort start");

    sort_args sort_args[4];
    for (int i = 0; i < 4; i++) {
        sort_args[i].nums = nums;
        sort_args[i].left = len / 4 * i;
        if (i != 3)
            sort_args[i].right = len / 4 * (i + 1) - 1;
        else
            sort_args[i].right = len - 1;
        pool_add_task(pool, qsort_task, &sort_args[i]);
    }

    pool_wait(pool);
    DEBUG_LOG_TIME("batch sort finish");

    merge_args args[3] = {{nums, sort_args[0].left, sort_args[0].right, sort_args[1].right},
                          {nums, sort_args[2].left, sort_args[2].right, sort_args[3].right},
                          {nums, sort_args[0].left, sort_args[1].right, sort_args[3].right}};

    pool_add_task(pool, merge_task, &args[0]);
    pool_add_task(pool, merge_task, &args[1]);
    pool_wait(pool);
    pool_add_task(pool, merge_task, &args[2]);
    pool_wait(pool);

    DEBUG_LOG_TIME("merge finished");
    // print_nums(nums, len);
    int ok = is_sort(nums, 0, len - 1);

    if (ok == 1) {
        DEBUG_LOG("sorted!");
    } else {
        DEBUG_LOG("error on sort");
    }

    pool_destroy(pool);
    pool_free(&pool);
    free(nums);
    return 0;
}