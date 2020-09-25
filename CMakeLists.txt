cmake_minimum_required(VERSION 3.17)
project(matrix)

set(CMAKE_CXX_STANDARD 20)

add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

add_executable(matrix
        Vector.hpp
        Matrix.hpp
        main.cpp
        Vec.hpp MatrixHelperFunctions.hpp VectorHelperFunctions.hpp Interfaces.hpp StaticPoly.hpp)

target_link_libraries(
        matrix
        PRIVATE
        project_warnings
        project_options
)