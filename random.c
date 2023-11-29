// random.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "random_source.h"
#include <stdint.h>
#include <limits.h>

typedef struct
{
    RandomSource base;
    int urandom_fd;
} RandomRandom;

static RandomSource *
random_destroy(RandomSource *src)
{
    RandomRandom *rr = (RandomRandom *) src;
    close(rr->urandom_fd);
    free(src);
    return NULL;
}

static double
random_next(RandomSource *src)
{
    RandomRandom *rr = (RandomRandom *) src;

    uint64_t value;

    if (rr->urandom_fd == -1) {
        fprintf(stderr, "Error opening /dev/urandom\n");
        return -1.0;
    }
    ssize_t bytesRead = read(rr->urandom_fd, &value, sizeof(value));

    if (bytesRead != sizeof(value)) {
        fprintf(stderr, "Error reading from /dev/urandom\n");
        return -1.0;
    }

    // Максимальное значение для uint64_t + 1
    double max_uint64_t = (double) (UINT64_MAX) + 1.0;

    // Преобразование в интервал [0, 1)
    double result = (double) value / max_uint64_t;

    return result;
}

RandomSource *
random_random_factory(const char *params)
{
    RandomRandom *rr = malloc(sizeof(RandomRandom));
    if (rr == NULL) {
        fprintf(stderr, "Error allocating memory for RandomRandom");
        return NULL;
    }

    rr->base.ops = malloc(sizeof(struct RandomSourceOperations));
    if (rr->base.ops == NULL) {
        fprintf(stderr, "Error allocating memory for RandomSourceOperations");
        free(rr);
        return NULL;
    }

    // Заполнение операций
    rr->base.ops->destroy = random_destroy;
    rr->base.ops->next = random_next;

    rr->urandom_fd = open("/dev/urandom", O_RDONLY);

    if (rr->urandom_fd == -1) {
        fprintf(stderr, "Error opening /dev/urandom");
        free(rr);
        return NULL;
    }

    return &(rr->base);
}
