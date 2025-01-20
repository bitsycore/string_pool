//
// Created by Doge on 19/01/2025.
//

#include "string.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "config.h"
#include "global_pool.h"
#include "string_page.h"
#include "string_pool.h"

#include "../common/error_handling.h"
#include "../common/hash.h"

// =========================================
// MARK: MEMORY MANAGEMENT
// =========================================

void string_free(String* ps) {
	sp_free(ps->str);
}

String* string_next(StringPage * string_page, const char * str, size_t index) {
	String* new_string = string_page_next_string(string_page);
	new_string->str = sp_strdup(str);
	new_string->length = strlen(str);
	new_string->ref_count = 1;
	new_string->_hash_index = index;
	return new_string;
}

// =========================================
// MARK: ADD and REMOVE
// =========================================

String* string_new(StringPool* pool, const char* str) {
	// =========================================
	// If null, Get Singleton
	if (pool == NULL) {
		pool = get_global_pool_singleton();
	}

	// =========================================
	// Get position in hash table
	const size_t index = hash(str, HASH_TABLE_SIZE);

	// =========================================
	// Check if the string already exists
	String* current = string_pool_get_string_with_index(pool, str, index);
	if (current)
		return current;

	// =========================================
	// Else Allocate memory for the new string
	String* new_string = string_next(pool->string_page, str, index);

	// =========================================
	// Insert in the hash table at the head
	new_string->_next = pool->hash_table[index];
	pool->hash_table[index] = new_string;
	pool->count++;

	return new_string;
}

void string_release(StringPool* pool, String** out_ptr_string) {
	// =========================================
	// String Pointer is NULL
	if (out_ptr_string == NULL || *out_ptr_string == NULL) {
		WARN("input String pointer or pointer pointer is NULL, already been freed ?");
		return;
	}
	// =========================================
	// If null, Get Singleton
	if (pool == NULL) pool = get_global_pool();
	if (pool == NULL) {
		WARN("input StringPool and Global StringPool is NULL");
		return;
	}

	String* ptr_string = *out_ptr_string;

	if (ptr_string->ref_count > 1) {
		ptr_string->ref_count--;
	} else {
		ptr_string->ref_count--;
		String* current = pool->hash_table[ptr_string->_hash_index];
		String* prev = NULL;

		while (current) {
			if (current == ptr_string) {
				if (prev) prev->_next = current->_next;
				else pool->hash_table[current->_hash_index] = current->_next;
				current->_next = NULL;
				string_free(current);
				pool->count--;
				break;
			}
			prev = current;
			current = current->_next;
		}
	}

	// =========================================
	// Set the pointer to NULL
	*out_ptr_string = NULL;
}

bool string_cmp_va(const String* first, ...) {
	if (first == NULL) return false;

	va_list args;
	va_start(args, first);

	String* current;
	while ((current = va_arg(args, String *)) != NULL) {
		if (first->str != current->str) {
			va_end(args);
			return false;
		}
	}

	va_end(args);
	return true;
}

static char* internal_string_replace_str(const char* original_str, const size_t original_str_int, const char* target,
                                         const size_t target_len, const char* replacement,
                                         const size_t replacement_len) {
	size_t result_len = 0;
	size_t alloc_len = original_str_int + 1;
	char* result_str = sp_malloc(alloc_len);
	if (!result_str)
		EXIT_ERROR("Failed to allocate memory for replace result string");

	const char* found;
	const char* current_ptr = original_str;

	while ((found = strstr(current_ptr, target)) != NULL) {
		const size_t prefix_len = found - current_ptr;

		if (result_len + prefix_len + replacement_len >= alloc_len) {
			alloc_len *= 2;
			char* temp_ptr = sp_realloc(result_str, alloc_len);
			if (!temp_ptr) {
				sp_free(result_str);
				EXIT_ERROR("Failed to reallocate memory for replace result string");
			}
			result_str = temp_ptr;
		}

		strncpy(result_str + result_len, current_ptr, prefix_len);
		result_len += prefix_len;
		strncpy(result_str + result_len, replacement, replacement_len);
		result_len += replacement_len;

		current_ptr = found + target_len;
	}

	const size_t remaining_len = strlen(current_ptr);
	if (result_len + remaining_len >= alloc_len) {
		alloc_len = result_len + remaining_len + 1;
		char* temp_ptr = sp_realloc(result_str, alloc_len);
		if (!temp_ptr) {
			sp_free(result_str);
			EXIT_ERROR("Failed to reallocate memory for replace result string");
		}
		result_str = temp_ptr;
	}
	strncpy(result_str + result_len, current_ptr, remaining_len);
	result_len += remaining_len;
	result_str[result_len] = '\0';

	return result_str;
}

String* string_replace(StringPool* pool, const String* original, const char* target, const char* replacement) {
	if (!original) {
		return NULL;
	}

	if (!target || !replacement) {
		return (String*) original;
	}

	const size_t target_len = strlen(target);
	if (target_len == 0) {
		return (String*) original;
	}

	if (pool == NULL) {
		pool = get_global_pool_singleton();
	}

	const char* result_str = internal_string_replace_str(
		original->str,
		original->length,
		target,
		target_len,
		replacement,
		strlen(replacement)
	);
	String* result = string_new(pool, result_str);
	sp_free((void*) result_str);

	return result;
}

const char* string_replace_str(const char* original, const char* target, const char* replacement) {
	if (!original) {
		return NULL;
	}

	if (!target || !replacement) {
		return original;
	}

	const size_t target_len = strlen(target);

	if (target_len == 0) {
		return original;
	}

	const char* result_str = internal_string_replace_str(
		original,
		strlen(original),
		target,
		target_len,
		replacement,
		strlen(replacement)
	);

	return result_str;
}
