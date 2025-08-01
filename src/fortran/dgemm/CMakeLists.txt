cmake_minimum_required(VERSION 3.10)
project(fortran_dgemm_example Fortran)

# Find MKL package
find_package(MKL REQUIRED)

# Add executable
add_executable(dgemm_example dgemm_example.f90)

# Link against MKL libraries
target_link_libraries(dgemm_example PRIVATE ${MKL_LIBRARIES})

# Fallback to FindBLAS if MKL not found
if(NOT MKL_FOUND)
  find_package(BLAS REQUIRED)
  target_link_libraries(dgemm_example PRIVATE ${BLAS_LIBRARIES})
endif()

# Set compiler flags
if(CMAKE_Fortran_COMPILER_ID MATCHES "GNU")
  target_compile_options(dgemm_example PRIVATE -O3)
elseif(CMAKE_Fortran_COMPILER_ID MATCHES "Intel")
  target_compile_options(dgemm_example PRIVATE -O3)
endif()
