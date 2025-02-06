//
// Created by Doge on 20/01/2025.
//

#include "scope_context.h"
#include "../common/error_handling.h"

#include <stdlib.h>

#include "../common/alloc.h"
#include "string.h"

ScopeContext* scope_context_new() {
	ScopeContext* context = sp_malloc(sizeof(ScopeContext));
	if (!context)
		EXIT_ERROR("Failed to allocate ScopeContext");
	context->count = 0;
	context->next = NULL;
	return context;
}

String* scope_context_add_string(ScopeContext* context, String* string) {
	if (!context || !string) {
		return NULL;
	}

	while (context->count >= SCOPE_CONTEXT_SIZE) {
		if (context->next) {
			context = context->next;
		} else {
			context->next = scope_context_new();
			context = context->next;
		}
	}

	context->string_array[context->count++] = string;

	return string;
}


void scope_context_free(ScopeContext** context) { // NOLINT(*-no-recursion)
	if (!context || !*context) {
		return;
	}

	ScopeContext* in_context = *context;

	for (size_t i = 0; i < in_context->count; ++i) {
		String* node = in_context->string_array[i];
		if (node) {
			string_release(&node);
		}
	}

	scope_context_free(&in_context->next);

	sp_free(*context);
	context = NULL;
}
