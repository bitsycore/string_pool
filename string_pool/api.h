#ifndef STRING_POOL_API_H
#define STRING_POOL_API_H

#include "string_pool_types.h"
#include "string.h"
#include "string_builder.h"
#include "string_pool.h"
#include "global_pool.h"

// =====================================================================================================================
// MARK: StringPool API
// =====================================================================================================================

#define SP_FREE() deinit_global_pool();
#define SP_COUNT_STR() get_global_pool_singleton()->count
#define SP_COUNT_REF() string_pool_count_ref(NULL)
#define SP_FIND(str) string_pool_find_string(NULL, str);

// =====================================================================================================================
// MARK: String API
// =====================================================================================================================

#define S_NEW(str) string_new(NULL, str)
#define S_RELEASE(ptr) string_release(NULL, &ptr)

#define S_CMP(first, second) (first->str == second->str)
#define S_CMP_VA(first, ...) string_cmp_va(first, __VA_ARGS__, NULL)
#define S_LENGTH(ptr) ptr->length
#define S_REPLACE(original, target, replacement) string_replace(NULL, original, target, replacement)

// =====================================================================================================================
// MARK: StringBuilder API
// =====================================================================================================================

#define SB_START() (String*) NULL;{ StringBuilder* __STR_123_BUILDER__ = string_builder_new(NULL);

#define SB_APPEND(str) string_builder_append(__STR_123_BUILDER__, str)
#define SB_APPEND_STR(str) string_builder_append_str(__STR_123_BUILDER__, str)
#define SB_APPEND_INT(int) string_builder_append_format(__STR_123_BUILDER__, "%d", int)
#define SB_APPEND_FORMAT(str, ...) string_builder_append_format(__STR_123_BUILDER__, str, __VA_ARGS__)
#define STRING_BUILDER_TO_STRING() string_builder_to_string(__STR_123_BUILDER__)

#define SB_END(final) final = string_builder_to_string(__STR_123_BUILDER__);string_builder_release(&__STR_123_BUILDER__); }

#endif
