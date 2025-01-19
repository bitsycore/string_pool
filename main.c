#include <stdio.h>

#include "string_pool/string_pool.h"

void print_string_info(const STRING_PTR string) {
    printf("String: %s (ptr: %p, ref_count: %zu)\n", string->str, (void *) string->str, string->ref_count);
}

int main() {
    const STRING_PTR ps1 = STRING_NEW("Hello");

    const STRING_PTR ps1_eq = STRING_FIND("Hello");
    if (!STRING_CMP(ps1, ps1_eq))
        return -1;
    printf("STRING_FIND: work.\n");

    // ===============================================
    // Check pointer equality
    const STRING_PTR ps2 = STRING_NEW("Hello");
    const STRING_PTR ps3 = STRING_NEW("HellO");

    if (!STRING_CMP(ps1, ps2))
        return -1;
    printf("STRING_CMP: work.\n");
    if (STRING_CMP_VA(ps3, ps2, ps1))
        return -1;
    printf("STRING_CMP_VA: work.\n");

    #define SIZE 10
    STRING_PTR b[SIZE];

    for (int k = 0; k < 10; k++) {
        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < SIZE; i++) {
                char buffer[10 + 20];
                snprintf(buffer, sizeof(buffer), i % 2 == 0 ? "hello+%d+%d+%d" : "%d+%d+%d-world", i, j, k);
                STRING_PTR ps = STRING_NEW(buffer);
                b[i] = ps;
            }
        }
    }

    for (int i = 0; i < SIZE; i++) {
        print_string_info(b[i]);
        STRING_RELEASE(b[i]);
    }

    // ===============================================
    // Release Pool and Quit
    STRING_POOL_RELEASE();
    return 0;
}
