#ifndef HASH_H
#define HASH_H

#include <stddef.h>

size_t hash_fnv1a(const char* str, size_t table_size);

size_t hash_fnv1a_optimized(const char* str, size_t table_size, size_t sample_size);

size_t hash_djb2(const char* str, size_t table_size);

size_t hash_sdbm(const char* str, size_t table_size);

size_t hash_murmur3_32(const char* str, size_t table_size);

#endif //HASH_H
