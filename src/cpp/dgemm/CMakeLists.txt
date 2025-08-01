cmake_minimum_required(VERSION 3.10)
project(cpp_dgemm_example CXX)

# Make sure the build type is set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Add executable
add_executable(cpp_dgemm_example dgemm_example.cpp)

# Include project src directory for cblas_helper.h
target_include_directories(cpp_dgemm_example PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Find MKL package
find_package(MKL)

# Check if we can find the MKL include directory
if(EXISTS "$ENV{CONDA_PREFIX}/include/mkl.h" OR EXISTS "$ENV{CONDA_PREFIX}/Library/include/mkl.h")
  set(MKL_HEADER_FOUND TRUE)
else()
  set(MKL_HEADER_FOUND FALSE)
  message(STATUS "MKL header not found")
endif()

# If MKL is found, use it
if(MKL_FOUND AND MKL_HEADER_FOUND)
  message(STATUS "Using MKL for BLAS: ${MKL_INCLUDE_DIRS}")
  target_include_directories(cpp_dgemm_example PRIVATE ${MKL_INCLUDE_DIRS})
  if(WIN32)
    target_include_directories(cpp_dgemm_example PRIVATE $ENV{CONDA_PREFIX}/Library/include)
  endif()

  # Find system MKL libraries
  find_library(MKL_CORE mkl_core_dll PATHS $ENV{CONDA_PREFIX}/Library/lib)
  find_library(MKL_LP64 mkl_intel_lp64_dll PATHS $ENV{CONDA_PREFIX}/Library/lib)
  find_library(MKL_THREAD mkl_intel_thread_dll PATHS $ENV{CONDA_PREFIX}/Library/lib)
  find_library(MKL_IOMP libiomp5md PATHS $ENV{CONDA_PREFIX}/Library/lib)

  # Add found libraries
  if(MKL_CORE AND MKL_LP64 AND MKL_THREAD AND MKL_IOMP)
    set(FULL_MKL_LIBS ${MKL_LP64} ${MKL_IOMP} ${MKL_THREAD} ${MKL_CORE})
    message(STATUS "Using complete MKL libraries: ${FULL_MKL_LIBS}")
    target_link_libraries(cpp_dgemm_example PRIVATE ${FULL_MKL_LIBS})
  else()
    message(STATUS "Using partial MKL libraries: ${MKL_LIBRARIES}")
    target_link_libraries(cpp_dgemm_example PRIVATE ${MKL_LIBRARIES})
    # Use system BLAS libraries as backup
    target_link_libraries(cpp_dgemm_example PRIVATE ${BLAS_LIBRARIES})
  endif()

  target_compile_definitions(cpp_dgemm_example PRIVATE HAVE_MKL)

  # Debug info
  message(STATUS "MKL_LIBRARIES: ${MKL_LIBRARIES}")
  message(STATUS "MKL_INCLUDE_DIRS: ${MKL_INCLUDE_DIRS}")
else()
  # Fallback to standard BLAS
  message(STATUS "MKL not found or headers missing, falling back to BLAS")
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
