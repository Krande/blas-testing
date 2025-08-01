# - Find BLAS library
# This module finds an installed BLAS library
#
# This module sets the following variables:
#  BLAS_FOUND - set to true if a library implementing the BLAS interface is found
#  BLAS_LIBRARIES - list of libraries to link against to use BLAS

# First try to use MKL if available
find_package(MKL QUIET)
if(MKL_FOUND)
  set(BLAS_FOUND TRUE)
  set(BLAS_LIBRARIES ${MKL_LIBRARIES})
  return()
endif()

# Standard BLAS search
if(BLAS_LIBRARIES)
  set(BLAS_FOUND TRUE)
  return()
endif()

# Check for BLAS in conda environment
find_library(BLAS_LIBRARY
  NAMES blas cblas openblas
  HINTS $ENV{CONDA_PREFIX}/lib $ENV{CONDA_PREFIX}/Library/lib
  DOC "BLAS library")

if(BLAS_LIBRARY)
  set(BLAS_LIBRARIES ${BLAS_LIBRARY})
  set(BLAS_FOUND TRUE)
else()
  set(BLAS_FOUND FALSE)
endif()

# Handle the QUIETLY and REQUIRED arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BLAS DEFAULT_MSG BLAS_LIBRARIES)

mark_as_advanced(BLAS_LIBRARY)
