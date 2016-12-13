# - Find Intel MKL
# Find the MKL libraries
#
# Options:
#
# MKL_STATAIC : use static linking
# MKL_MULTI_THREADED: use multi-threading
# MKL_SDL : Single Dynamic Library interface
#
# This module defines the following variables: # # MKL_FOUND : True if MKL_INCLUDE_DIR are found
# MKL_INCLUDE_DIR : where to find mkl.h, etc.
# MKL_INCLUDE_DIRS : set when MKL_INCLUDE_DIR found
# MKL_LIBRARIES : the library to link against.
include(FindPackageHandleStandardArgs)
set(INTEL_ROOT "/opt/intel" CACHE PATH "Folder contains intel libs")
set(MKL_ROOT ${INTEL_ROOT}/mkl CACHE PATH "Folder contains MKL")
set(MKL_MULTI_THREADED true)
# Find include dir
find_path(MKL_INCLUDE_DIR mkl.h
    PATHS ${MKL_ROOT}/include)
# Find include directory
# There is no include folder under linux
# Handle suffix
set(_MKL_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
if(MKL_STATAIC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES .so)
endif()
# MKL is composed by four layers: Interface, Threading, Computational and RTL
if(MKL_SDL)
    find_library(MKL_LIBRARY mkl_rt
        PATHS ${MKL_ROOT}/lib/intel64/)
    set(MKL_MINIMAL_LIBRARY ${MKL_LIBRARY})
else()
    ######################### Interface layer #######################
    set(MKL_INTERFACE_LIBNAME mkl_intel_lp64)
    find_library(MKL_INTERFACE_LIBRARY ${MKL_INTERFACE_LIBNAME}
        PATHS ${MKL_ROOT}/lib/intel64/)
    ######################## Threading layer ########################
    if(MKL_MULTI_THREADED)
        set(MKL_THREADING_LIBNAME mkl_intel_thread)
    else()
        set(MKL_THREADING_LIBNAME mkl_sequential)
    endif()
    find_library(MKL_THREADING_LIBRARY ${MKL_THREADING_LIBNAME}
        PATHS ${MKL_ROOT}/lib/intel64/)
    ####################### Computational layer #####################
    find_library(MKL_CORE_LIBRARY mkl_core
        PATHS ${MKL_ROOT}/lib/intel64/)
    find_library(MKL_FFT_LIBRARY mkl_cdft_core
        PATHS ${MKL_ROOT}/lib/intel64/)
    find_library(MKL_SCALAPACK_LIBRARY mkl_scalapack_ilp64
        PATHS ${MKL_ROOT}/lib/intel64/)
    ############################ RTL layer ##########################
    set(MKL_RTL_LIBNAME libiomp5)
    find_library(MKL_RTL_LIBRARY ${MKL_RTL_LIBNAME}
        PATHS ${INTEL_ROOT}/composer_xe_2015.0.090/compiler/lib/intel64/)
    set(MKL_LIBRARY ${MKL_INTERFACE_LIBRARY} ${MKL_THREADING_LIBRARY} ${MKL_CORE_LIBRARY} ${MKL_SCALAPACK_LIBRARY} ${MKL_RTL_LIBRARY} ${MKL_FFT_LIBRARY} )
    set(MKL_MINIMAL_LIBRARY ${MKL_INTERFACE_LIBRARY} ${MKL_THREADING_LIBRARY} ${MKL_CORE_LIBRARY} ${MKL_RTL_LIBRARY})
endif()
set(CMAKE_FIND_LIBRARY_SUFFIXES ${_MKL_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES})
find_package_handle_standard_args(MKL DEFAULT_MSG
    MKL_INCLUDE_DIR MKL_LIBRARY MKL_MINIMAL_LIBRARY)
if(MKL_INCLUDE_DIR)
    set(MKL_FOUND true)
endif(MKL_INCLUDE_DIR)
if(MKL_FOUND)
    set(MKL_INCLUDE_DIRS ${MKL_INCLUDE_DIR})
    set(MKL_LIBRARIES ${MKL_LIBRARY})
    set(MKL_MINIMAL_LIBRARIES ${MKL_LIBRARY})
endif()
