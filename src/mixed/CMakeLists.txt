cmake_minimum_required(VERSION 3.10)
project(mixed_dgemm_example CXX Fortran)

# Make sure the build type is set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Disable CMake's automatic setup of debug flags when in Debug mode
# This prevents incompatible flags like /RTC1 and /O2 on MSVC
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_DEBUG "")

# Find MKL or standard BLAS package
find_package(MKL)
if(NOT MKL_FOUND)
  find_package(BLAS REQUIRED)
endif()

# Create the Fortran wrapper library
add_library(fortran_dgemm_wrapper STATIC fortran_wrapper.f90)
set_target_properties(fortran_dgemm_wrapper PROPERTIES 
  POSITION_INDEPENDENT_CODE ON
  Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Link against BLAS libraries
if(MKL_FOUND)
  # Find system MKL libraries for Windows
  if(WIN32)
    find_library(MKL_CORE mkl_core_dll PATHS $ENV{CONDA_PREFIX}/Library/lib)
    find_library(MKL_LP64 mkl_intel_lp64_dll PATHS $ENV{CONDA_PREFIX}/Library/lib)
    find_library(MKL_THREAD mkl_intel_thread_dll PATHS $ENV{CONDA_PREFIX}/Library/lib)
    find_library(MKL_IOMP libiomp5md PATHS $ENV{CONDA_PREFIX}/Library/lib)

    if(MKL_CORE AND MKL_LP64 AND MKL_THREAD AND MKL_IOMP)
      set(FULL_MKL_LIBS ${MKL_LP64} ${MKL_IOMP} ${MKL_THREAD} ${MKL_CORE})
      message(STATUS "Using complete MKL libraries for mixed example: ${FULL_MKL_LIBS}")
      target_link_libraries(fortran_dgemm_wrapper PRIVATE ${FULL_MKL_LIBS})
    else()
      message(STATUS "Using default MKL libraries for mixed example")
      target_link_libraries(fortran_dgemm_wrapper PRIVATE ${MKL_LIBRARIES})
    endif()
  else()
    target_link_libraries(fortran_dgemm_wrapper PRIVATE ${MKL_LIBRARIES})
  endif()
  target_compile_definitions(fortran_dgemm_wrapper PRIVATE HAVE_MKL)
else()
  # Fallback to standard BLAS
  target_link_libraries(fortran_dgemm_wrapper PRIVATE ${BLAS_LIBRARIES})
endif()

# Add the C++ executable
add_executable(mixed_dgemm_example mixed_example.cpp)

# Include direct_dgemm.h
target_include_directories(mixed_dgemm_example PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link against the Fortran wrapper library
target_link_libraries(mixed_dgemm_example PRIVATE fortran_dgemm_wrapper)

# If Fortran requires any specific runtime libraries, link those too
if(CMAKE_Fortran_COMPILER_ID MATCHES "GNU")
  if(UNIX)
    target_link_libraries(mixed_dgemm_example PRIVATE gfortran)
  endif()
elseif(CMAKE_Fortran_COMPILER_ID MATCHES "Intel")
  if(UNIX)
    target_link_libraries(mixed_dgemm_example PRIVATE ifcore)
  endif()
endif()

# Set compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(mixed_dgemm_example PRIVATE -O3)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(mixed_dgemm_example PRIVATE /O2)
endif()

if(CMAKE_Fortran_COMPILER_ID MATCHES "GNU")
  target_compile_options(fortran_dgemm_wrapper PRIVATE -O3)
elseif(CMAKE_Fortran_COMPILER_ID MATCHES "Intel")
  target_compile_options(fortran_dgemm_wrapper PRIVATE -O3)
endif()

# Output executable path information
message(STATUS "Mixed C++/Fortran DGEMM example will be at: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mixed_dgemm_example${CMAKE_EXECUTABLE_SUFFIX}")
