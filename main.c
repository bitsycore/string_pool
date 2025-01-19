#include <stdio.h>

#include "string_pool/api.h"

void print_string_info(const STRING_PTR string) {
    printf("String: %s (ptr: %p, ref_count: %zu)\n", string->str, (void *) string->str, string->ref_count);
}

int main() {
    STRING_PTR ps1 = STRING_NEW("Hello");
    STRING_PTR ps2 = STRING_NEW("Hello");
    STRING_PTR ps3 = STRING_NEW("HellO");

    // ===============================================
    // STRING_CMP & STRING_CMP_VA test
    if (!STRING_CMP(ps1, ps2))
        return -1;

    printf("STRING_CMP: work.\n");

    if (STRING_CMP_VA(ps3, ps2, ps1))
        return -1;
    printf("STRING_CMP_VA: work.\n");


    // ===============================================
    // STRING_FIND test
    STRING_PTR ps1_eq = STRINGPOOL_FIND("Hello");

    if (!STRING_CMP(ps1, ps1_eq))
        return -1;

    printf("STRING_FIND: work.\n");

    // ===============================================
    // Lots of init test
    STRING_PTR str_test = STRING_BUILDER_START()

        for (int k = 0; k < 10; k++) {
            for (int j = 0; j < 10; j++) {
                for (int i = 0; i < 10; i++) {
                    char buffer[10 + 20];
                    snprintf(buffer, sizeof(buffer), i % 2 == 0 ? "hello+%d+%d+%d" : "%d+%d+%d-world", i, j, k);
                    const STRING_PTR string = STRING_NEW(buffer);
                    STRING_BUILDER_APPEND_FORMAT(
                        "String: %s (ptr: %p, ref_count: %zu)\n",
                        string->str,
                        (void *) string->str,
                        string->ref_count
                    );
                }
            }
        }

    STRING_BUILDER_END(str_test);
    puts(str_test->str);

    // ===============================================
    // BUILDER TEST
    STRING_PTR final_string = STRING_BUILDER_START()

        const int age = 30;
        STRING_BUILDER_APPEND(ps2);
        STRING_BUILDER_APPEND_STR(" ");
        STRING_BUILDER_APPEND_STR("Name: ");
        STRING_BUILDER_APPEND_FORMAT("%s, ", "John");
        STRING_BUILDER_APPEND_STR("Age: ");
        STRING_BUILDER_APPEND_FORMAT("%d", age);

    STRING_BUILDER_END(final_string);

    puts(final_string->str);

    STRING_PTR abc = STRING_BUILDER_START()
        String *test = STRING_NEW("Test replace NEW, and the NEW");
        String *replaced = STRING_REPLACE(test, "NEW", "Hi");
        const int age = 30;
        STRING_BUILDER_APPEND(replaced);
        STRING_BUILDER_APPEND_STR(" ");
        STRING_BUILDER_APPEND_STR("Name: ");
        STRING_BUILDER_APPEND_FORMAT("%s, ", "John");
        STRING_BUILDER_APPEND_STR("Age: ");
        STRING_BUILDER_APPEND_FORMAT("%d", age);

    STRING_BUILDER_END(abc);

    puts(abc->str);

    // ===============================================
    // Release Pool and Quit
    STRINGPOOL_FREE();
    return 0;
}
