
# include $(CPPEXAMPLE_MOD_DIR)/ulab/code/micropython.mk

CPPEXAMPLE_MOD_DIR := $(USERMOD_DIR)

# Add our source files to the respective variables.
SRC_USERMOD += $(CPPEXAMPLE_MOD_DIR)/src/alloc.c
SRC_USERMOD_CXX += $(CPPEXAMPLE_MOD_DIR)/src/convert.cpp
SRC_USERMOD_CXX += $(CPPEXAMPLE_MOD_DIR)/src/core.cpp
SRC_USERMOD_CXX += $(CPPEXAMPLE_MOD_DIR)/src/highgui.cpp
SRC_USERMOD_CXX += $(CPPEXAMPLE_MOD_DIR)/src/imgcodecs.cpp
SRC_USERMOD_CXX += $(CPPEXAMPLE_MOD_DIR)/src/imgproc.cpp
SRC_USERMOD_CXX += $(CPPEXAMPLE_MOD_DIR)/src/numpy.cpp
SRC_USERMOD += $(CPPEXAMPLE_MOD_DIR)/src/opencv_upy.c

CMAKE_C_FLAGS_INIT += -imacros $(CPPEXAMPLE_MOD_DIR)/include/zephyr_stdint.h
CMAKE_CXX_FLAGS_INIT += -imacros $(CPPEXAMPLE_MOD_DIR)/include/zephyr_stdint.h


CFLAGS_USERMOD += -DULAB_MAX_DIMS=4
CXXFLAGS_USERMOD += -DULAB_MAX_DIMS=4

# Add our module directory to the include path.
CFLAGS_USERMOD += -I$(CPPEXAMPLE_MOD_DIR) -DULAB_MAX_DIMS=4
CXXFLAGS_USERMOD += -I$(CPPEXAMPLE_MOD_DIR) \
	-I$(CPPEXAMPLE_MOD_DIR)/opencv/build \
	-I$(CPPEXAMPLE_MOD_DIR)/opencv/modules/core/include \
	-I$(CPPEXAMPLE_MOD_DIR)/opencv/modules/imgcodecs/include \
	-I$(CPPEXAMPLE_MOD_DIR)/opencv/modules/imgproc/include \
	-std=c++11 \
	-Wno-error=float-conversion \
	-DOPENCV_DISABLE_THREAD_SUPPORT=ON \
	-DULAB_HAS_SCIPY=0

# __NEWLIB__ is not defined for some reason, which causes a conflicting
# definition of uint here:
# https://github.com/opencv/opencv/blob/9cdd525bc59b34a3db8f6db905216c5398ca93d6/modules/core/include/opencv2/core/hal/interface.h#L35-L39
# target_compile_definitions(usermod INTERFACE -D__NEWLIB__)
CFLAGS_USERMOD += -D__NEWLIB__
CXXFLAGS_USERMOD += -D__NEWLIB__

# set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -imacros ${CMAKE_CURRENT_LIST_DIR}/include/zephyr_stdint.h")
# set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -imacros ${CMAKE_CURRENT_LIST_DIR}/include/zephyr_stdint.h")

# We use C++ features so have to link against the standard library.
LDFLAGS_USERMOD += \
	-L$(CPPEXAMPLE_MOD_DIR)/opencv/build/lib \
	-L$(CPPEXAMPLE_MOD_DIR)/opencv/build/3rdparty/lib

LIBS_USERMOD += \
	-lopencv_imgcodecs \
	-lopencv_imgproc\
	-lopencv_core \
	-llibpng \
	-lzlib \
	-lm \
	-lstdc++ \
	-lsupc++
