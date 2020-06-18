cmake_minimum_required(VERSION 3.16)
project(matrix)

set(CMAKE_CXX_STANDARD 20)

add_executable(matrix
        Vector.hpp
        Matrix.hpp
        main.cpp)
