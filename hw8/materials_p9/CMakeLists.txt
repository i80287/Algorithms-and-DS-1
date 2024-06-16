cmake_minimum_required(VERSION 3.27)
project(materials_p9)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(materials_p9
#        solve/p9_generic_gen.cpp
#        solve/p9_t1.cpp
#        solve/p9_t3.cpp
#        solve/p9_t4.cpp
#        solve/p9_t5.cpp
#        config_macros.hpp
#        p9.cpp
#        p9_1.cpp
#        p9_2.cpp
#        p9_3.cpp
#        p9_4.cpp
#        p9_5.cpp
#        p9_6.cpp
#        p9_7.cpp
#        p9_8.cpp
        p9_parallel.cpp
#        p9_parallel_bfs.cpp
#        p9_parallel_matrixmove.cpp
#        template.cpp
#        test_tools.hpp
#        tmp.cpp
#        tmpdaw.cpp
)
