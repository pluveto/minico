#include <stdio.h>
#include <stdlib.h>
#include "co.h"

typedef struct {
    int number;
    int factor;
    int current;
} PrimeFactorData;

int prime_factor_co(Co *co, void *data)
{
    PrimeFactorData *pfData = (PrimeFactorData *)data;
    CO_BEGIN(co);

    pfData->current = 2;
    while (pfData->number > 1)
    {
        while (pfData->number % pfData->current == 0)
        {
            pfData->number /= pfData->current;
            pfData->factor = pfData->current;
            CO_YIELD(co, pfData->factor);
        }
        pfData->current++;
        CO_YIELD(co, 0); // Yield to indicate progress
    }

    CO_END(co);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int number = atoi(argv[1]);
    if (number <= 1)
    {
        fprintf(stderr, "Expected a number greater than 1.\n");
        return 1;
    }

    Co co;
    PrimeFactorData data = {number, 0, 2}; // Initialize with the input number
    co_init(&co, prime_factor_co, &data);

    while (!co_done(&co))
    {
        int factor = co_next(&co);
        if (factor > 0)
        {
            printf("Found Factor: %d\n", factor);
            printf("Continue? ([Y]/n) ");
            char c = getchar();
            if (c == 'n' || c == 'N')
            {
                co_stop(&co);
            }
        }
    }

    printf("Done.\n");

    return 0;
}
