#ifndef STRING_POOL_TYPES_H
#define STRING_POOL_TYPES_H

#include <stddef.h>

#define HASH_TABLE_SIZE 256

typedef struct String String;
typedef struct StringPool StringPool;

struct String {
    char *str;
    size_t length;
    size_t ref_count;
    String *next;
};

struct StringPool {
    String** hash_table;
    size_t count;
};

#endif //STRING_POOL_TYPES_H
