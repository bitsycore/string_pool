#include "private.h"

#include <stdarg.h>
#include <stdbool.h>

#include "string_pool.h"
#include "../common/error_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global String Pool now without static keyword
StringPool *GLOBAL_POOL = NULL;

// =====================================================================================================================
// MARK: HASH FUNCTION
// =====================================================================================================================

#define HASH_TABLE_SIZE 256

static unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = (int) *str++)) {
        hash = (hash << 5) + hash + c; // hash * 33 + c
    }
    return hash % HASH_TABLE_SIZE;
}

// =====================================================================================================================
// MARK: STRING
// =====================================================================================================================

static void string_free(String *ps) {
    free(ps->str);
    free(ps);
}

// Function to add a string to the pool
String *string_new(StringPool *pool, const char *str) {
    // If null, get singleton
    if (pool == NULL) {
        pool = get_global_pool_singleton();
    }

    const unsigned long index = hash(str);
    String *current = pool->hash_table[index];

    // Check for existing string
    while (current) {
        if (strcmp(current->str, str) == 0) {
            current->ref_count++; // Increment reference count
            return current; // Return existing string
        }
        current = current->next;
    }

    // Allocate memory for the new string
    String *new_string = malloc(sizeof(String));
    if (!new_string)
        EXIT_ERROR("Failed to allocate String");
    new_string->str = strdup(str);
    if (!new_string->str) {
        free(new_string);
        EXIT_ERROR("Failed to duplicate string");
    }
    new_string->length = strlen(str);
    new_string->ref_count = 1; // Initialize reference count to 1
    new_string->next = pool->hash_table[index]; // Insert at the head of the list
    pool->hash_table[index] = new_string;
    pool->count++;
    return new_string;
}

/**
 * Release a String from the pool if its reference count is zero, otherwise decrement the reference count.
 * The input pointer is set to NULL after the string is released.
 * @param pool Pointer to the StringPool
 * @param out_ptr_string Pointer to the String pointer
 */
void string_release(StringPool *pool, String **out_ptr_string) {
    if (pool == NULL) {
        pool = get_global_pool();
    }
    if (pool == NULL) {
        WARN("input ool and Global pool is NULL");
        return;
    }
    if (out_ptr_string == NULL || *out_ptr_string == NULL) {
        return;
    }
    String *ptr_string = *out_ptr_string;
    if (ptr_string->ref_count > 1) {
        ptr_string->ref_count--;
    } else {
        // Remove the string from the pool
        const unsigned long index = hash(ptr_string->str);
        String *current = pool->hash_table[index];
        String *prev = NULL;

        while (current) {
            if (current == ptr_string) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    pool->hash_table[index] = current->next;
                }
                string_free(current);
                pool->count--;
                break;
            }
            prev = current;
            current = current->next;
        }
    }
    *out_ptr_string = NULL;
}

bool string_cmp_va(String *first, ...) {
    if (first == NULL) return false;

    va_list args;
    va_start(args, first);

    String *current;
    while ((current = va_arg(args, String *)) != NULL) {
        if (first->str != current->str) {
            va_end(args);
            return false;
        }
    }

    va_end(args);
    return true;
}

// =====================================================================================================================
// MARK: STRING POOL
// =====================================================================================================================

// Function to get a pooled string from the pool by string value
String *string_pool_get_string(const StringPool *pool, const char *str) {
    if (!pool)
        EXIT_ERROR("String Pool is Null");

    const unsigned long index = hash(str);
    String *current = pool->hash_table[index];

    while (current) {
        if (strcmp(current->str, str) == 0) {
            current->ref_count++; // Increment reference count
            return current; // Return existing string
        }
        current = current->next;
    }
    return NULL; // Not found
}


// Function to create a new string pool
StringPool *string_pool_new() {
    StringPool *pool = malloc(sizeof(StringPool));
    if (!pool)
        EXIT_ERROR("Failed to allocate String Pool");

    pool->count = 0;
    pool->hash_table = calloc(HASH_TABLE_SIZE, sizeof(String *));
    if (!pool->hash_table)
        EXIT_ERROR("Failed to allocate String Hash Table");
    return pool;
}


void string_pool_free(StringPool **in_pool) {
    if (!in_pool || !*in_pool)
        EXIT_ERROR("Invalid string pool");

    StringPool *pool = *in_pool;
    size_t unreleased_count_ref = 0;
    size_t unreleased_count_string = 0;

    for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
        String *current = pool->hash_table[i];
        while (current) {
            String *temp = current;
            current = current->next;
            if (temp->ref_count > 1) {
                unreleased_count_ref += temp->ref_count; // count all the references
                unreleased_count_string++;
            }
            string_free(temp);
        }
    }
    if (unreleased_count_ref > 0) {
        char buffer[128]; // Adjust buffer size if needed
        snprintf(buffer, sizeof(buffer), "String Pool freed with %zu unreleased handle, for %zu strings", unreleased_count_ref, unreleased_count_string);
        WARN(buffer);
    }

    free(pool->hash_table);
    free(pool);
    *in_pool = NULL;
}

// =====================================================================================================================
// MARK: GLOBAL INITIALIZER
// =====================================================================================================================

StringPool *get_global_pool_singleton() {
    if (GLOBAL_POOL == NULL) {
        GLOBAL_POOL = string_pool_new();
    }
    return GLOBAL_POOL;
}

StringPool *get_global_pool() {
    return GLOBAL_POOL;
}

void deinit_global_pool() {
    string_pool_free(&GLOBAL_POOL);
    GLOBAL_POOL = NULL;
}

void init_global_pool() {
    get_global_pool_singleton();
}
