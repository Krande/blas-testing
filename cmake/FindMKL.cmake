# Find the MKL libraries
#
# Options:
#
# This module defines the following variables:
#
#   MKL_FOUND            - True if MKL is found
#   MKL_INCLUDE_DIRS     - Directory where MKL headers are located
#   MKL_LIBRARIES        - MKL libraries to link against

# Determine MKL_ROOT_DIR based on environment variables
if(NOT DEFINED MKL_ROOT_DIR)
  if(DEFINED ENV{MKLROOT})
    set(MKL_ROOT_DIR $ENV{MKLROOT})
  elseif(DEFINED ENV{CONDA_PREFIX})
    set(MKL_ROOT_DIR $ENV{CONDA_PREFIX})
  endif()
endif()

# Find include directory
find_path(MKL_INCLUDE_DIR
  NAMES mkl.h
  HINTS ${MKL_ROOT_DIR}/include
        $ENV{CONDA_PREFIX}/include
  DOC "MKL include directory")

# Libraries to search for
set(_mkl_libs mkl_intel_lp64 mkl_intel_thread mkl_core iomp5 pthread m dl)

# MKL is composed of several libraries
set(MKL_LIBRARIES "")
foreach(_lib ${_mkl_libs})
  find_library(_found
    NAMES ${_lib}
    HINTS ${MKL_ROOT_DIR}/lib
          ${MKL_ROOT_DIR}/lib/intel64
          $ENV{CONDA_PREFIX}/lib
    DOC "MKL ${_lib} library")
  if(_found)
    list(APPEND MKL_LIBRARIES ${_found})
  endif()
  unset(_found CACHE)
endforeach()

# Handle the QUIETLY and REQUIRED arguments and set MKL_FOUND
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MKL DEFAULT_MSG MKL_INCLUDE_DIR MKL_LIBRARIES)

# Copy the results to the output variables
if(MKL_FOUND)
  set(MKL_INCLUDE_DIRS ${MKL_INCLUDE_DIR})
endif()

mark_as_advanced(MKL_INCLUDE_DIR MKL_LIBRARIES)
