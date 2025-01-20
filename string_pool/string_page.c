//
// Created by dwa on 20/01/2025.
//

#include "string_page.h"

#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "string_pool_types.h"

StringPage *string_page_alloc() {
    StringPage *string_page = sp_malloc(sizeof(StringPage));
    string_page->count = 0;
    string_page->next = NULL;

    memset(string_page->strings, 0, sizeof(String) * STRING_PAGE_SIZE);

    return string_page;
}

void string_page_free(StringPage *string_page) {
    if (!string_page) return;
    string_page_free(string_page->next);
    sp_free(string_page);
}

String* string_page_next_string(StringPage *string_page) {
    if (!string_page) return NULL;

    if (string_page->count >= STRING_PAGE_SIZE) {
        if (!string_page->next) {
            string_page->next = string_page_alloc();
        }
        return string_page_next_string(string_page->next);
    }

    return &string_page->strings[string_page->count++];
}