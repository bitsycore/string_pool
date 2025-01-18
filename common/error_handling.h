#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>

// Error Handling Macros
#define WARN(msg) do { \
fprintf(stderr, "Warning: %s\nFile: %s\nLine: %d\nFunction: %s\n", msg, __FILE__, __LINE__, __func__); \
} while (0)

#define EXIT_ERROR(msg) do { \
fprintf(stderr, "Error: %s\nFile: %s\nLine: %d\nFunction: %s\n", msg, __FILE__, __LINE__, __func__); \
exit(EXIT_FAILURE); \
} while (0)

#endif //ERROR_HANDLING_H
