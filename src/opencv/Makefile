ifndef PLATFORM
$(error PLATFORM not specified. Use 'make PLATFORM=rp2350' or similar.)
endif

TOOLCHAIN_FILE = ../../platforms/${PLATFORM}.toolchain.cmake

# TODO: For some reason, specifying this in the toolchain file doesn't work
CMAKE_ARGS += -DBUILD_LIST=core,imgproc,imgcodecs

# Generic build
all:
	cd opencv && mkdir -p build && cmake -S . -B build -DPICO_BUILD_DOCS=0 -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_FILE} ${CMAKE_ARGS} && make -C build -f Makefile $(MAKEFLAGS) $(MAKE_ARGS)

# Clean the OpenCV build
clean:
	cd opencv && rm -rf build
