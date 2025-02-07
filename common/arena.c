#include "arena.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct Block {
	size_t size;
	bool free;
	struct Block* next;
} Block;

typedef struct Area {
	struct Area* next;
	Block blocks[];
} Area;

// avoid excessive fragmentation
#define MIN_BLOCK_SIZE_CALCULATE  (ARENA_MIN_BLOCK_SIZE * sizeof(Block))

static Area* AREAS = NULL;

static Area* allocate_new_area() {
	if (ARENA_DEBUG) printf("[AREA] New Area Allocated\n");
	Area* new_area = malloc(sizeof(Area) + ARENA_SIZE);
	if (!new_area) return NULL;
	new_area->next = AREAS;
	AREAS = new_area;
	Block* block = new_area->blocks;
	block->size = ARENA_SIZE - sizeof(Block);
	block->free = true;
	block->next = NULL;
	return new_area;
}

static Block* find_free_block(const size_t size) {
	for (Area* area = AREAS; area; area = area->next) {
		Block* block = area->blocks;
		while (block) {
			if (block->free && block->size >= size) return block;
			block = block->next;
		}
	}
	return NULL;
}

static void split_block(Block* block, const size_t size) {
	if (block->size >= size + MIN_BLOCK_SIZE_CALCULATE) {
		Block* new_block = (Block*) ((uint8_t*) (block + 1) + size);
		new_block->size = block->size - size - sizeof(Block);
		new_block->free = true;
		new_block->next = block->next;
		block->size = size;
		block->next = new_block;
	}
}

static void merge_blocks() {
	for (Area* area = AREAS; area; area = area->next) {
		Block* block = area->blocks;
		while (block && block->next) {
			if (block->free && block->next->free) {
				block->size += block->next->size + sizeof(Block);
				block->next = block->next->next;
			} else {
				block = block->next;
			}
		}
	}
}

// ========================================================
// MARK: Public
// ========================================================

void* arena_malloc(const size_t size) {
	if (size == 0) return NULL;

	Block* block = find_free_block(size);
	if (block) {
		split_block(block, size);
		block->free = false;
		return block + 1;
	}

	Area* area = allocate_new_area();
	if (!area) {
		return NULL;
	}

	Block* new_block = area->blocks;
	split_block(new_block, size);
	new_block->free = false;
	return new_block + 1;
}

void* arena_calloc(const size_t num, const size_t size) {
	void* ptr = arena_malloc(num * size);
	if (ptr) memset(ptr, 0, num * size);
	return ptr;
}

void* arena_realloc(void* ptr, const size_t size) {
	if (!ptr) return arena_malloc(size);

	const Block* block = (Block*) ptr - 1;
	if (block->size >= size) return ptr;
	void* new_ptr = arena_malloc(size);
	if (new_ptr) {
		memcpy(new_ptr, ptr, block->size);
		arena_free(ptr);
	}
	return new_ptr;
}

void arena_free(void* ptr) {
	if (!ptr) return;
	Block* block = (Block*) ptr - 1;
	block->free = true;
	merge_blocks();
}

char* arena_strdup(const char* s) {
	int len = 0;
	while (s[len])
		len++;

	char* str = arena_malloc(len + 1);
	char* p = str;
	while (*s != '\0') {
		*p = *s;
		p++;
		s++;
	}
	*p = '\0';
	return str;
}

// ========================================================
// MARK: Area Managing
// ========================================================

void arena_preheat(const int num_areas) {
	for (int i = 0; i < num_areas; ++i) {
		if (allocate_new_area() == NULL) {
			fprintf(stderr, "Failed to preheat area %d\n", i);
			return;
		}
	}
}

void arena_destroy() {
	while (AREAS) {
		Area* next_area = AREAS->next;
		free(AREAS);
		AREAS = next_area;
	}
}
