#ifndef STRING_POOL_H
#define STRING_POOL_H

#include "string_pool_types.h"

// =====================================================================================================================
// MARK: String Pool API
// =====================================================================================================================

/**
 * Create a new StringPool
 * @return Pointer to the new StringPool
 */
StringPool *string_pool_new();

/**
 * Free the memory allocated for the StringPool
 * @param in_pool Pointer to the StringPool pointer that is set to NULL after the pool is freed
 */
void string_pool_free(StringPool **in_pool);

/**
 * Find a string in the pool
 * @param pool Pointer to the StringPool
 * @param str The string to be found
 * @return Pointer to the String if found, NULL otherwise
 */
String *string_pool_find_string(StringPool *pool, const char *str);

/**
 * Find a string in the pool with a specific index
 * @param pool Pointer to the StringPool
 * @param str The string to be found
 * @param index The index of the string in the hash table
 * @return Pointer to the String if found, NULL otherwise
 */
String *string_pool_find_string_with_index(StringPool *pool, const char *str, size_t index);

size_t string_pool_count_ref(StringPool *pool);

#endif