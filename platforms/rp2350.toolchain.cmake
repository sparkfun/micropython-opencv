# Define PICO_SDK_PATH in your environment before running this script
if(NOT DEFINED ENV{PICO_SDK_PATH})
    message(FATAL_ERROR "PICO_SDK_PATH environment variable is not set. Please define it before running this script.")
endif()

# Include the RP2350 toolchain file
include("$ENV{PICO_SDK_PATH}/cmake/preload/toolchains/pico_arm_cortex_m33_gcc.cmake")

# Include the common embedded OpenCV settings
include("${CMAKE_CURRENT_LIST_DIR}/common.cmake")

# Set RP2350 specific settings
set(OPENCV_DISABLE_THREAD_SUPPORT ON)

# Fix for https://github.com/raspberrypi/pico-sdk/issues/2505
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -DOPENCV_INCLUDE_PORT_FILE=\\\"${CMAKE_CURRENT_LIST_DIR}/include/rp2350_unsafe_cv_xadd.h\\\"")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -DOPENCV_INCLUDE_PORT_FILE=\\\"${CMAKE_CURRENT_LIST_DIR}/include/rp2350_unsafe_cv_xadd.h\\\"")

# Fix for https://github.com/sparkfun/micropython-opencv/issues/31
# Source: https://docs.zephyrproject.org/4.0.0/doxygen/html/zephyr__stdint_8h_source.html
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -imacros ${CMAKE_CURRENT_LIST_DIR}/include/zephyr_stdint.h")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -imacros ${CMAKE_CURRENT_LIST_DIR}/include/zephyr_stdint.h")
