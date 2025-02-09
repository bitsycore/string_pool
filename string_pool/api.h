#ifndef STRING_POOL_API_H
#define STRING_POOL_API_H

#include "string_pool_types.h"
#include "global_pool.h"
#include "scope_context.h"
#include "string.h"
#include "string_builder.h"
#include "string_pool.h"

// =====================================================================================================================
// MARK: StringPool API
// =====================================================================================================================

#define SP_GLOBAL_FREE() deinit_global_pool();
#define SP_GET_STRING(string) string_pool_get_string(NULL, string);

// =====================================================================================================================
// MARK: String API
// =====================================================================================================================

#define S_NEW(str) string_new(NULL, str)
#define S_RELEASE(ptr) string_release(&ptr)

#define S_CMP(first, second) (first->str == second->str)
#define S_CMP_VA(first, ...) string_cmp_va(first, __VA_ARGS__, NULL)

#define S_LENGTH(ptr) ptr->length

#define S_REPLACE(original, target, replacement) string_replace(NULL, original, target, replacement)
#define S_REPLACE_STR(original, target, replacement) string_replace_str(NULL, original, target, replacement)

// =====================================================================================================================
// MARK: Scopes
// =====================================================================================================================

#define S_SCOPE_CTX RES_SP_SCOPE____CONTEXT____

#define S_SCOPE() \
	for (\
	ScopeContext* S_SCOPE_CTX = scope_context_new(); \
	S_SCOPE_CTX != NULL; \
	scope_context_free(&S_SCOPE_CTX) \
	)

#define S_SCOPE_NEW(str) scope_context_add_string(S_SCOPE_CTX, S_NEW(str))
#define S_SCOPE_INSERT(str) scope_context_add_string(S_SCOPE_CTX, str)
#define S_SCOPE_REPLACE(original, target, replacement) scope_context_add_string(S_SCOPE_CTX, S_REPLACE(original, target, replacement))

// =====================================================================================================================
// MARK: StringBuilder API
// =====================================================================================================================

#define SB_CTX RES_SP_STRING_BUILDER____CONTEXT____

/**
 * Creates a {@link StringBuilder} that build its result to a {@link String} created in global {@link StringPool}.
 * @param var Name of the variable that will be declared and assigned the result of the builder.
 */
#define SB(var) \
	String* var = (String*)0xFEFEFE;\
	for\
	(\
	/*INIT*/ StringBuilder* SB_CTX = string_builder_new(NULL); \
	/*COND*/ SB_CTX != NULL; \
	/*END */ var = string_builder_to_string(SB_CTX), string_builder_release(&SB_CTX) \
	)

/**
 * Creates a {@link StringBuilder} that build its result to a {@link String} created in global {@link StringPool}.
 * An error indicates the absence of outer S_SCOPE (or S_SCOPE_CTX parameter).
 * @param var Name of the variable that will be declared and assigned the result of the builder.
 */
#define SB_SCOPED(var) \
	String* var = (String*)0xFEFEFE;\
	for\
	(\
	/*INIT*/ StringBuilder* SB_CTX = string_builder_new(NULL); \
	/*COND*/ SB_CTX != NULL; \
	/*END */ var = string_builder_to_string(SB_CTX), string_builder_release(&SB_CTX), scope_context_add_string(S_SCOPE_CTX, var) \
	)

#define SB_START(var) NULL;{ StringBuilder* SB_CTX = string_builder_new(NULL);
#define SB_END(var) var = string_builder_to_string(SB_CTX);string_builder_release(&SB_CTX); }

#define SB_APPEND(str) string_builder_append(SB_CTX, str)
#define SB_APPEND_STR(str) string_builder_append_str(SB_CTX, str)
#define SB_APPEND_INT(int) string_builder_append_format(SB_CTX, "%d", int)
#define SB_APPEND_FORMAT(str, ...) string_builder_append_format(SB_CTX, str, __VA_ARGS__)
#define SB_TO_STRING(var) var = string_builder_to_string(SB_CTX);

#endif
