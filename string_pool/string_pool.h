#ifndef STRING_POOL_H
#define STRING_POOL_H

#include <stdbool.h>
#include <stddef.h>

// String Structure
typedef struct String String;
struct String {
    char *str;
    size_t length;
    size_t ref_count;
    String *next;
};

// String Pool Structure
typedef struct StringPool StringPool;
struct StringPool {
    String **hash_table;
    size_t count;
};

// String Pool API


StringPool *string_pool_new();
void string_pool_free(StringPool **in_pool);
String *string_new(StringPool *pool, const char *str);
void string_release(StringPool *pool, String **out_ptr_string);
String *string_pool_get_string(const StringPool *pool, const char *str);
bool string_cmp(String *first, ...);

#endif