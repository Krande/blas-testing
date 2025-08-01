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

# Check for BLAS/LAPACK as fallback
find_package(BLAS)
find_package(LAPACK)
if(BLAS_FOUND)
  message(STATUS "Found BLAS: ${BLAS_LIBRARIES}")
endif()
if(LAPACK_FOUND)
  message(STATUS "Found LAPACK: ${LAPACK_LIBRARIES}")
endif()

# Add Mixed C++/Fortran example
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/mixed)
  add_subdirectory(src/mixed)
else()
  message(STATUS "Creating Mixed C++/Fortran example directory")
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/mixed)
  # Add the directory now that it exists
  add_subdirectory(src/mixed)
endif()

# Add subdirectories
add_subdirectory(src/cpp/dgemm)
add_subdirectory(src/fortran/dgemm)

# Add OpenBLAS C++ example
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/openblas)
  add_subdirectory(src/cpp/openblas)
else()
  message(STATUS "Creating C++ OpenBLAS example directory")
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/openblas)
  # Add the directory now that it exists
  add_subdirectory(src/cpp/openblas)
endif()

# Add OpenBLAS Fortran example
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/fortran/openblas)
  add_subdirectory(src/fortran/openblas)
else()
  message(STATUS "Creating Fortran OpenBLAS example directory")
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/fortran/openblas)
  # Add the directory now that it exists
  add_subdirectory(src/fortran/openblas)
  # Try to create the OpenBLAS example directory
  message(STATUS "OpenBLAS example directory not found, creating one from dgemm example")
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/openblas)

  # Copy C++ DGEMM example files to OpenBLAS directory
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/dgemm/dgemm_example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/openblas/dgemm_example.cpp
    COPYONLY
  )

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/dgemm/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/openblas/CMakeLists.txt
    COPYONLY
  )

  # Now add the OpenBLAS directory
  add_subdirectory(src/cpp/openblas)
endif()

# Print completion message
message(STATUS "Configuration complete. Run 'cmake --build ${CMAKE_BINARY_DIR}' to build the project.")
