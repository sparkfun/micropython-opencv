# Set Pico SDK flags to create our own malloc wrapper and enable exceptions
CMAKE_ARGS += -DSKIP_PICO_MALLOC=1 -DPICO_CXX_ENABLE_EXCEPTIONS=1

# Get current directory
CURRENT_DIR = $(shell pwd)

# Set the MicroPython user C module path to the OpenCV module
MAKE_ARGS = USER_C_MODULES="$(CURRENT_DIR)/src/opencv_upy.cmake"

# Ensure we're building the OpenCV board variant
MAKE_ARGS += BOARD_VARIANT=LARGE_BINARY

# Use the OpenCV driver manifest
MAKE_ARGS += FROZEN_MANIFEST="$(CURRENT_DIR)/manifest.py"

# Build MicroPython with the OpenCV module
all:
	@cd micropython/ports/rp2 && export CMAKE_ARGS="$(CMAKE_ARGS)" && make -f Makefile $(MAKEFLAGS) $(MAKE_ARGS)

# Clean the MicroPython build
clean:
	@cd micropython/ports/rp2 && make -f Makefile $(MAKEFLAGS) $(MAKE_ARGS) clean

# Load the MicroPython submodules
submodules:
	@cd micropython/ports/rp2 && make -f Makefile $(MAKEFLAGS) $(MAKE_ARGS) submodules
