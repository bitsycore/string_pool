#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

#include "string_pool_types.h"

/**
 * Free the memory allocated for the String
 * @param ps Pointer to the String
 */
void string_free(String* ps);

/**
 * Allocate memory for a new String
 * @param str The string to be duplicated
 * @return Pointer to the new String
 */
String* string_alloc(const char* str);

/**
 * Create a new String and add it to the pool or get existing one with same value
 * @param pool Pointer to the StringPool
 * @param str The string to be duplicated
 * @return Pointer to the String
 */
String* string_new(StringPool* pool, const char* str);

/**
 * Release a String from the pool if its reference count is zero, otherwise decrement the reference count.
 * The input pointer is set to NULL after the string is released.
 * @param pool Pointer to the StringPool
 * @param out_ptr_string Pointer to the String pointer
 */
void string_release(StringPool* pool, String** out_ptr_string);

/**
 * Compare two strings
 * @param first Pointer to the first string
 * @param ... Variable number of Pointer to the strings to be compared, last one need to be NULL
 * @return true if the strings are equal, false otherwise
 */
bool string_cmp_va(const String* first, ...);

/**
 * Replace all occurrences of a target string with a replacement string
 * @param pool Pointer to the StringPool
 * @param original Pointer to the original String
 * @param target The target string to be replaced
 * @param replacement The replacement string
 * @return Pointer to the new string
 */
String* string_replace(StringPool* pool, const String* original, const char* target, const char* replacement);

/**
 * Replace all occurrences of a target string with a replacement string, this need to be freed.
 * @param original The original string
 * @param target The target string to be replaced
 * @param replacement The replacement string
 * @return Pointer to the new string
 */
const char* string_replace_str(const char* original, const char* target, const char* replacement);

#endif //STRING_H
