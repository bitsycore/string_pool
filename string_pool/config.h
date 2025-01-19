#ifndef CONFIG_H
#define CONFIG_H

#define HASH_TABLE_SIZE 8191 // Prime number 8191, 16381, or 32749
#define HASH_FUNCTION HASH_DJB2

#if HASH_FUNCTION == HASH_FNV1A
	#define HASH_SELECTED(str, table_size) hash_fnv1a(str, table_size)
#elif HASH_FUNCTION == HASH_OPT_FNV1A
	#define HASH_SELECTED(str, table_size) hash_fnv1a_optimized(str, table_size)
#elif HASH_FUNCTION == HASH_DJB2
	#define HASH_SELECTED(str, table_size) hash_djb2(str, table_size)
#elif HASH_FUNCTION == HASH_SDBM
	#define HASH_SELECTED(str, table_size) hash_sdbm(str, table_size)
#else
	#define HASH_SELECTED(str, table_size) hash_djb2(str, table_size)
#endif

#define hash(str, table_size) HASH_SELECTED(str, table_size)

#endif //CONFIG_H
