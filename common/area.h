//
// Created by Doge on 07/02/2025.
//

#ifndef AREA_H
#define AREA_H

#include <stdbool.h>
#include <stddef.h>

#define AREA_DEBUG true
#define AREA_SIZE 1024 * 1024 * 64
#define MIN_BLOCK_SIZE 2

void* area_malloc(size_t size);
void* area_calloc(size_t num, size_t size);
void* area_realloc(void* ptr, size_t size);
void area_free(void* ptr);

char *area_strdup(const char *s);

void area_preheat(int num_areas);
void area_destroy();

#endif //AREA_H
