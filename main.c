#include <stdio.h>

#include "string_pool/api.h"

int main() {
	S_SCOPE_START

		String* ps1 = S_SCOPE_NEW("Hello");
		String* ps2 = S_SCOPE_NEW("Hello");
		String* ps3 = S_SCOPE_NEW("HellO");

		// ===============================================
		// STRING_CMP & STRING_CMP_VA test
		if (!S_CMP(ps1, ps2))
			return -1;
		printf("STRING_CMP: work.\n");

		if (S_CMP_VA(ps3, ps2, ps1))
			return -1;
		printf("STRING_CMP_VA: work.\n");


		// ===============================================
		// STRING_FIND test
		String* ps1_eq = SP_GET_STRING("Hello");

		if (!S_CMP(ps1, ps1_eq))
			return -1;

		printf("STRING_FIND: work.\n");

		// ===============================================
		// Lots of init test
		for (int j = 0; j < 100; j++) {
			String* str_test = SB_START()
				for (int i = 0; i < 100; i++) {
					char buffer[10 + 20];
					snprintf(buffer, sizeof(buffer), i % 2 == 0 ? "%d-WORLD" : "%d-HELLO", j * 100 + i);
					const String* string = scope_context_add_string(__SCOPE_CONTEXT__,S_NEW(buffer));;
					SB_APPEND_FORMAT(
						"String: %s (ptr: %p, ref_count: %zu)\n",
						string->str,
						(void *) string->str,
						string->ref_count
					);
				}
			SB_END(str_test);
			puts(str_test->str);
			S_RELEASE(str_test);
		}
		// ===============================================
		// BUILDER TEST
		// String* final_string = SB_START()
		//
		// 	const int age = 30;
		// 	SB_APPEND(ps2);
		// 	SB_APPEND_STR(" --- ");
		// 	SB_APPEND_STR("Name: ");
		// 	SB_APPEND_FORMAT("%s, ", "John");
		// 	SB_APPEND_STR("Age: ");
		// 	SB_APPEND_INT(age);
		//
		// SB_END(final_string);
		//
		// puts(final_string->str);
		// S_RELEASE(final_string);

		// String* abc = SB_START()
		//
		// 	String* test = S_SCOPE_NEW("Test replace NEW, and the NEW");
		// 	String* replaced = S_REPLACE(test, "NEW", "Hi");
		// 	const int age = 11;
		// 	SB_APPEND_STR(" ");
		// 	SB_APPEND_STR("Name: ");
		// 	SB_APPEND_FORMAT("%s, ", "BOB");
		// 	SB_APPEND_STR("Age: ");
		// 	SB_APPEND_FORMAT("%d", age);
		// 	S_RELEASE(replaced);
		// SB_END(abc);
		//
		// puts(abc->str);
		// S_RELEASE(abc);

	S_SCOPE_END

	// ===============================================
	// Release Pool and Quit
	SP_FREE();
	return 0;
}
