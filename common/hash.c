#include "hash.h"

size_t hash(const char *str, const size_t length) {
    size_t hash = 5381;
    int c;

    while ((c = (int) *str++)) {
        hash = (hash << 5) + hash + c; // hash * 33 + c
    }
    return hash % length;
}