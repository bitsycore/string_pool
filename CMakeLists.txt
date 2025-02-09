cmake_minimum_required(VERSION 3.20)
project(testStringPool C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ==============================================================================
# MARK: Test Project
# ==============================================================================

add_library(
        StringPool
        string_pool/string_pool.h
        string_pool/string_pool.c
        string_pool/string_pool_types.h
        string_pool/api.h
        string_pool/string_builder.c
        string_pool/string_builder.h
        string_pool/global_pool.c
        string_pool/global_pool.h
        string_pool/string.c
        string_pool/string.h
        string_pool/config.h
        common/error_handling.h
        common/hash.c
        common/hash.h
        common/maths.h
        string_pool/scope_context.c
        string_pool/scope_context.h
        string_pool/string_page.c
        string_pool/string_page.h
        common/memory_leak.h
        common/memory_leak.c
        common/debug.c
        common/debug.h
        common/arena.c
        common/arena.h
        common/alloc.c
        common/alloc.h
)

if (WIN32)
    # Dbghelp.lib for call stack
    target_link_libraries(StringPool dbghelp)
elseif (UNIX)
    target_link_libraries(StringPool m)
#    find_library(DL_LIBRARY dl)
#    if(DL_LIBRARY)
#        target_link_libraries(StringPool ${DL_LIBRARY})
#    else()
#        message(WARNING "dl library not found.")
#    endif()
endif ()

# ==============================================================================
# MARK: Test Project
# ==============================================================================

add_executable(testStringPool main.c
)
target_link_libraries(testStringPool StringPool)