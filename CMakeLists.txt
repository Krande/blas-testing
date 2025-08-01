cmake_minimum_required(VERSION 3.10)
project(blas_testing LANGUAGES CXX Fortran)

# Set CMAKE_PREFIX_PATH to include MKL directory
list(APPEND CMAKE_PREFIX_PATH $ENV{CONDA_PREFIX})

# Create MKL finder module
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Add subdirectories
add_subdirectory(src/cpp/dgemm)
add_subdirectory(src/fortran/dgemm)
