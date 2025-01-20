#ifndef STRING_POOL_TYPES_H
#define STRING_POOL_TYPES_H

#include <stddef.h>

#include "config.h"

typedef struct String String;
typedef struct StringPool StringPool;
typedef struct StringBuilder StringBuilder;
typedef struct StringBuilderNode StringBuilderNode;

struct String {
	char* str;
	size_t length;
	size_t ref_count;
	String* _next;
	size_t _hash_index;
};

struct StringPool {
	String* hash_table[HASH_TABLE_SIZE];
	size_t count;
};

struct StringBuilderNode {
	enum {
		STRING_BUILDER_NODE_TYPE_STR,
		STRING_BUILDER_NODE_TYPE_STRING,
	} type;

	union {
		char* str;
		String* string;
	};

	StringBuilderNode* next;
};

struct StringBuilder {
	StringPool* pool;
	StringBuilderNode* strings;
	size_t count;
};

#endif //STRING_POOL_TYPES_H
