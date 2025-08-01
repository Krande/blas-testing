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

  # Find system MKL libraries for Windows
  if(WIN32)
    find_library(MKL_CORE mkl_core_dll PATHS $ENV{CONDA_PREFIX}/Library/lib)
    find_library(MKL_LP64 mkl_intel_lp64_dll PATHS $ENV{CONDA_PREFIX}/Library/lib)
    find_library(MKL_THREAD mkl_intel_thread_dll PATHS $ENV{CONDA_PREFIX}/Library/lib)
    find_library(MKL_IOMP libiomp5md PATHS $ENV{CONDA_PREFIX}/Library/lib)

    if(MKL_CORE AND MKL_LP64 AND MKL_THREAD AND MKL_IOMP)
      set(FULL_MKL_LIBS ${MKL_LP64} ${MKL_IOMP} ${MKL_THREAD} ${MKL_CORE})
      message(STATUS "Using complete MKL libraries for Fortran: ${FULL_MKL_LIBS}")
      target_link_libraries(fortran_dgemm_example PRIVATE ${FULL_MKL_LIBS})
    else()
      message(STATUS "Using default MKL libraries for Fortran")
      target_link_libraries(fortran_dgemm_example PRIVATE ${MKL_LIBRARIES})
    endif()
  else()
    target_link_libraries(fortran_dgemm_example PRIVATE ${MKL_LIBRARIES})
  endif()
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
