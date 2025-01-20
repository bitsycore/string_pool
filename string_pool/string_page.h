//
// Created by dwa on 20/01/2025.
//

#ifndef STRING_PAGE_H
#define STRING_PAGE_H
#include "string_pool_types.h"

StringPage *string_page_alloc();

void string_page_free(StringPage *string_page);

String* string_page_next_string(StringPage *string_page);

#endif //STRING_PAGE_H
