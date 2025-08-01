cmake_minimum_required(VERSION 3.10)
project(cpp_dgemm_example CXX)

# Find MKL package
find_package(MKL REQUIRED)

# Add executable
add_executable(dgemm_example dgemm_example.cpp)

# Link against MKL libraries
target_link_libraries(dgemm_example PRIVATE ${MKL_LIBRARIES})
target_include_directories(dgemm_example PRIVATE ${MKL_INCLUDE_DIRS})

# Fallback to FindBLAS if MKL not found
if(NOT MKL_FOUND)
  find_package(BLAS REQUIRED)
  target_link_libraries(dgemm_example PRIVATE ${BLAS_LIBRARIES})
endif()

# Set compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add platform-specific options
if(WIN32)
  target_compile_definitions(dgemm_example PRIVATE _CRT_SECURE_NO_WARNINGS)
elseif(UNIX)
  target_compile_options(dgemm_example PRIVATE -O3)
endif()
