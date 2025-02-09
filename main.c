#include "common/alloc.h"
#include "common/memory_leak.h"

#include <stdio.h>

#include "common/error_handling.h"
#include "string_pool/api.h"

void test_builder_scoped(ScopeContext* S_SCOPE_CTX) {
	SB_SCOPED(final_string) {
		SB_APPEND_STR("Hello");
		SB_APPEND_STR(" ");
		SB_APPEND_STR("World");
		SB_APPEND_STR(" ");
		SB_APPEND_STR("Hello");
	}
	puts(final_string->str);
}

void test_builder_scoped2(ScopeContext* S_SCOPE_CTX) {
	SB_SCOPED(final_string) {
		SB_APPEND_STR("Hello");
		SB_APPEND_INT(5000);
		SB_APPEND_STR("World 5000");
	}

	SB_SCOPED(final_string2) {
		SB_APPEND_STR("Are you ok ?");
		SB_APPEND_STR(" ");
		SB_APPEND_STR("World 5000 -- ");
		SB_APPEND_FORMAT("%x", 125);
	}

	puts(final_string->str);
	puts(final_string2->str);
}

void test_builder(ScopeContext* S_SCOPE_CTX) {
	SB(abc) {
		const String* test = S_SCOPE_NEW("Test replace NEW, and the NEW");
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
	}

	puts(abc->str);
	S_RELEASE(abc);
}

void test_string_cmp(const String* ps1, const String* ps2, const String* ps3) {
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

void test_loops_and_multi_scope(ScopeContext* S_SCOPE_CTX) {
	String* test = S_SCOPE_NEW("Test");

	for (int j = 0; j < 10; j++) {
		S_SCOPE() {
			SB_SCOPED(str_test) {
				for (int i = 0; i < 1000; i++) {
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
			}
			printf("String builder built %zu chars\n", str_test->length);
		}
	}
}

void test_string_find(const String* ps1) {
	const String* ps1_eq = SP_GET_STRING("Hello");

	if (!S_CMP(ps1, ps1_eq))
		return;

	printf("STRING_FIND: work.\n");
}

void test_s_new(void) {
	String* test = S_NEW("Hello");
	String* test2 = S_NEW("Hello");
	S_RELEASE(test);
	S_RELEASE(test2);
}

ScopeContext* test_scope_manual() {
	ScopeContext* context = scope_context_new();
	const String* string_in_context = scope_context_add_string(context, S_NEW("Hello"));
	const String* string_in_context2 = scope_context_add_string(context, S_NEW("Hello World"));
	puts(string_in_context->str);
	puts(string_in_context2->str);
	return context;
}

void test_end_scope_manual(ScopeContext** context) {
    scope_context_free(context);
}

void* imp_malloc(size_t size) {
	return ml_malloc(size);
}

void imp_free(void* ptr) {
	ml_free(ptr);
}

int main() {
	Arena* arena = arena_new_custom(sizeof(String)*2, imp_malloc, imp_free, false);
	for (int i = 0; i < 100; i++) {
		String* string = arena_alloc(arena, sizeof(String));
		printf("String: %p\n", (void *) string);
	}
	arena_destroy(&arena);
	SB(StringResult){
		SB_APPEND_STR("HELLO WORLD");
		SB_APPEND_STR("----------------------");
		SB_APPEND_STR("HELLO WORLD");
		SB_APPEND_STR("----------------------");
		SB_APPEND_STR("HELLO WORLD");
		SB_APPEND_STR("HELLO WORLD");
		SB_APPEND_STR("----------------------");
		SB_APPEND_STR("HELLO WORLD");
		SB_APPEND_STR("----------------------");
		SB_APPEND_STR("HELLO WORLD");
		SB_APPEND_STR("----------------------");
		SB_APPEND_STR("HELLO WORLD");
		SB_APPEND_STR("HELLO WORLD");
	}

	puts(StringResult->str);
	S_RELEASE(StringResult);

	S_SCOPE() {
		const String* test1 = S_SCOPE_NEW("Hello");
		const String* test2 = S_SCOPE_NEW("Hello World");
		puts(test1->str);
		puts(test2->str);
	}

	ScopeContext* scope_ctx = test_scope_manual();

	test_s_new();

	String* ps1 = S_NEW("Hello");
	String* ps2 = S_NEW("Hello");
	String* ps3 = S_NEW("Hell_0");

	// ===============================================
	// STRING_CMP & STRING_CMP_VA test
	test_string_cmp(ps1, ps2, ps3);

	// ===============================================
	// STRING_FIND test
	test_string_find(ps1);

	S_SCOPE() {
		// ===============================================
		// Lots of init test
		test_loops_and_multi_scope(S_SCOPE_CTX);

		// ===============================================
		// BUILDER TEST
		test_builder(S_SCOPE_CTX);
		test_builder_scoped2(S_SCOPE_CTX);
		test_builder_scoped(S_SCOPE_CTX);
	}

	S_RELEASE(ps1);
	S_RELEASE(ps2);
	S_RELEASE(ps3);

	test_end_scope_manual(&scope_ctx);

	// ===============================================
	// Release Pool and Quit
	SP_GLOBAL_FREE();
	ml_print_memory_leaks();
	arena_global_destroy();
	return 0;
}
