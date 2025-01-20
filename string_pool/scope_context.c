//
// Created by Doge on 20/01/2025.
//

#include "scope_context.h"
#include "../common/error_handling.h"

#include <stdlib.h>

#include "string.h"

ScopeContext* scope_context_new() {
	ScopeContext* context = malloc(sizeof(ScopeContext));
	if (!context)
		EXIT_ERROR("Failed to allocate ScopeContext");
	context->count = 0;
	context->next = NULL;
	return context;
}

String* scope_context_add_string(ScopeContext* context, String* string) {
	// NOLINT(*-no-recursion)
	if (!context || !string) {
		return NULL;
	}

	if (context->count >= 64) {
		context->next = scope_context_new();
		context = context->next;
	}

	context->strings[context->count++] = string;

	return string;
}

void scope_context_free(ScopeContext* context) {
	// NOLINT(*-no-recursion)
	if (!context) {
		return;
	}

	for (size_t i = 0; i < context->count; ++i) {
		String* node = context->strings[i];
		if (node) {
			string_release(NULL, &node);
		}
	}

	if (context->next) {
		scope_context_free(context->next);
	}

	free(context);
}
