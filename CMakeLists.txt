cmake_minimum_required(VERSION 3.10)
project(blas_testing LANGUAGES CXX Fortran)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Set CMAKE_PREFIX_PATH to include MKL directory
list(APPEND CMAKE_PREFIX_PATH $ENV{CONDA_PREFIX})

# Create MKL finder module
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Display build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Fortran compiler: ${CMAKE_Fortran_COMPILER_ID} ${CMAKE_Fortran_COMPILER_VERSION}")
message(STATUS "Conda prefix: $ENV{CONDA_PREFIX}")

# Set output directories for all targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add subdirectories
add_subdirectory(src/cpp/dgemm)
add_subdirectory(src/fortran/dgemm)

# Print completion message
message(STATUS "Configuration complete. Run 'cmake --build ${CMAKE_BINARY_DIR}' to build the project.")
