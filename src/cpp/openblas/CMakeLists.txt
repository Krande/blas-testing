cmake_minimum_required(VERSION 3.10)
project(cpp_openblas_example CXX)

# Make sure the build type is set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Add executable
add_executable(cpp_openblas_example dgemm_example.cpp)

# Find BLAS package
find_package(BLAS REQUIRED)

# Link against BLAS libraries
target_link_libraries(cpp_openblas_example PRIVATE ${BLAS_LIBRARIES})
target_compile_definitions(cpp_openblas_example PRIVATE HAVE_BLAS)

# Set compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add platform-specific options
if(WIN32)
  target_compile_definitions(cpp_openblas_example PRIVATE _CRT_SECURE_NO_WARNINGS)
elseif(UNIX)
  target_compile_options(cpp_openblas_example PRIVATE -O3)
endif()

# Output executable path information
message(STATUS "C++ OpenBLAS DGEMM example will be at: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cpp_openblas_example${CMAKE_EXECUTABLE_SUFFIX}")
