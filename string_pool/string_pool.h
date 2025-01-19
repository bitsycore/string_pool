#ifndef STRING_POOL_API_H
#define STRING_POOL_API_H

#include "string_pool_extra.h"

// =====================================================================================================================
// MARK: Simplified API
// =====================================================================================================================

#define STRING_PTR String*
#define STRING_NEW(str) string_new(NULL, str)
#define STRING_RELEASE(ptr) string_release(NULL, &ptr)

#define STRING_CMP(first, second) (first->str == second->str)
#define STRING_CMP_VA(first, ...) string_cmp_va(first, __VA_ARGS__, NULL)
#define STRING_LENGTH(ptr) ptr->length
#define STRING_FIND(str) string_pool_find_string(NULL, str)

#define STRING_POOL_RELEASE() deinit_global_pool();
#define STRING_POOL_COUNT() get_global_pool_singleton()->count
#define STRING_POOL_REF() get_pool_ref_count(NULL)

#endif
