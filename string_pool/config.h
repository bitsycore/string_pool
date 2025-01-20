#ifndef CONFIG_H
#define CONFIG_H

// ====================
// Hash type "enum"
#define HASH_FNV1A 1
#define HASH_OPT_FNV1A 2
#define HASH_DJB2 3
#define HASH_SDBM 4
#define HASH_MURMUR3_32 5

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

// ====================
// MARK: CONFIG
#define HASH_FUNCTION HASH_OPT_FNV1A
#define HASH_TABLE_SIZE 8191 // Prime number 8191, 16381, or 32749
#define SCOPE_CONTEXT_SIZE 64
#define STRING_PAGE_SIZE 128
#define POOR_GRIND 0


#endif // CONFIG_H