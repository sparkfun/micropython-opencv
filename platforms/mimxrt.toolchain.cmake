# Include the RP2350 toolchain file
# include("$ENV{PICO_SDK_PATH}/cmake/preload/toolchains/pico_arm_cortex_m33_gcc.cmake")

# include("/home/dryw/github/micropython/lib/nxp_driver/sdk/devices/MIMXRT1062/all_lib_device_MIMXRT1062.cmake")

set(CMAKE_SYSTEM_PROCESSOR cortex-m7)
set(CMAKE_SYSTEM_NAME MIMXRT)
set(CMAKE_C_COMPILER arm-none-eabi-gcc CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER arm-none-eabi-g++ CACHE FILEPATH "C++ compiler")
set(CMAKE_C_FLAGS_INIT " -mcpu=cortex-m7 -mthumb -mtune=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16")
set(CMAKE_CXX_FLAGS_INIT " -mcpu=cortex-m7 -mthumb -mtune=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16")

set(CMAKE_C_LINK_FLAGS "-Wl,--build-id=none")
set(CMAKE_CXX_LINK_FLAGS "-Wl,--build-id=none")

get_property(IS_IN_TRY_COMPILE GLOBAL PROPERTY IN_TRY_COMPILE)
if(IS_IN_TRY_COMPILE)
    set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -nostdlib")
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -nostdlib")
endif()

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
