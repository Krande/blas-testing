# Find the CBLAS library
#
# This module defines
#  CBLAS_FOUND             - True if CBLAS was found
#  CBLAS_INCLUDE_DIRS      - include directories for CBLAS
#  CBLAS_LIBRARIES         - libraries for CBLAS

# First try to use MKL if available
find_package(MKL QUIET)
if(MKL_FOUND)
  set(CBLAS_FOUND TRUE)
  set(CBLAS_LIBRARIES ${MKL_LIBRARIES})
  set(CBLAS_INCLUDE_DIRS ${MKL_INCLUDE_DIRS})
  return()
endif()

# Find CBLAS include directory
find_path(CBLAS_INCLUDE_DIR
  NAMES cblas.h
  HINTS $ENV{CONDA_PREFIX}/include $ENV{CONDA_PREFIX}/Library/include
  DOC "CBLAS include directory")

# Find CBLAS library
find_library(CBLAS_LIBRARY
  NAMES cblas openblas
  HINTS $ENV{CONDA_PREFIX}/lib $ENV{CONDA_PREFIX}/Library/lib
  DOC "CBLAS library")

# Set CBLAS_LIBRARIES and CBLAS_INCLUDE_DIRS
if(CBLAS_LIBRARY AND CBLAS_INCLUDE_DIR)
  set(CBLAS_LIBRARIES ${CBLAS_LIBRARY})
  set(CBLAS_INCLUDE_DIRS ${CBLAS_INCLUDE_DIR})
  set(CBLAS_FOUND TRUE)
else()
  set(CBLAS_FOUND FALSE)
endif()

# Handle the QUIETLY and REQUIRED arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CBLAS DEFAULT_MSG CBLAS_LIBRARIES CBLAS_INCLUDE_DIRS)

mark_as_advanced(CBLAS_INCLUDE_DIR CBLAS_LIBRARY)
