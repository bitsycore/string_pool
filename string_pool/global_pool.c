#include "global_pool.h"

#include "string_pool.h"

static StringPool *GLOBAL_POOL = NULL;

StringPool *get_global_pool() {
    return GLOBAL_POOL;
}

StringPool *get_global_pool_singleton() {
    init_global_pool();
    return GLOBAL_POOL;
}

void deinit_global_pool() {
    string_pool_free(&GLOBAL_POOL);
}

void init_global_pool() {
    if (GLOBAL_POOL == NULL) {
        GLOBAL_POOL = string_pool_new();
    }
}