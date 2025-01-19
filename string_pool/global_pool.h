#ifndef GLOBAL_POOL_H
#define GLOBAL_POOL_H

#include "string_pool_types.h"

/**
 * Initialize the global string pool
 */
void init_global_pool();

/**
 * Deinitialize the global string pool
 */
void deinit_global_pool();

/**
 * Get the global string pool singleton and initialize it if it is not yet initialized
 * @return Pointer to the global string pool
 */
StringPool *get_global_pool_singleton();

/**
 * Get the global string pool without initializing it
 * @return Pointer to the global string pool
 */
StringPool *get_global_pool();

#endif //GLOBAL_POOL_H
