cmake_minimum_required(VERSION 3.29)
project(ThreadPool C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

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

