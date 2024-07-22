#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "co.h"

typedef struct
{
    int count;
    int interval;
} TimerData;

int timer_func(Co *co, void *data)
{
    TimerData *timer = (TimerData *)data;

    CO_BEGIN(co);

    while (timer->count > 0)
    {
        printf("Timer: %d\n", timer->count);
        timer->count--;
        sleep(timer->interval);
        CO_YIELD(co, 0);
    }

    CO_END(co);
    return 0;
}

int main()
{
    TimerData timer = {.count = 5, .interval = 1};
    Co co;

    co_init(&co, timer_func, &timer);

    while (!co_done(&co))
    {
        co_next(&co);
    }

    return 0;
}
