cmake_minimum_required(VERSION 3.16)
project(matrix)

set(CMAKE_CXX_STANDARD 20)

add_executable(matrix
        main.cpp
        # Matrix.cpp
        # Matrix.hpp
        Vector.cpp
        Vector.hpp Matrix.cpp Matrix.hpp)