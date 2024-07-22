#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "co.h"

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 3

typedef struct
{
    int buffer[BUFFER_SIZE];
    int count;
} Buffer;

Buffer buffer = {.count = 0};

int producer_func(Co *co, void *data)
{
    int *produce_count = (int *)data;
    srand(time(NULL) ^ (getpid() << 16));

    CO_BEGIN(co);

    while (*produce_count > 0)
    {
        if (buffer.count < BUFFER_SIZE)
        {
            if (buffer.count < BUFFER_SIZE)
            {
                buffer.buffer[buffer.count++] = *produce_count;
                printf("Produced: %d\n", *produce_count);
                (*produce_count)--;
            }
        }
        usleep(rand() % 200000); // Sleep for a random time between 0 and 200ms
        CO_YIELD(co, 0);
    }

    CO_END(co);
    return 0;
}

int consumer_func(Co *co, void *data)
{
    int *consume_count = (int *)data;

    CO_BEGIN(co);

    while (*consume_count > 0)
    {
        if (buffer.count > 0)
        {
            if (buffer.count > 0)
            {
                int item = buffer.buffer[--buffer.count];
                printf("Consumed: %d\n", item);
                (*consume_count)--;
            }
        }
        usleep(100000); // Sleep for 100ms
        CO_YIELD(co, 0);
    }

    CO_END(co);
    return 0;
}

int main()
{
    int produce_count[NUM_PRODUCERS] = {5, 5, 5}; // Each producer will produce 5 items
    int consume_count = 15;                       // Total items to consume
    Co producer[NUM_PRODUCERS], consumer;

    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        co_init(&producer[i], producer_func, &produce_count[i]);
    }
    co_init(&consumer, consumer_func, &consume_count);

    while (1)
    {
        int all_producers_finished = 1;
        for (int i = 0; i < NUM_PRODUCERS; i++)
        {
            if (producer[i].state != CO_FIN)
            {
                co_next(&producer[i]);
                all_producers_finished = 0;
            }
        }
        if (consumer.state != CO_FIN)
        {
            co_next(&consumer);
        }

        if (all_producers_finished && consumer.state == CO_FIN)
        {
            break;
        }
        usleep(100000); // Sleep for 100ms
    }

    return 0;
}
