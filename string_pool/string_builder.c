#include "string_builder.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "../common/alloc.h"
#include "../common/memory_leak.h"

#include "global_pool.h"
#include "string.h"
#include "string_pool_types.h"
#include "../common/error_handling.h"

StringBuilder* string_builder_new(StringPool* pool) {
	StringBuilder* builder = ml_malloc(sizeof(StringBuilder));
	if (!builder)
		EXIT_ERROR("Failed to allocate StringBuilder");

	builder->pool = pool ? pool : get_global_pool_singleton();
	builder->strings = NULL;
	builder->count = 0;

	return builder;
}

static StringBuilderNode* string_builder_node_new(const char* str) {
	StringBuilderNode* node = ml_malloc(sizeof(StringBuilderNode));
	if (!node)
		EXIT_ERROR("Failed to allocate StringBuilderNode");

	node->type = STRING_BUILDER_NODE_TYPE_STR;
	node->str = ml_strdup(str);
	if (!node->str) {
		ml_free(node);
		EXIT_ERROR("Failed to allocate memory for strdup");
	}

	node->next = NULL;
	return node;
}

static StringBuilderNode* string_builder_node_new_string(String* string) {
	StringBuilderNode* node = ml_malloc(sizeof(StringBuilderNode));
	if (!node)
		EXIT_ERROR("Failed to allocate StringBuilderNode");
	node->type = STRING_BUILDER_NODE_TYPE_STRING;
	node->string = string;
	string->ref_count++;
	node->next = NULL;
	return node;
}

void string_builder_append_str(StringBuilder* builder, const char* str) {
	if (!builder || !str) {
		WARN("input StringBuilder or str is NULL");
		return;
	}

	StringBuilderNode* node = string_builder_node_new(str);

	// Insert in the head
	node->next = builder->strings;
	builder->strings = node;
	builder->count++;
}

void string_builder_append(StringBuilder* builder, String* string) {
	if (!builder || !string) {
		WARN("input StringBuilder or str is NULL");
		return;
	}

	StringBuilderNode* node = string_builder_node_new_string(string);

	// Insert in the head
	node->next = builder->strings;
	builder->strings = node;
	builder->count++;
}

void string_builder_append_format(StringBuilder* builder, const char* format, ...) {
	if (!builder || !format) {
		WARN("input StringBuilder or format is NULL");
		return;
	}

	va_list args;
	va_start(args, format);

	// Estimate buffer size
	const int size = vsnprintf(NULL, 0, format, args);
	va_end(args);

	if (size <= 0) {
		WARN("Failed to format string, invalid format ?");
		return;
	}

	char* buffer = ml_malloc(size + 1);
	if (!buffer)
		EXIT_ERROR("Failed to allocate memory for formatted string");

	va_start(args, format);
	vsnprintf(buffer, size + 1, format, args);
	va_end(args);


	string_builder_append_str(builder, buffer);
	ml_free(buffer);
}

void string_builder_append_string(StringBuilder* builder, String* string) {
	if (!builder || !string) {
		WARN("input StringBuilder or string is NULL");
		return;
	}

	StringBuilderNode* node = string_builder_node_new_string(string);

	// Insert in the head
	node->next = builder->strings;
	builder->strings = node;
	builder->count++;
}


String* string_builder_to_string(StringBuilder* builder) {
	if (!builder) {
		WARN("input StringBuilder is NULL");
		return NULL;
	}

	if (builder->count == 0) return string_new(builder->pool, "");

	size_t total_length = 0;
	StringBuilderNode* current = builder->strings;

	// Count total length and store the string in array
	StringBuilderNode** nodes = ml_malloc(sizeof(StringBuilderNode*) * builder->count);
	if (!nodes)
		EXIT_ERROR("Failed to allocate memory for nodes array");


	size_t i = builder->count;
	while (current) {
		if (current->type == STRING_BUILDER_NODE_TYPE_STR)
			total_length += strlen(current->str);
		else if (current->type == STRING_BUILDER_NODE_TYPE_STRING)
			total_length += current->string->length;


		nodes[--i] = current;
		current = current->next;
	}

	char* result = ml_malloc(total_length + 1);
	if (!result)
		EXIT_ERROR("Failed to allocate result string");
	result[0] = '\0';

	for (size_t j = 0; j < builder->count; j++) {
		if (nodes[j]->type == STRING_BUILDER_NODE_TYPE_STR)
			strcat(result, nodes[j]->str);
		else if (nodes[j]->type == STRING_BUILDER_NODE_TYPE_STRING) {
			strcat(result, nodes[j]->string->str);
		}
	}

	String* final_string = string_new(builder->pool, result);

	ml_free(result);
	ml_free(nodes);

	return final_string;
}


void string_builder_release(StringBuilder** in_builder) {
	if (!in_builder || !*in_builder) {
		WARN("input StringBuilder pointer or pointer pointer is NULL, already been freed ?");
		return;
	}

	StringBuilder* builder = *in_builder;
	StringBuilderNode* current = builder->strings;
	while (current) {
		StringBuilderNode* temp = current;
		current = current->next;

		if (temp->type == STRING_BUILDER_NODE_TYPE_STR) {
			ml_free(temp->str);
		} else if (temp->type == STRING_BUILDER_NODE_TYPE_STRING) {
			string_release(&temp->string);
		}

		ml_free(temp);
	}

	ml_free(builder);
	*in_builder = NULL;
}
