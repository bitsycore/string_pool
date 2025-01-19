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

#define STRINGPOOL_FREE() deinit_global_pool();
#define STRINGPOOL_COUNT_STR() get_global_pool_singleton()->count
#define STRINGPOOL_COUNT_REF() get_pool_ref_count(NULL)
#define STRINGPOOL_FIND(str) string_pool_find_string(NULL, str);

// =====================================================================================================================
// MARK: String API
// =====================================================================================================================

#define STRING_PTR String*

#define STRING_NEW(str) string_new(NULL, str)
#define STRING_RELEASE(ptr) string_release(NULL, &ptr)

#define STRING_CMP(first, second) (first->str == second->str)
#define STRING_CMP_VA(first, ...) string_cmp_va(first, __VA_ARGS__, NULL)
#define STRING_LENGTH(ptr) ptr->length
#define STRING_REPLACE(original, target, replacement) string_replace(NULL, original, target, replacement)

// =====================================================================================================================
// MARK: StringBuilder API
// =====================================================================================================================

#define STRING_BUILDER_PTR StringBuilder*

#define STRING_BUILDER_START() (STRING_PTR) NULL;{ StringBuilder* __STR_123_BUILDER__ = string_builder_new(NULL);

#define STRING_BUILDER_APPEND(str) string_builder_append(__STR_123_BUILDER__, str)
#define STRING_BUILDER_APPEND_STR(str) string_builder_append_str(__STR_123_BUILDER__, str)
#define STRING_BUILDER_APPEND_FORMAT(str, ...) string_builder_append_format(__STR_123_BUILDER__, str, __VA_ARGS__)
#define STRING_BUILDER_TO_STRING() string_builder_to_string(__STR_123_BUILDER__)

#define STRING_BUILDER_END(final) final = string_builder_to_string(__STR_123_BUILDER__);string_builder_release(&__STR_123_BUILDER__); }

#endif