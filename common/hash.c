#include "hash.h"

#include <stdint.h>
#include <string.h>

uint32_t hash_fnv1a(const char* str, const uint32_t table_size) {
	uint32_t hash_value = 2166136261u;
	while (*str) {
		hash_value ^= (unsigned char) *str++;
		hash_value *= 16777619;
	}
	return hash_value % table_size;
}

uint32_t hash_fnv1a_optimized(const char* str, const uint32_t table_size, const uint32_t sample_size) {
	uint32_t hash_value = 2166136261u;

	// Hash first sample characters
	for (size_t i = 0; i < sample_size && *str; i++) {
		hash_value ^= (unsigned char) *str++;
		hash_value *= 16777619;
	}

	// Skip to the last "sample" characters if the string is longer
	if (*str) {
		const char* end = str;
		size_t length = 0;
		while (*end++) length++;
		if (length > sample_size) {
			str += length - sample_size;
		}

		// Hash last "sample" characters
		while (*str) {
			hash_value ^= (unsigned char) *str--;
			hash_value *= 16777619;
		}
	}

	return hash_value % table_size;
}

uint32_t hash_djb2(const char* str, const uint32_t table_size) {
	uint32_t hash_value = 5381;
	while (*str) {
		hash_value = ((hash_value << 5) + hash_value) + *str++; // hash * 33 + char
	}
	return hash_value % table_size;
}

uint32_t hash_sdbm(const char* str, const uint32_t table_size) {
	uint32_t hash_value = 0;
	while (*str) {
		hash_value = *str++ + (hash_value << 6) + (hash_value << 16) - hash_value;
	}
	return hash_value % table_size;
}

uint32_t hash_murmur3_32(const char* str, const uint32_t table_size) {
	uint32_t len = 0;
	while (str[len] != '\0') {
		len++;
	}

	const uint32_t seed = 2166136261u;
	uint32_t h = seed;
	const uint32_t c1 = 0xcc9e2d51;
	const uint32_t c2 = 0x1b873593;
	const int r1 = 15;

	const uint32_t nblocks = len / 4;
	const uint32_t* blocks = (const uint32_t*)str;

	for (uint32_t i = 0; i < nblocks; i++) {
		uint32_t k = blocks[i];
		k *= c1;
		k = (k << r1) | (k >> (32 - r1));
		k *= c2;

		h ^= k;
		h = (h << 13) | (h >> (32 - 13));
		h = h * 5 + 0xe6546b64;
	}

	const uint8_t* tail = (const uint8_t*)(str + nblocks * 4);
	uint32_t k1 = 0;

	switch (len & 3) {
		case 3:
			k1 ^= tail[2] << 16;
		break;
		case 2:
			k1 ^= tail[1] << 8;
		break;
		case 1:
			k1 ^= tail[0];
		break;
		default:
			break;
	}

	if (k1 != 0) {
		k1 *= c1;
		k1 = (k1 << r1) | (k1 >> (32 - r1));
		k1 *= c2;
		h ^= k1;
	}

	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;

	return h % table_size;
}
