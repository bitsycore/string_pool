cmake_minimum_required(VERSION 3.30)
project(testStringPool C)

set(CMAKE_C_STANDARD 99)

add_executable(
        testStringPool main.c
        string_pool/string_pool.h
        string_pool/string_pool.c
        string_pool/api.h
        common/error_handling.h
        string_pool/string_pool_types.h
        common/hash.c
        common/hash.h
        common/maths.h
        string_pool/string_builder.c
        string_pool/string_builder.h
        string_pool/global_pool.c
        string_pool/global_pool.h
        string_pool/string.c
        string_pool/string.h
)
