#ifndef STRING_POOL_API_H
#define STRING_POOL_API_H

#include "private.h"

// =====================================================================================================================
// MARK: MACRO API
// =====================================================================================================================

#define STRING_REF String*
#define STRING_NEW(str) string_new(NULL, str)
#define STRING_RELEASE(ptr) string_release(NULL, &ptr)
#define STRING_CMP(first, second) (first->str == second->str)
#define STRING_CMP_VA(first, ...) string_cmp_va(first, __VA_ARGS__, NULL)
#define STRING_POOL_RELEASE() deinit_global_pool();

#endif
