// random_source.h
#ifndef __RANDOM_SOURCE_H__
#define __RANDOM_SOURCE_H__

typedef struct RandomSourceOperations
{
    struct RandomSource *(*destroy)(struct RandomSource *src);
    double (*next)(struct RandomSource *src);
} RandomSourceOperations;

typedef struct RandomSource
{
    RandomSourceOperations *ops;
} RandomSource;

#endif
