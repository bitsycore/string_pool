#include <stdio.h>

#include "string_pool/string_pool.h"

void print_string_info(const STRING_REF string) {
    printf("String: %s (ptr: %p, ref_count: %zu)\n", string->str, (void *) string->str, string->ref_count);
}

int main() {
    STRING_REF ps1 = STRING_NEW("Hello");
    STRING_REF ps2 = STRING_NEW("World");
    STRING_REF ps3 = STRING_NEW("World5");
    STRING_REF ps4 = STRING_NEW("Hello5");

    STRING_REF pool[100];

    for (int k = 0; k < 10000; k++) {
        for (int j = 0; j < 100; j++) {
            for (int i = 0; i < 100; i++) {
                char buffer[64];
                snprintf(buffer, sizeof(buffer), "hello+%d", (i*j));
                STRING_REF ps = STRING_NEW(buffer);
                pool[i] = ps;
            }
        }
    }

    for (int i = 0; i < 100; i++) {
        print_string_info(pool[i]);
    }

    // Duplicate string
    STRING_REF ps5 = STRING_NEW("Hello6");
    STRING_REF ps6 = STRING_NEW("Hello7");
    STRING_REF ps7 = STRING_NEW("Hello8");
    STRING_REF ps8 = STRING_NEW("Hello9");
    STRING_REF ps9 = STRING_NEW("Hello10");

    print_string_info(ps1);
    print_string_info(ps2);
    print_string_info(ps3);
    print_string_info(ps4);
    print_string_info(ps5);
    print_string_info(ps6);
    print_string_info(ps7);
    print_string_info(ps8);
    print_string_info(ps9);

    // Check pointer equality
    if (STRING_CMP(ps1, ps4) && STRING_CMP_VA(ps5, ps6, ps7))
        printf("ps1 and ps4 point to the same string.\n");
    else
        printf("ps1 and ps4 point to different strings.\n");

    // Release strings
    STRING_RELEASE(ps1);
    STRING_RELEASE(ps2);
    STRING_RELEASE(ps3);
    STRING_RELEASE(ps4);
    STRING_RELEASE(ps5);

    STRING_POOL_RELEASE();

    return 0;
}
