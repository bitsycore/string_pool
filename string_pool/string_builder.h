#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include "string_pool_types.h"

// =====================================================================================================================
// MARK: String Builder API
// =====================================================================================================================

/**
 * Create a new StringBuilder
 * @return Pointer to the new StringBuilder
 */
StringBuilder* string_builder_new(StringPool* pool);

/**
 * Append a string to the StringBuilder
 * @param builder Pointer to the StringBuilder
 * @param string The string to be appended
 */
void string_builder_append(StringBuilder* builder, String* string);

/**
 * Append a string to the StringBuilder
 * @param builder Pointer to the StringBuilder
 * @param str The string to be appended
 */
void string_builder_append_str(StringBuilder* builder, const char* str);

/**
 * Append a formatted string to the StringBuilder
 * @param builder Pointer to the StringBuilder
 * @param format The format string
 * @param ... Variable number of arguments
 */
void string_builder_append_format(StringBuilder* builder, const char* format, ...);


/**
 * Create a new String from the StringBuilder using the string pool
 * @param builder Pointer to the StringBuilder
 * @return Pointer to the new String, use STRING_RELEASE to free
 */
String* string_builder_to_string(StringBuilder* builder);

/**
 * Release the memory allocated for the StringBuilder
 * @param in_builder Pointer to the StringBuilder pointer that is set to NULL after the builder is freed
 */
void string_builder_release(StringBuilder** in_builder);

#endif //STRING_BUILDER_H
