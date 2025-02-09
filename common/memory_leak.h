#ifndef MEMORY_LEAK_H
#define MEMORY_LEAK_H

#include "../string_pool/config.h"
#include <stdbool.h>
#include <stdlib.h>

// ============================================
// MARK: LEAK DETECTOR
// ============================================

#if ENABLE_LEAK_DETECTOR == true

void* imp_ml_malloc(void* (*custom_malloc)(size_t), size_t size, const char* file, int line);
void* imp_ml_calloc(void* (*custom_calloc)(size_t, size_t), size_t num, size_t size, const char* file, int line);
void* imp_ml_realloc(void* (*custom_realloc)(void*, size_t), void* ptr, size_t size, const char* file, int line);
void imp_ml_free(void (*custom_free)(void*), void* ptr);
char* imp_ml_strdup(void* (*custom_malloc)(size_t), const char* s, const char* file, int line);

#define ml_malloc(size) imp_ml_malloc(malloc, size, __FILE__, __LINE__)
#define ml_calloc(num, size) imp_ml_calloc(calloc, num, size, __FILE__, __LINE__)
#define ml_realloc(ptr, size) imp_ml_realloc(realloc, ptr, size, __FILE__, __LINE__)
#define ml_free(ptr) imp_ml_free(free, ptr)
#define ml_strdup(s) imp_ml_strdup(malloc, s, __FILE__, __LINE__)

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
