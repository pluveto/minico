#include <stdio.h>
#include <stdlib.h>

#include "co.h"

typedef struct
{
    int n;
    int a, b;
} FibData;

int fib_func(Co *co, void *data)
{
    FibData *fib = (FibData *)data;

    CO_BEGIN(co);

    fib->a = 0;
    fib->b = 1;
    for (; fib->n > 0; --fib->n)
    {
        int curr = fib->a;
        int next = fib->a + fib->b;
        fib->a = fib->b;
        fib->b = next;
        CO_YIELD(co, curr);
    }

    CO_END(co);
    return 0;
}

int get_fib_count(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 10;
    }
    char *endptr;
    long n = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || n <= 0)
    {
        fprintf(stderr, "Expected a positive integer argument.\n");
        exit(EXIT_FAILURE);
    }
    return (int)n;
}

int main(int argc, char *argv[])
{
    int n = get_fib_count(argc, argv);
    FibData fib = {.n = n};
    Co co;

    co_init(&co, fib_func, &fib);

    while (co.state != CO_FIN)
    {
        int val = co_next(&co);
        if (co.state != CO_FIN)
        {
            printf("%d ", val);
        }
    }
    printf("\n");

    return 0;
}
