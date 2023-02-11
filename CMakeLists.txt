cmake_minimum_required(VERSION 3.10)
project(main C CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_executable(main main.cpp)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL 1.0.0 REQUIRED)

target_link_libraries(main
        cpprest
        -lstdc++fs
        Boost::boost
        Boost::system
        OpenSSL::SSL)
