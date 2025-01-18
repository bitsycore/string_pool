#include <stdio.h>

#include "string_pool/string_pool.h"

void print_string_info(const STRING_REF string) {
    printf("String: %s (ptr: %p, ref_count: %zu)\n", string->str, (void *) string->str, string->ref_count);
}

int main() {
    STRING_REF ps1 = STRING_NEW("Hello");
    STRING_REF ps2 = STRING_NEW(" ");
    STRING_REF ps3 = STRING_NEW("World");
    STRING_REF ps4 = STRING_NEW("Hello");

    // Duplicate string
    STRING_REF ps5 = STRING_NEW("Hello");
    STRING_REF ps6 = STRING_NEW("Hello");
    STRING_REF ps7 = STRING_NEW("Hello");
    STRING_REF ps8 = STRING_NEW("Hello");
    STRING_REF ps9 = STRING_NEW("Hello");

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
