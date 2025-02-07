#ifndef MEMORY_LEAK_H
#define MEMORY_LEAK_H

#include "../string_pool/config.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================
// MARK: AREA
// ============================================

#if USE_ARENA == true
#include "arena.h"
#define selected_malloc arena_malloc
#define selected_calloc arena_calloc
#define selected_realloc arena_realloc
#define selected_free arena_free
#define selected_strdup arena_strdup
#else
#include <stdlib.h>
#define selected_malloc malloc
#define selected_calloc calloc
#define selected_realloc realloc
#define selected_free free
#define selected_strdup strdup
#endif

// ============================================
// MARK: LEAK DETECTOR
// ============================================

#if ENABLE_LEAK_DETECTOR == true

void* imp_ml_malloc(void* (*custom_malloc)(size_t), size_t size, const char* file, int line);

void* imp_ml_calloc(void* (*custom_calloc)(size_t, size_t), size_t num, size_t size, const char* file, int line);

void* imp_ml_realloc(void* (*custom_realloc)(void*, size_t), void* ptr, size_t size, const char* file, int line);

void imp_ml_free(void (*custom_free)(void*), void* ptr);

char* imp_ml_strdup(void* (*custom_malloc)(size_t), const char* s, const char* file, int line);

#define ml_malloc(size) imp_ml_malloc(selected_malloc, size, __FILE__, __LINE__)
#define ml_calloc(num, size) imp_ml_calloc(selected_calloc, num, size, __FILE__, __LINE__)
#define ml_realloc(ptr, size) imp_ml_realloc(selected_realloc, ptr, size, __FILE__, __LINE__)
#define ml_free(ptr) imp_ml_free(selected_free, ptr)
#define ml_strdup(s) imp_ml_strdup(selected_malloc, s, __FILE__, __LINE__)

void ml_print_memory_leaks();

void ml_cleanup_memory_tracking();

#else

#define ml_malloc(size) selected_malloc(size)
#define ml_calloc(count, size) selected_calloc(count, size)
#define ml_realloc(ptr, size) selected_realloc(ptr, size)
#define ml_free(ptr) selected_free(ptr)
#define ml_strdup(str) selected_strdup(str)

#define ml_print_memory_leaks() {}
#define ml_cleanup_memory_tracking() {}

#endif

#endif //MEMORY_LEAK_H
