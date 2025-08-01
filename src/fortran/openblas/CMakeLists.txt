cmake_minimum_required(VERSION 3.10)
project(fortran_openblas_example Fortran)

# Make sure the build type is set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Add executable
add_executable(fortran_openblas_example dgemm_example.f90)

# Find OpenBLAS package or standard BLAS
find_package(BLAS REQUIRED)
target_link_libraries(fortran_openblas_example PRIVATE ${BLAS_LIBRARIES})
target_compile_definitions(fortran_openblas_example PRIVATE HAVE_OPENBLAS)

# Set compiler flags based on compiler type
if(CMAKE_Fortran_COMPILER_ID MATCHES "GNU")
  target_compile_options(fortran_openblas_example PRIVATE -O3)
elseif(CMAKE_Fortran_COMPILER_ID MATCHES "Intel")
  target_compile_options(fortran_openblas_example PRIVATE -O3)
elseif(CMAKE_Fortran_COMPILER_ID MATCHES "Flang|LLVMFlang")
  # Add specific flags for Flang if needed
endif()

# Output executable path information
message(STATUS "Fortran OpenBLAS DGEMM example will be at: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fortran_openblas_example${CMAKE_EXECUTABLE_SUFFIX}")
