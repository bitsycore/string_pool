#ifndef SCOPE_SCONTEXT_H
#define SCOPE_SCONTEXT_H

#include <stddef.h>

#include "string_pool_types.h"

typedef struct ScopeContext ScopeContext;

struct ScopeContext {
	String* string_array[SCOPE_CONTEXT_SIZE];
	size_t count;
	ScopeContext* next;
};

ScopeContext* scope_context_new();

String* scope_context_add_string(ScopeContext* context, String* string);

void scope_context_free(ScopeContext** context);


#endif //SCOPE_SCONTEXT_H
