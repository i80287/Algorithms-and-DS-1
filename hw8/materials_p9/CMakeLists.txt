cmake_minimum_required(VERSION 3.27)
project(materials_p9 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(materials_p9
        #        p9_dp.cpp
        #        p9_parallel.cpp
        #        p9_parallel_matrix_move.cpp
        solve/p9_generic_gen.cpp
        p9_parallel_matrix_move.cpp
)

# Empty by default
set(TEST_COMPILE_OPTIONS)
set(TEST_COMPILE_DEFINITIONS __VSCODE_LOCALGHOST__=1)

#set(TEST_COMPILE_OPTIONS ${TEST_COMPILE_OPTIONS} -UNDEBUG)
set(TEST_COMPILE_OPTIONS ${TEST_COMPILE_OPTIONS}
        #                -fsanitize=address,undefined
        #        -fno-omit-frame-pointer
        #        -fstack-protector-all
        #        -mshstk
        -g3
        #        -DNDEBUG
        #        -Ofast
        -march=native
        -Wall
        -Wextra
        -Wcast-qual
        -Wpedantic
        -Wshift-overflow
        -Wunused
        -Wconversion
        -Wshadow
        -Wnull-dereference
        -Wundef
        -Wwrite-strings
        -Wsign-conversion
        -Wmissing-noreturn
        -Wunreachable-code
        -Wcast-align
        -Warray-bounds
        -pedantic-errors
        -fsized-deallocation
)

set(TEST_COMPILE_DEFINITIONS ${TEST_COMPILE_DEFINITIONS}
        __VSCODE_LOCALGHOST__=1
        #        _LIBCPP_ENABLE_ASSERTIONS=1
        #        _GLIBCXX_DEBUG
        #        _GLIBCXX_DEBUG_PEDANTIC
        #        _GLIBCXX_CONCEPT_CHECKS
)

target_compile_options(materials_p9 PRIVATE ${TEST_COMPILE_OPTIONS})
target_compile_definitions(materials_p9 PRIVATE ${TEST_COMPILE_DEFINITIONS})
#target_link_options(materials_p9 PRIVATE -lstdc++_libbacktrace -ltbb -fsanitize=address,undefined) # -Wl,-z,stack-size=268435456)
