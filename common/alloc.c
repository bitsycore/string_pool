#if ENABLE_LEAK_DETECTOR == true

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct AllocationInfo {
    void* address;
    size_t size;
    const char* file;
    int line;
    struct AllocationInfo* next;
} AllocationInfo;

static AllocationInfo* allocation_list = NULL;

static void add_allocation(void* address, const size_t size, const char* file, const int line) {
    AllocationInfo* new_allocation = malloc(sizeof(AllocationInfo));
    if (new_allocation == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for allocation tracking!\n");
        return;
    }
    new_allocation->address = address;
    new_allocation->size = size;
    new_allocation->file = file;
    new_allocation->line = line;
    new_allocation->next = allocation_list;
    allocation_list = new_allocation;
}

// Function to remove allocation information from the list
static void remove_allocation(const void* address) {
    AllocationInfo* current = allocation_list;
    AllocationInfo* previous = NULL;

    while (current != NULL) {
        if (current->address == address) {
            if (previous == NULL) {
                allocation_list = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}


void* imp_sp_malloc(const size_t size, const char* file, const int line) {
    void* ptr = malloc(size);
    if (ptr != NULL) {
        add_allocation(ptr, size, file, line);
    }
    return ptr;
}


void* imp_sp_calloc(const size_t num, const size_t size, const char* file, const int line) {
    void* ptr = calloc(num, size);
    if (ptr != NULL) {
        const size_t total_size = num * size;
        add_allocation(ptr, total_size, file, line);
    }
    return ptr;
}

void* imp_sp_realloc(void* ptr, const size_t size, const char* file, const int line) {
    void* new_ptr = realloc(ptr, size);
    if (new_ptr != NULL) {
        if (ptr != NULL) {
            remove_allocation(ptr); // Remove old allocation information
        }

        add_allocation(new_ptr, size, file, line);
    }
    return new_ptr;
}

void imp_sp_free(void* ptr) {
    if (ptr != NULL) {
         const AllocationInfo* current = allocation_list;
         while (current != NULL) {
             if (current->address == ptr) {
                 break;
             }
             current = current->next;
         }
        remove_allocation(ptr);
        free(ptr);

    }
}

char* imp_sp_strdup(const char* s, const char* file, const int line) {
    if (s == NULL) return NULL;
    const size_t len = strlen(s) + 1;
    char* new_str = imp_sp_malloc(len, file, line);
    if (new_str != NULL) {
        memcpy(new_str, s, len);
    }
    return new_str;
}

void sp_print_memory_leaks() {
    const AllocationInfo* current = allocation_list;

    if (current == NULL) {
        return;
    }

    size_t total_leaked_memory = 0;
    size_t number_leaked_block = 0;
    printf("\n--------------------- [MEMORY LEAK DETECTED] ---------------------\n\n");
    while (current != NULL) {
        if (current->size >= 1024 * 1024 * 2.5) {
            printf("[%llu]\t0x%p\t%.2f MB\t%s:%d\n",
                   number_leaked_block, current->address,
                   (double) current->size / (1024 * 1024),
                   current->file, current->line);
        } else if (current->size >= 1024 * 2.5) {
            printf("[%llu]\t0x%p\t%.2f KB\t%s:%d\n",
                   number_leaked_block, current->address,
                   (double) current->size / 1024,
                   current->file, current->line);
        } else {
            printf("[%llu]\t0x%p\t%zu Bytes\t%s:%d\n",
                   number_leaked_block, current->address,
                   current->size,
                   current->file, current->line);
        }
        total_leaked_memory += current->size;
        current = current->next;
        number_leaked_block++;
    }
    printf("\n------------------------------------------------------------------\nBlocks: %llu\t%zu bytes (%.2f KB, %.2f MB)\n------------------------------------------------------------------\n",
           number_leaked_block, total_leaked_memory, (double) total_leaked_memory / 1024.0,
           (double) total_leaked_memory / (1024.0 * 1024.0));
}

void sp_cleanup_memory_tracking() {
    AllocationInfo* current = allocation_list;

    while (current != NULL) {
        AllocationInfo* next = current->next;
        free(current);
        current = next;
    }

    allocation_list = NULL;
}

#endif