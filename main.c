#include <stdio.h>

#include "common/error_handling.h"
#include "common/alloc.h"
#include "string_pool/api.h"

void builder_test_1(ScopeContext* S_SCOPE_CONTEXT) {
	String* ps2 = S_SCOPE_NEW("Hello");
	SB(final_string, {

	   const int age = 30;
	   SB_APPEND(ps2);
	   SB_APPEND_STR(" --- ");
	   SB_APPEND_STR("Name: ");
	   SB_APPEND_FORMAT("%s, ", "John");
	   SB_APPEND_STR("Age: ");
	   SB_APPEND_INT(age);

	   });

	puts(final_string->str);
	S_RELEASE(final_string);
}

void builder_test_2(ScopeContext* S_SCOPE_CONTEXT) {
	SB(abc, {
	   String* test = S_SCOPE_NEW("Test replace NEW, and the NEW");
	   String* replaced = S_SCOPE_REPLACE(test, "NEW", "");

	   if (test->length <= replaced->length)
	   EXIT_ERROR("Error in string_replace");

	   const int age = 11;
	   SB_APPEND_STR(" ");
	   SB_APPEND(replaced);
	   SB_APPEND_STR("Name: ");
	   SB_APPEND_FORMAT("%s, ", "BOB");
	   SB_APPEND_STR("Age: ");
	   SB_APPEND_FORMAT("%d", age);

	   });

	puts(abc->str);
	S_RELEASE(abc);
}

void string_cmp_test(String* ps1, String* ps2, String* ps3) {
	if (!S_CMP(ps1, ps2)) {
		printf("Error S_CMP\n");
	} else {
		printf("S_CMP: work.\n");
	}

	if (S_CMP_VA(ps3, ps2, ps1)) {
		printf("Error S_CMP_VA\n");
	} else {
		printf("STRING_CMP_VA: work.\n");
	}
}

void loops_and_multi_scope_test(ScopeContext* S_SCOPE_CONTEXT) {
	String* test = S_SCOPE_NEW("Test");
	for (int j = 0; j < 100; j++) {
		S_SCOPE({
			SB(str_test, {
				for (int i = 0; i < 100; i++) {
				char buffer[10 + 20];
				snprintf(buffer, sizeof(buffer), i % 2 == 0 ? "%d-WORLD" : "%d-HELLO", j * 100 + i);
				const String* string = S_SCOPE_NEW(buffer);
				SB_APPEND_FORMAT(
					"String: %s (ptr: %p, ref_count: %zu)\n",
					string->str,
					(void *) string->str,
					string->ref_count
				);
				}
				SB_APPEND(test);
				SB_APPEND_INT(5);
				SB_APPEND_INT(test);
				});
			puts(str_test->str);
			S_RELEASE(str_test);
			});
	}
}

bool string_find_test(String* ps1) {
	String* ps1_eq = SP_GET_STRING("Hello");

	if (!S_CMP(ps1, ps1_eq))
		return false;

	printf("STRING_FIND: work.\n");

	return true;
}

int main() {
	ScopeContext* context = scope_context_new();
	String* contextedString = scope_context_add_string(context, S_NEW("Hello"));
	puts(contextedString->str);

	String* test = S_NEW("Hello");
	String* test2 = S_NEW("Hello");
	S_RELEASE(test);
	S_RELEASE(test2);

	S_SCOPE({

		String* ps1 = S_SCOPE_NEW("Hello");
		String* ps2 = S_SCOPE_NEW("Hello");
		String* ps3 = S_SCOPE_NEW("Hell_0");

		// ===============================================
		// STRING_CMP & STRING_CMP_VA test
		string_cmp_test(ps1, ps2, ps3);

		// ===============================================
		// STRING_FIND test
		string_find_test(ps1);

		// ===============================================
		// Lots of init test
		loops_and_multi_scope_test(S_SCOPE_CONTEXT);

		// ===============================================
		// BUILDER TEST
		builder_test_1(S_SCOPE_CONTEXT);
		builder_test_2(S_SCOPE_CONTEXT);

		})

	scope_context_free(&context);

	// ===============================================
	// Release Pool and Quit
	SP_GLOBAL_FREE();
	malloc(154);
	malloc(14);
	malloc(154);
	malloc(154);
	malloc(2154);
	malloc(154);
	malloc(154);
	malloc(15545154);
	malloc(15544);
	malloc(15400);
	malloc(154);
	sp_print_memory_leaks();
	return 0;
}
