//
// Created by Doge on 20/01/2025.
//

#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>

#define sp_malloc(size) malloc(size);sp_alloc_plus()
#define sp_calloc(count, size) calloc(count, size);sp_alloc_plus()
#define sp_realloc(ptr, size) realloc(ptr, size)
#define sp_free(ptr) free(ptr);sp_alloc_minus()
#define sp_strdup(str) strdup(str);sp_alloc_plus()

void sp_alloc_plus();
void sp_alloc_minus();
size_t sp_alloc_get();

#endif //ALLOC_H
