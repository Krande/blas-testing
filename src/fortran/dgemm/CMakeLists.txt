cmake_minimum_required(VERSION 3.10)
project(fortran_dgemm_example Fortran)

# Make sure the build type is set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Add executable
add_executable(fortran_dgemm_example dgemm_example.f90)

# Find MKL package
find_package(MKL)

# If MKL is found, use it
if(MKL_FOUND)
  message(STATUS "Using MKL for BLAS: ${MKL_INCLUDE_DIRS}")
  target_link_libraries(fortran_dgemm_example PRIVATE ${MKL_LIBRARIES})
else()
  # Fallback to standard BLAS
  message(STATUS "MKL not found, falling back to BLAS")
  find_package(BLAS REQUIRED)
  target_link_libraries(fortran_dgemm_example PRIVATE ${BLAS_LIBRARIES})
endif()

# Set compiler flags based on compiler type
if(CMAKE_Fortran_COMPILER_ID MATCHES "GNU")
  target_compile_options(fortran_dgemm_example PRIVATE -O3)
elseif(CMAKE_Fortran_COMPILER_ID MATCHES "Intel")
  target_compile_options(fortran_dgemm_example PRIVATE -O3)
elseif(CMAKE_Fortran_COMPILER_ID MATCHES "Flang|LLVMFlang")
  # Add specific flags for Flang if needed
  # target_compile_options(fortran_dgemm_example PRIVATE ...)
endif()

# Output executable path information
message(STATUS "Fortran DGEMM example will be at: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fortran_dgemm_example${CMAKE_EXECUTABLE_SUFFIX}")
