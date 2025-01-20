#include "alloc.h"

#include <stdint.h>

static int32_t sp_alloc = -1;

void sp_alloc_plus() {
	if (sp_alloc == -1)
		sp_alloc = 0;
	sp_alloc++;
}

void sp_alloc_minus() {
	if (sp_alloc == -1)
		sp_alloc = 0;
	sp_alloc--;
}

int32_t sp_alloc_get() {
	return sp_alloc;
}
