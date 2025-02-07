#include "memory_leak.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if ENABLE_LEAK_DETECTOR == true

typedef struct AllocationInfo {
	void* address;
	size_t size;
	const char* file;
	int line;
	struct AllocationInfo* next;
} AllocationInfo;

static AllocationInfo* ALLOCATION_LIST = NULL;

static void add_allocation(void* address, const size_t size, const char* file, const int line) {
	AllocationInfo* new_allocation = malloc(sizeof(AllocationInfo));
	if (new_allocation == NULL) {
		fputs("Error: Failed to allocate memory for allocation tracking!\n", stderr);
		return;
	}
	new_allocation->address = address;
	new_allocation->size = size;
	new_allocation->file = file;
	new_allocation->line = line;
	new_allocation->next = ALLOCATION_LIST;
	ALLOCATION_LIST = new_allocation;
}

// Function to remove allocation information from the list
static void remove_allocation(const void* address) {
	AllocationInfo* current = ALLOCATION_LIST;
	AllocationInfo* previous = NULL;

	while (current != NULL) {
		if (current->address == address) {
			if (previous == NULL) {
				ALLOCATION_LIST = current->next;
			} else {
				previous->next = current->next;
			}
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}


void* imp_ml_malloc(void* (*custom_malloc)(size_t), const size_t size, const char* file, const int line) {
	void* ptr = custom_malloc(size);
	if (ptr != NULL) {
		add_allocation(ptr, size, file, line);
	}
	return ptr;
}


void* imp_ml_calloc(void* (*custom_calloc)(size_t, size_t), const size_t num, const size_t size, const char* file,
                    const int line) {
	void* ptr = custom_calloc(num, size);
	if (ptr != NULL) {
		const size_t total_size = num * size;
		add_allocation(ptr, total_size, file, line);
	}
	return ptr;
}

void* imp_ml_realloc(void* (*custom_realloc)(void*, size_t), void* ptr, const size_t size, const char* file,
                     const int line) {
	void* new_ptr = custom_realloc(ptr, size);
	if (new_ptr != NULL) {
		if (ptr != NULL) {
			remove_allocation(ptr); // Remove old allocation information
		}

		add_allocation(new_ptr, size, file, line);
	}
	return new_ptr;
}

void imp_ml_free(void (*custom_free)(void*), void* ptr) {
	if (ptr != NULL) {
		const AllocationInfo* current = ALLOCATION_LIST;
		while (current != NULL) {
			if (current->address == ptr) {
				break;
			}
			current = current->next;
		}
		remove_allocation(ptr);
		custom_free(ptr);
	}
}

char* imp_ml_strdup(void* (*custom_malloc)(size_t), const char* s, const char* file, const int line) {
	if (s == NULL) return NULL;
	const size_t len = strlen(s) + 1;
	char* new_str = imp_ml_malloc(custom_malloc, len, file, line);
	if (new_str != NULL) {
		memcpy(new_str, s, len);
	}
	return new_str;
}

void ml_print_memory_leaks() {
	const AllocationInfo* current = ALLOCATION_LIST;

	if (current == NULL) {
		return;
	}

	size_t total_leaked_memory = 0;
	size_t number_leaked_block = 0;

	printf("\n--------------------- [MEMORY LEAK DETECTED] ---------------------\n\n");
	while (current != NULL) {
		if ((double) current->size >= 1024 * 1024 * 2.5) {
			printf("[%zu]\t0x%p\t%.2f MB\t%s:%d\n",
			       number_leaked_block, current->address,
			       (double) current->size / (1024 * 1024),
			       current->file, current->line);
		} else if ((double) current->size >= 1024 * 2.5) {
			printf("[%zu]\t0x%p\t%.2f KB\t%s:%d\n",
			       number_leaked_block, current->address,
			       (double) current->size / 1024,
			       current->file, current->line);
		} else {
			printf("[%zu]\t0x%p\t%zu Bytes\t%s:%d\n",
			       number_leaked_block, current->address,
			       current->size,
			       current->file, current->line);
		}
		total_leaked_memory += current->size;
		current = current->next;
		number_leaked_block++;
	}
	printf(
		"\n------------------------------------------------------------------\nBlocks: %zu\t%zu bytes (%.2f KB, %.2f MB)\n------------------------------------------------------------------\n",
		number_leaked_block, total_leaked_memory, (double) total_leaked_memory / 1024.0,
		(double) total_leaked_memory / (1024.0 * 1024.0));
}

void ml_cleanup_memory_tracking() {
	AllocationInfo* current = ALLOCATION_LIST;

	while (current != NULL) {
		AllocationInfo* next = current->next;
		free(current);
		current = next;
	}

	ALLOCATION_LIST = NULL;
}

#endif
