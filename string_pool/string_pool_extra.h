#ifndef STRING_POOL_H
#define STRING_POOL_H

#include <stdbool.h>
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

size_t get_pool_ref_count(StringPool *pool);

// =====================================================================================================================
// MARK: String API
// =====================================================================================================================

/**
 * Create a new String and add it to the pool or get existing one with same value
 * @param pool Pointer to the StringPool
 * @param str The string to be duplicated
 * @return Pointer to the String
 */
String *string_new(StringPool *pool, const char *str);

/**
 * Release a String from the pool if its reference count is zero, otherwise decrement the reference count.
 * The input pointer is set to NULL after the string is released.
 * @param pool Pointer to the StringPool
 * @param out_ptr_string Pointer to the String pointer
 */
void string_release(StringPool *pool, String **out_ptr_string);

/**
 * Compare two strings
 * @param first Pointer to the first string
 * @param ... Variable number of Pointer to the strings to be compared, last one need to be NULL
 * @return true if the strings are equal, false otherwise
 */
bool string_cmp_va(const String *first, ...);

// =====================================================================================================================
// MARK: Global String Pool
// =====================================================================================================================

/**
 * Initialize the global string pool
 */
void init_global_pool();

/**
 * Deinitialize the global string pool
 */
void deinit_global_pool();

/**
 * Get the global string pool singleton and initialize it if it is not yet initialized
 * @return Pointer to the global string pool
 */
StringPool *get_global_pool_singleton();

#endif
