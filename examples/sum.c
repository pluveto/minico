#include <stdio.h>
#include "co.h"

void foo()
{
    int arr[100] = {0};
    int i = 0;
    for (i = 0; i < 100; ++i)
    {
        arr[i] = i;
    }
}

int task(Co *co, void *data)
{
    CO_BEGIN(co);

    int i = 0;
    int sum = 0;
    printf("Task start: i = %d, sum = %d\n", i, sum);

    for (i = 1; i <= 3; ++i)
    {
        sum += i;
        printf("Task yield: i = %d, sum = %d\n", i, sum);
        foo();
        CO_YIELD(co, sum);
        foo();
    }

    printf("Task end: i = %d, sum = %d\n", i, sum);

    CO_END(co);
    return 0;
}

int main()
{
    Co co;
    co_init(&co, task, NULL);

    while (!co_done(&co))
    {
        int result = co_next(&co);
        printf("Main: result = %d\n", result);
        foo();
    }

    return 0;
}
