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

// Scopes
#define S_SCOPE_CONTEXT RES_SP___SCOPE_CONTEXT____
#define S_SCOPE_START() { ScopeContext* RES_SP___SCOPE_CONTEXT____ = scope_context_new();
#define S_SCOPE_END() scope_context_free(&RES_SP___SCOPE_CONTEXT____); }
#define S_SCOPE(block) S_SCOPE_START() block S_SCOPE_END()

#define S_SCOPE_NEW(str) scope_context_add_string(RES_SP___SCOPE_CONTEXT____, S_NEW(str))
#define S_SCOPE_REPLACE(original, target, replacement) scope_context_add_string(RES_SP___SCOPE_CONTEXT____, S_REPLACE(original, target, replacement))

// =====================================================================================================================
// MARK: StringBuilder API
// =====================================================================================================================

#define SB_CONTEXT RES_SP__STR_123_BUILDER__
#define SB_START(var) String* var = NULL;{ StringBuilder* SB_CONTEXT = string_builder_new(NULL);
#define SB_END(var) var = string_builder_to_string(SB_CONTEXT);string_builder_release(&SB_CONTEXT); }
#define SB(var, block) SB_START(var) block SB_END(var)

#define SB_APPEND(str) string_builder_append(SB_CONTEXT, str)
#define SB_APPEND_STR(str) string_builder_append_str(SB_CONTEXT, str)
#define SB_APPEND_INT(int) string_builder_append_format(SB_CONTEXT, "%d", int)
#define SB_APPEND_FORMAT(str, ...) string_builder_append_format(SB_CONTEXT, str, __VA_ARGS__)

#endif
