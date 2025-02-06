#ifndef ALLOC_H
#define ALLOC_H

#include <stdio.h>
#include <stdbool.h>
#include "../string_pool/config.h"

#if ENABLE_LEAK_DETECTOR == true

void* imp_sp_malloc(size_t size, const char* file, int line);
void* imp_sp_calloc(size_t num, size_t size, const char* file, int line);
void* imp_sp_realloc(void* ptr, size_t size, const char* file, int line);
void imp_sp_free(void* ptr);
char* imp_sp_strdup(const char* s, const char* file, int line);
void sp_print_memory_leaks();
void sp_cleanup_memory_tracking();

#define sp_malloc(size) imp_sp_malloc(size, __FILE__, __LINE__)
#define sp_calloc(num, size) imp_sp_calloc(num, size, __FILE__, __LINE__)
#define sp_realloc(ptr, size) imp_sp_realloc(ptr, size, __FILE__, __LINE__)
#define sp_free(ptr) imp_sp_free(ptr)
#define sp_strdup(s) imp_sp_strdup(s, __FILE__, __LINE__)

#if OVERRIDE_REAL_ALLOC == true
#define malloc(size) imp_sp_malloc(size, __FILE__, __LINE__)
#define calloc(num, size) imp_sp_calloc(num, size, __FILE__, __LINE__)
#define realloc(ptr, size) imp_sp_realloc(ptr, size, __FILE__, __LINE__)
#define free(ptr) imp_sp_free(ptr)
#define strdup(s) imp_sp_strdup(s, __FILE__, __LINE__)
#endif

#else

#define sp_malloc(size) malloc(size)
#define sp_calloc(count, size) calloc(count, size)
#define sp_realloc(ptr, size) realloc(ptr, size)
#define sp_free(ptr) free(ptr)
#define sp_strdup(str) strdup(str)
#define sp_print_memory_leaks() {}

#endif

#endif //ALLOC_H
