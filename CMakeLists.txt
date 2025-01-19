cmake_minimum_required(VERSION 3.30)
project(testStringPool C)

set(CMAKE_C_STANDARD 99)

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
)

add_executable(testStringPool main.c)

target_link_libraries(testStringPool StringPool)