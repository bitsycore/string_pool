#ifndef ALLOC_H
#define ALLOC_H

#include "arena.h"

void* re_malloc(size_t size);
void* re_calloc(size_t size);
void re_free(void* ptr);
void re_strdup(const char* str);

#define selected_malloc re_malloc
#define selected_calloc re_calloc
#define selected_realloc realloc
#define selected_free re_free
#define selected_strdup re_strdup

#endif //ALLOC_H
