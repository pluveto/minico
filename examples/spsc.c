#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "co.h"

#define BUFFER_SIZE 10

typedef struct
{
    int buffer[BUFFER_SIZE];
    int count;
} Buffer;

Buffer buffer = {.count = 0};

int producer_func(Co *co, void *data)
{
    int *produce_count = (int *)data;

    CO_BEGIN(co);

    while (*produce_count > 0)
    {
        if (buffer.count < BUFFER_SIZE)
        {
            buffer.buffer[buffer.count++] = *produce_count;
            printf("Produced: %d\n", *produce_count);
            (*produce_count)--;
        }
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
            int item = buffer.buffer[--buffer.count];
            printf("Consumed: %d\n", item);
            (*consume_count)--;
        }
        CO_YIELD(co, 0);
    }

    CO_END(co);
    return 0;
}

int main()
{
    int produce_count = 15;
    int consume_count = 15;
    Co producer, consumer;

    co_init(&producer, producer_func, &produce_count);
    co_init(&consumer, consumer_func, &consume_count);

    while (producer.state != CO_FIN || consumer.state != CO_FIN)
    {
        if (producer.state != CO_FIN)
        {
            co_next(&producer);
        }
        if (consumer.state != CO_FIN)
        {
            co_next(&consumer);
        }
        usleep(100000); // Sleep for 100ms
    }

    return 0;
}
