#include "pool.h"
#include "sort.hpp"
#include "utils.hpp"
#include <unistd.h>

#define LENGTH 1000000000
#define UPPER (LENGTH)

int main() {
    const int len = LENGTH;
    DEBUG_LOG_TIME("ARRAY MALLOC");

    int *nums = malloc(sizeof(int) * len);
    gen_random(nums, len, UPPER);
    DEBUG_LOG_TIME("ARRAY MALLOC FINISH");
//    print_nums(nums, len);

    ThreadPool *pool = NULL;
    pool_create(&pool);
    pool_init(pool);
    pool_start(pool, 4);
    DEBUG_LOG_TIME("SORT START");

    pool_quick_sort(pool, nums, len);
    sleep(1);
    print_nums(nums, len);
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