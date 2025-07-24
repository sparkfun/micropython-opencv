#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# opencv_upy.cmake
# 
# CMake file for the MicroPython port of OpenCV.
#-------------------------------------------------------------------------------

# Create an INTERFACE library for our CPP module.
add_library(usermod_cv2 INTERFACE)

# Add our source files to the library.
target_sources(usermod_cv2 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/alloc.c
    ${CMAKE_CURRENT_LIST_DIR}/convert.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core.cpp
    ${CMAKE_CURRENT_LIST_DIR}/highgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgcodecs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgproc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/numpy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/opencv_upy.c
)

# Add the src directory as an include directory.
target_include_directories(usermod_cv2 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_cv2)

# OpenCV creates some global variables on the heap. These get created before
# the GC is initialized, so we need to allocate some space for them on the C
# heap. 64kB seems sufficient.
set(MICROPY_C_HEAP_SIZE 65536)

# Makes m_tracked_calloc() and m_tracked_free() available. These track pointers
# in a linked list to ensure the GC does not free them. Needed for some OpenCV
# functions
set(MICROPY_TRACKED_ALLOC 1)

# Set ULAB max number of dimensions to 4 (default is 2), which is needed for
# some OpenCV functions
target_compile_definitions(usermod INTERFACE ULAB_MAX_DIMS=4)

# Include ULAB
include(${CMAKE_CURRENT_LIST_DIR}/ulab/code/micropython.cmake)

# Include OpenCV
set(OpenCV_DIR ${CMAKE_CURRENT_LIST_DIR}/opencv/opencv/build)
find_package(OpenCV REQUIRED)
target_include_directories(usermod INTERFACE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(usermod INTERFACE ${OpenCV_LIBS})

# Tell the linker to wrap malloc, free, calloc and realloc. These are defined in
# alloc.cpp, and ensure OpenCV stuff gets allocated by the garbage collector.
target_link_libraries(usermod INTERFACE "-Wl,--wrap,malloc")
target_link_libraries(usermod INTERFACE "-Wl,--wrap,free")
target_link_libraries(usermod INTERFACE "-Wl,--wrap,calloc")
target_link_libraries(usermod INTERFACE "-Wl,--wrap,realloc")

# __NEWLIB__ is not defined for some reason, which causes a conflicting
# definition of uint here:
# https://github.com/opencv/opencv/blob/9cdd525bc59b34a3db8f6db905216c5398ca93d6/modules/core/include/opencv2/core/hal/interface.h#L35-L39
target_compile_definitions(usermod INTERFACE -D__NEWLIB__)
