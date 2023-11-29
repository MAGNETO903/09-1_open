// main.c
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "random_source.h"

enum
{
    CMD_ARGC_REQUIRED = 3,
    GENERATOR_TYPE_ARGV_IDX = 1,
    NUM_AMOUNT_ARGV_IDX = 2,
    GENERATOR_FACTORY_PARAM_ARGV_IDX = 3,
    FUNC_NAME_MAX_LEN = 255
};

int
main(int argc, char *argv[])
{

    if (argc != CMD_ARGC_REQUIRED + 1) {
        fprintf(stderr, "Required 3 cmd args\n");
        return -1;
    }

    char *generator_type = argv[GENERATOR_TYPE_ARGV_IDX];
    int amount = atoi(argv[NUM_AMOUNT_ARGV_IDX]);
    char *params = argv[GENERATOR_FACTORY_PARAM_ARGV_IDX];

    void *handle = dlopen(NULL, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Failed to dlopen\n");
        return -1;
    }

    char factory_func_name[FUNC_NAME_MAX_LEN];
    if (FUNC_NAME_MAX_LEN < snprintf(factory_func_name, FUNC_NAME_MAX_LEN, "random_%s_factory", generator_type)) {
        fprintf(stderr, "Function name too long\n");
        return -1;
    }

    RandomSource *(*factory_func)(const char *) = dlsym(handle, factory_func_name);
    if (!factory_func) {
        fprintf(stderr, "Factory function not found\n");
        dlclose(handle);
        return -1;
    }

    RandomSource *my_rand_generator = factory_func(params);
    if (!my_rand_generator) {
        fprintf(stderr, "Error creating generator\n");
        dlclose(handle);
        return 1;
    }

    for (int i = 0; i < amount; i++) {
        double random_num = my_rand_generator->ops->next(my_rand_generator);
        printf("%.10g\n", random_num);
    }
    // destroying
    my_rand_generator->ops->destroy(my_rand_generator);
    dlclose(handle);

    return 0;
}
