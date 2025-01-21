#include "debug.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =====================================================================================================================
#ifdef _WIN32 // MARK: WINDOWS
// =====================================================================================================================

// =====================================================================================================================
#ifdef _MSC_VER // MARK: Visual Studio
// =====================================================================================================================

#include <windows.h>
#include <dbghelp.h>

char* print_call_stack() {
	void* stack[100];
	void* process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);

	const unsigned short frames = CaptureStackBackTrace(1, 100, stack, NULL);
	SYMBOL_INFO* symbol = calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	// Allocate initial buffer
	size_t buffer_size = 1024;
	char* buffer = malloc(buffer_size);
	if (!buffer) {
		free(symbol);
		return NULL;
	}
	buffer[0] = '\0';

	for (unsigned int i = 0; i < frames; i++) {
		SymFromAddr(process, (DWORD64) (stack[i]), 0, symbol);
		size_t needed = snprintf(NULL, 0, "  %i: %s - 0x%0X\n", frames - i - 1, symbol->Name,
		                         (unsigned long) symbol->Address) + 1;
		if (strlen(buffer) + needed >= buffer_size) {
			buffer_size *= 2;
			char* temp = realloc(buffer, buffer_size);
			if (!temp) {
				free(symbol);
				free(buffer);
				return NULL;
			}
			buffer = temp;
		}
		snprintf(buffer + strlen(buffer), buffer_size - strlen(buffer), "  %i: %s - 0x%0X\n", frames - i - 1,
		         symbol->Name, (unsigned long) symbol->Address);
	}

	free(symbol);
	return buffer;
}

// =====================================================================================================================
#else // MARK: MinGW
// =====================================================================================================================

char* print_call_stack() {
	const char msg[] = "  Unavailable on MinGW\n";
	char* heap_msg = malloc(strlen(msg) + 1);
	if (!heap_msg) {
		return NULL;
	}
	return strcpy(heap_msg, msg);
}

// =====================================================================================================================
#endif
// =====================================================================================================================

// =====================================================================================================================
#else // MARK: UNIX
// =====================================================================================================================

#include <execinfo.h>
#include <unistd.h>

char* print_call_stack() {
	void* buffer[100];
	int nptrs = backtrace(buffer, 100);
	char** symbols = backtrace_symbols(buffer, nptrs);
	if (symbols == NULL) {
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}

	// Allocate initial buffer
	size_t buffer_size = 1024;
	char* result = malloc(buffer_size);
	if (!result) {
		free(symbols);
		return NULL;
	}
	result[0] = '\0';

	for (int i = 0; i < nptrs; i++) {
		size_t needed = snprintf(NULL, 0, "%s\n", symbols[i]) + 1;
		if (strlen(result) + needed >= buffer_size) {
			buffer_size *= 2;
			char* temp = realloc(result, buffer_size);
			if (!temp) {
				free(symbols);
				free(result);
				return NULL;
			}
			result = temp;
		}
		snprintf(result + strlen(result), buffer_size - strlen(result), "%s\n", symbols[i]);

		char command[256];
		snprintf(command, sizeof(command), "addr2line -e %s %p", program_invocation_name, buffer[i]);
		FILE* fp = popen(command, "r");
		if (fp) {
			char addr2line_output[256];
			while (fgets(addr2line_output, sizeof(addr2line_output), fp)) {
				needed = strlen(addr2line_output) + 1;
				if (strlen(result) + needed >= buffer_size) {
					buffer_size *= 2;
					char* temp = realloc(result, buffer_size);
					if (!temp) {
						free(symbols);
						free(result);
						pclose(fp);
						return NULL;
					}
					result = temp;
				}
				strcat(result, addr2line_output);
			}
			pclose(fp);
		}
	}

	free(symbols);
	return result;
}

// =====================================================================================================================
#endif
// =====================================================================================================================
