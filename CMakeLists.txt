cmake_minimum_required(VERSION 3.18)
project(main)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++17 -Wno-multichar")

add_executable(main main.cpp)
add_executable(test test.cpp)

include_directories(include)
target_include_directories(test PUBLIC include-test)
