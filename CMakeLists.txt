cmake_minimum_required(VERSION 3.29)
project(ThreadPool C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

#set(CMAKE_BUILD_TYPE "Release")

set_source_files_properties(utils.c PROPERTIES COMPILE_OPTIONS -Wunused-value)
set_source_files_properties(utils.c PROPERTIES COMPILE_OPTIONS -Waddress)


include_directories(.)

add_executable(ThreadPool
        main.c
        pool.c
        pool.h
        utils.hpp
        sort.hpp)

add_executable(FuncTest
        test1.c
        sort.hpp
)

