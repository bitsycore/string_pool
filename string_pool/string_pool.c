#include "string_pool.h"
#include "../common/error_handling.h"
#include "../common/hash.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StringPool *GLOBAL_POOL = NULL;

// =====================================================================================================================
// MARK: STRING
// =====================================================================================================================

/**
 * Free the memory allocated for the String
 * @param ps Pointer to the String
 */
static void string_free(String *ps) {
    free(ps->str);
    free(ps);
}

/**
 * Allocate memory for a new String
 * @param str The string to be duplicated
 * @return Pointer to the new String
 */
static String *string_alloc(const char *str) {
    String *new_string = malloc(sizeof(String));
    if (!new_string)
        EXIT_ERROR("Failed to allocate String");
    new_string->str = strdup(str);
    if (!new_string->str) {
        free(new_string);
        EXIT_ERROR("Failed to duplicate string");
    }
    new_string->length = strlen(str);
    new_string->ref_count = 1;
    return new_string;
}

String *string_new(StringPool *pool, const char *str) {
    // =========================================
    // If null, Get Singleton
    if (pool == NULL) {
        pool = get_global_pool_singleton();
    }

    // =========================================
    // Get position in hash table
    const size_t index = hash(str, HASH_TABLE_SIZE);

    // =========================================
    // Check if the string already exists
    String *current = string_pool_find_string_with_index(pool, str, index);
    if (current)
        return current;

    // =========================================
    // Else Allocate memory for the new string
    String *new_string = string_alloc(str);

    // =========================================
    // Insert in the hash table at the head
    new_string->next = pool->hash_table[index];
    pool->hash_table[index] = new_string;
    pool->count++;

    return new_string;
}

void string_release(StringPool *pool, String **out_ptr_string) {
    // =========================================
    // String Pointer is NULL
    if (out_ptr_string == NULL || *out_ptr_string == NULL) {
        WARN("input String pointer or pointer pointer is NULL, already been freed ?");
        return;
    }
    // =========================================
    // If null, Get Singleton
    if (pool == NULL) pool = GLOBAL_POOL;
    if (pool == NULL) {
        WARN("input StringPool and Global StringPool is NULL");
        return;
    }

    String *ptr_string = *out_ptr_string;

    if (ptr_string->ref_count > 1) {
        ptr_string->ref_count--;
    } else {
        const size_t index = hash(ptr_string->str, HASH_TABLE_SIZE);
        String *current = pool->hash_table[index];
        String *prev = NULL;

        while (current) {
            if (current == ptr_string) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    pool->hash_table[index] = current->next;
                }
                current->next = NULL;
                string_free(current);
                pool->count--;
                break;
            }
            prev = current;
            current = current->next;
        }
    }

    // =========================================
    // Set the pointer to NULL
    *out_ptr_string = NULL;
}

bool string_cmp_va(const String *first, ...) {
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

String *string_pool_find_string(StringPool *pool, const char *str) {
    if (!pool) pool = GLOBAL_POOL;
    if (!pool)
        EXIT_ERROR("in StringPool and Global StringPool is Null");

    const size_t index = hash(str, HASH_TABLE_SIZE);

    return string_pool_find_string_with_index(pool, str, index);
}

String *string_pool_find_string_with_index(StringPool *pool, const char *str, const size_t index) {
    if (!pool) pool = GLOBAL_POOL;
    if (!pool)
        EXIT_ERROR("in StringPool and Global StringPool is Null");

    String *current = pool->hash_table[index];

    while (current) {
        if (strcmp(current->str, str) == 0) {
            current->ref_count++;
            return current;
        }
        current = current->next;
    }

    return NULL;
}

size_t get_pool_ref_count(StringPool *pool) {
    if (!pool) pool = GLOBAL_POOL;
    if (!pool) EXIT_ERROR("in StringPool and Global StringPool is Null");

    size_t ref_count = 0;
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
        const String *current = pool->hash_table[i];
        while (current) {
            ref_count += current->ref_count;
            current = current->next;
        }
    }
    return ref_count;
}

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

    const size_t unreleased_count_ref = get_pool_ref_count(pool);
    const size_t unreleased_count_string = pool->count;

    for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
        String *current = pool->hash_table[i];
        while (current) {
            String *temp = current;
            current = current->next;
            string_free(temp);
        }
    }

    // ===============================================
    // Print Warning if there are unreleased strings
    if (unreleased_count_ref > 0 || unreleased_count_string > 0) {
        char buffer[62 + 20 + 20]; // 62 is the length of the string below and 20 is the max char length of a size_t
        snprintf(
            buffer,
            sizeof(buffer),
            "String Pool freed with %zu unreleased handle, for %zu strings",
            unreleased_count_ref,
            unreleased_count_string
        );
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
    init_global_pool();
    return GLOBAL_POOL;
}

void deinit_global_pool() {
    string_pool_free(&GLOBAL_POOL);
}

void init_global_pool() {
    if (GLOBAL_POOL == NULL) {
        GLOBAL_POOL = string_pool_new();
    }
}
