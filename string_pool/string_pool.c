#include "string_pool.h"
#include "../common/error_handling.h"
#include "../common/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "global_pool.h"
#include "string.h"
#include "string_page.h"

// =====================================================================================================================
// MARK: STRING POOL
// =====================================================================================================================

String* string_pool_get_string(StringPool* pool, const char* str) {
	if (!pool) pool = get_global_pool();
	if (!pool)
		EXIT_ERROR("in StringPool and Global StringPool is Null");

	const size_t index = hash(str, HASH_TABLE_SIZE);
	String* result = string_pool_get_string_with_index(pool, str, index);

	if (result != NULL)
		result->ref_count--;

	return result;
}

String* string_pool_get_string_with_index(StringPool* pool, const char* str, const size_t index) {
	if (!pool) pool = get_global_pool();
	if (!pool)
		EXIT_ERROR("in StringPool and Global StringPool is Null");

	String* current = pool->hash_table[index];

	while (current) {
		if (strcmp(current->str, str) == 0) {
			current->ref_count++;
			return current;
		}
		current = current->next_collision;
	}

	return NULL;
}

size_t string_pool_count_ref(StringPool* pool) {
	if (!pool) pool = get_global_pool();
	if (!pool)
		EXIT_ERROR("in StringPool and Global StringPool is Null");

	size_t ref_count = 0;
	for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
		const String* current = pool->hash_table[i];
		while (current) {
			ref_count += current->ref_count;
			current = current->next_collision;
		}
	}
	return ref_count;
}

StringPool* string_pool_new() {
	StringPool* pool = sp_malloc(sizeof(StringPool));
	if (!pool)
		EXIT_ERROR("Failed to allocate String Pool");

	pool->count = 0;

	pool->string_page = string_page_alloc();

	for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
		pool->hash_table[i] = NULL;
	}

	return pool;
}


void string_pool_free(StringPool** in_pool) {
	if (!in_pool || !*in_pool)
		EXIT_ERROR("Invalid string pool");

	StringPool* pool = *in_pool;

	const size_t unreleased_count_ref = string_pool_count_ref(pool);
	const size_t unreleased_count_string = pool->count;

	for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
		String* current = pool->hash_table[i];
		while (current) {
			String* temp = current;
			current = current->next_collision;
			string_free(temp);
		}
	}

	string_page_free(pool->string_page);

	// ===============================================
	// Print Warning if there are unreleased strings
	if (unreleased_count_ref > 0 || unreleased_count_string > 0) {
		char buffer[62 + 20 + 20]; // 62 is the length of the string below and 20 is the max char length of a size_t
		snprintf(
			buffer,
			sizeof(buffer),
			"String Pool freed with %zu unreleased handle, for %zu strings",
			unreleased_count_ref,
			unreleased_count_string
		);
		WARN(buffer);
	}

	sp_free(pool);
	*in_pool = NULL;
}
