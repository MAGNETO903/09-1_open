// linear.c
#include <stdlib.h>
#include "random_source.h"
#include <stdio.h>

enum
{
    MULTIPLIER = 1103515245,
    INCREMENT = 12345,
    MODULUS = 0x7FFFFFFF
};

typedef struct
{
    RandomSource base;
    unsigned int seed;
} LinearRandom;

static RandomSource *
linear_destroy(RandomSource *src)
{
    free(src);
    return NULL;
}

static double
linear_next(RandomSource *src)
{
    LinearRandom *lr = (LinearRandom *) src;
    lr->seed = lr->seed * MULTIPLIER + INCREMENT;
    return (double) (lr->seed & MODULUS) / MODULUS;
}

RandomSource *
random_linear_factory(const char *params)
{

    LinearRandom *linear_random_instance = malloc(sizeof(LinearRandom));
    if (linear_random_instance == NULL) {
        fprintf(stderr, "Error allocating memory for RandomRandom");
        return NULL;
    }

    linear_random_instance->base.ops = malloc(sizeof(struct RandomSourceOperations));
    if (linear_random_instance->base.ops == NULL) {
        fprintf(stderr, "Error allocating memory for RandomSourceOperations");
        free(linear_random_instance);
        return NULL;
    }

    // Заполнение операций
    linear_random_instance->base.ops->destroy = linear_destroy;
    linear_random_instance->base.ops->next = linear_next;

    unsigned int seed;
    if (sscanf(params, "%u", &seed) < 1) {
        return NULL;
    }

    linear_random_instance->seed = seed;

    return &(linear_random_instance->base);
}
