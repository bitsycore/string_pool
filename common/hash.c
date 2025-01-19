#include "hash.h"

#include <string.h>

size_t hash_fnv1a(const char* str, const size_t table_size) {
	size_t hash_value = 2166136261u;
	while (*str) {
		hash_value ^= (unsigned char) *str++;
		hash_value *= 16777619;
	}
	return hash_value % table_size;
}

size_t hash_fnv1a_optimized(const char *str, const size_t table_size) {
	size_t hash_value = 2166136261u;

	// Hash first 16 characters
	for (size_t i = 0; i < 16 && *str; i++) {
		hash_value ^= (unsigned char)*str++;
		hash_value *= 16777619;
	}

	// Skip to the last 16 characters if the string is longer
	if (*str) {
		const char *end = str;
		size_t length = 0;
		while (*end++) length++;
		if (length > 16) {
			str += length - 16;
		}

		// Hash last 16 characters
		while (*str) {
			hash_value ^= (unsigned char)*str++;
			hash_value *= 16777619;
		}
	}

	return hash_value % table_size;
}

size_t hash_djb2(const char* str, const size_t table_size) {
	size_t hash_value = 5381;
	while (*str) {
		hash_value = ((hash_value << 5) + hash_value) + *str++; // hash * 33 + char
	}
	return hash_value % table_size;
}

size_t hash_sdbm(const char* str, const size_t table_size) {
	size_t hash_value = 0;
	while (*str) {
		hash_value = *str++ + (hash_value << 6) + (hash_value << 16) - hash_value;
	}
	return hash_value % table_size;
}
