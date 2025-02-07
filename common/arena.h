//
// Created by Doge on 07/02/2025.
//

#ifndef AREA_H
#define AREA_H

#include <stdbool.h>
#include <stddef.h>

#define ARENA_DEBUG true
#define ARENA_SIZE 1024 * 1024 * 64
#define ARENA_MIN_BLOCK_SIZE 2

void* arena_malloc(size_t size);

void* arena_calloc(size_t num, size_t size);

void* arena_realloc(void* ptr, size_t size);

void arena_free(void* ptr);

char* arena_strdup(const char* s);

void arena_preheat(int num_areas);

void arena_destroy();

#endif //AREA_H
