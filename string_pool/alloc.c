#include "alloc.h"

#include <stddef.h>

static size_t sp_alloc = 0;

void sp_alloc_plus() {
	sp_alloc++;
}

void sp_alloc_minus() {
	sp_alloc--;
}

size_t sp_alloc_get() {
	return sp_alloc;
}
