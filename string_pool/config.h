#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// ====================
// Hash type "enum"
#define HASH_FNV1A 1
#define HASH_OPT_FNV1A 2
#define HASH_DJB2 3
#define HASH_SDBM 4
#define HASH_MURMUR3_32 5

// ====================
// MARK: CONFIG
#define HASH_FUNCTION HASH_MURMUR3_32
#define HASH_TABLE_SIZE 32749 // Prime number 8191, 16381, or 32749
#define SCOPE_CONTEXT_SIZE 64
#define STRING_PAGE_SIZE 128

#define ENABLE_LEAK_DETECTOR true
#define USE_ARENA true

// ====================
// SELECTOR
#if HASH_FUNCTION == HASH_FNV1A
#define hash(str, table_size) hash_fnv1a(str, table_size)
#elif HASH_FUNCTION == HASH_OPT_FNV1A
#define hash(str, table_size) hash_fnv1a_optimized(str, table_size, 32)
#elif HASH_FUNCTION == HASH_DJB2
#define hash(str, table_size) hash_djb2(str, table_size)
#elif HASH_FUNCTION == HASH_SDBM
#define hash(str, table_size) hash_sdbm(str, table_size)
#elif HASH_FUNCTION == HASH_MURMUR3_32
#define hash(str, table_size) hash_murmur3_32(str, table_size)
#else
#define hash(str, table_size) hash_djb2(str, table_size)
#endif

#endif // CONFIG_H