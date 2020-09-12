cmake_minimum_required(VERSION 3.17)
project(matrix)

set(CMAKE_CXX_STANDARD 20)

add_executable(matrix
        Vector.hpp
        Matrix.hpp
        main.cpp
        Vec.hpp MatrixHelperFunctions.hpp VectorHelperFunctions.hpp Interfaces.hpp StaticPoly.hpp)
