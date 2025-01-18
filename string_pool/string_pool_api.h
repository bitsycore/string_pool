#ifndef STRING_POOL_API_H
#define STRING_POOL_API_H

#include "string_pool.h"

// Global String Pool API
void deinit_global_pool();
StringPool *get_global_pool_singleton();
StringPool *get_global_pool();

// =====================================================================================================================
// MARK: MACRO API
// =====================================================================================================================

#define STRING_REF String*
#define STRING_NEW(str) string_new(NULL, str)
#define STRING_RELEASE(ptr) string_release(NULL, &ptr)
#define STRING_CMP(first, ...) string_cmp(first, __VA_ARGS__, NULL)

#endif
