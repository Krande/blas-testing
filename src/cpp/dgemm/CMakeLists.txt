cmake_minimum_required(VERSION 3.10)
project(cpp_dgemm_example CXX)

# Make sure the build type is set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Add executable
add_executable(cpp_dgemm_example dgemm_example.cpp)

# Find MKL package
find_package(MKL)

# If MKL is found, use it
if(MKL_FOUND)
  message(STATUS "Using MKL for BLAS: ${MKL_INCLUDE_DIRS}")
  target_include_directories(cpp_dgemm_example PRIVATE ${MKL_INCLUDE_DIRS})
  target_link_libraries(cpp_dgemm_example PRIVATE ${MKL_LIBRARIES})
  target_compile_definitions(cpp_dgemm_example PRIVATE HAVE_MKL)
else()
  # Fallback to standard BLAS
  message(STATUS "MKL not found, falling back to BLAS")
  find_package(BLAS REQUIRED)
  target_link_libraries(cpp_dgemm_example PRIVATE ${BLAS_LIBRARIES})
  target_compile_definitions(cpp_dgemm_example PRIVATE HAVE_BLAS)
endif()

# Set compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add platform-specific options
if(WIN32)
  target_compile_definitions(cpp_dgemm_example PRIVATE _CRT_SECURE_NO_WARNINGS)
elseif(UNIX)
  target_compile_options(cpp_dgemm_example PRIVATE -O3)
endif()

# Output executable path information
message(STATUS "C++ DGEMM example will be at: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cpp_dgemm_example${CMAKE_EXECUTABLE_SUFFIX}")
