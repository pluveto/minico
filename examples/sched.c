#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "co.h"

int rand_range(int min, int max)
{
    return min + rand() % (max - min + 1);
}

typedef struct
{
    int id;
    int count;
    const char *name;
} TaskData;

int task_func(Co *co, void *data)
{
    TaskData *task = (TaskData *)data;

    CO_BEGIN(co);

    while (task->count > 0)
    {
        printf("Task %d: %d\n", task->id, task->count);
        task->count--;
        usleep(rand_range(100000, 200000));
        CO_YIELD(co, 0);
    }

    printf("Task %s completed\n", task->name);

    CO_END(co);
    return 0;
}

int main()
{
    TaskData task1 = {.id = 1, .count = 5, .name = "Task1"};
    TaskData task2 = {.id = 2, .count = 3, .name = "Task2"};
    TaskData task3 = {.id = 3, .count = 4, .name = "Task3"};
    Co co1, co2, co3;

    co_init(&co1, task_func, &task1);
    co_init(&co2, task_func, &task2);
    co_init(&co3, task_func, &task3);

    while (co1.state != _CO_STATE_FIN || co2.state != _CO_STATE_FIN || co3.state != _CO_STATE_FIN)
    {
        if (co1.state != _CO_STATE_FIN)
        {
            co_next(&co1);
        }
        if (co2.state != _CO_STATE_FIN)
        {
            co_next(&co2);
        }
        if (co3.state != _CO_STATE_FIN)
        {
            co_next(&co3);
        }
    }

    return 0;
}
