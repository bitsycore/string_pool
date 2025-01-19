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
    STRING_PTR str_test = SB_START()

        for (int k = 0; k < 10; k++) {
            for (int j = 0; j < 10; j++) {
                for (int i = 0; i < 10; i++) {
                    char buffer[10 + 20 + 20 + 20];
                    snprintf(buffer, sizeof(buffer), "%d+%d+%d-world", i, j, k);
                    const STRING_PTR string = STRING_NEW(buffer);
                    SB_APPEND_FORMAT(
                        "String: %s (ptr: %p, ref_count: %zu)\n",
                        string->str,
                        (void *) string->str,
                        string->ref_count
                    );
                }
            }
        }

    SB_END(str_test);
    puts(str_test->str);

    // ===============================================
    // BUILDER TEST
    STRING_PTR final_string = SB_START()

        const int age = 30;
        SB_APPEND(ps2);
        SB_APPEND_STR(" --- ");
        SB_APPEND_STR("Name: ");
        SB_APPEND_FORMAT("%s, ", "John");
        SB_APPEND_STR("Age: ");
        SB_APPEND_INT(age);

    SB_END(final_string);

    puts(final_string->str);

    STRING_PTR abc = SB_START()
        String *test = STRING_NEW("Test replace NEW, and the NEW");
        String *replaced = STRING_REPLACE(test, "NEW", "Hi");
        const int age = 30;
        SB_APPEND(replaced);
        SB_APPEND_STR(" ");
        SB_APPEND_STR("Name: ");
        SB_APPEND_FORMAT("%s, ", "John");
        SB_APPEND_STR("Age: ");
        SB_APPEND_FORMAT("%d", age);

    SB_END(abc);

    puts(abc->str);

    // ===============================================
    // Release Pool and Quit
    STRINGPOOL_FREE();
    return 0;
}
