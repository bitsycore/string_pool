#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>
#include "config.h"

#if POOR_GRIND == 1
#define sp_malloc(size) malloc(size);sp_alloc_plus()
#define sp_calloc(count, size) calloc(count, size);sp_alloc_plus()
#define sp_realloc(ptr, size) realloc(ptr, size)
#define sp_free(ptr) free(ptr);sp_alloc_minus()
#define sp_strdup(str) strdup(str);sp_alloc_plus()
#elif POOR_GRIND == 0
#define sp_malloc(size) malloc(size)
#define sp_calloc(count, size) calloc(count, size)
#define sp_realloc(ptr, size) realloc(ptr, size)
#define sp_free(ptr) free(ptr)
#define sp_strdup(str) strdup(str)
#endif

void sp_alloc_plus();

void sp_alloc_minus();

size_t sp_alloc_get();

#endif //ALLOC_H
