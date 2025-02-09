//
// Created by Doge on 08/02/2025.
//

#include "alloc.h"
#include <string.h>
#include "arena.h"

void* re_malloc(size_t size) {
	return arena_alloc(NULL, size);
}

void* re_calloc(size_t size) {
	void* ptr =  arena_alloc(NULL, size);
	memset(ptr, 0, size);
	return ptr;
}

void re_free(void* ptr) {

}

void re_strdup(const char* str) {
	size_t len = strlen(str);
	char* new_str = re_malloc(len + 1);
	strcpy(new_str, str);
}