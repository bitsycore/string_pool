cmake_minimum_required(VERSION 3.30)
project(testStringPool C)

set(CMAKE_C_STANDARD 99)

add_executable(testStringPool main.c
        string_pool/string_pool.h
        string_pool/string_pool.c
        string_pool/string_pool_api.h
        common/error_handling.h)
