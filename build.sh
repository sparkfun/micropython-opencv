if which nproc > /dev/null; then
    MAKEOPTS="-j$(nproc)"
else
    MAKEOPTS="-j$(sysctl -n hw.ncpu)"
fi

# TODO: Could also make these opts into the build_micropython_opencv function if we care...
FROZEN_MODULES_DIR="$(dirname "$0")/frozen_modules"
FROZEN_EXAMPLES_ARCHIVE_SCRIPT="frozen_examples.py"
FROZEN_EXAMPLES_UNPACKED_DIR="micropython-opencv-examples"

# Uses freezefs to create a frozen filesystem archive for the provided directory.
# See https://github.com/bixb922/freezefs for more details on freezefs
# Options:
    # $1: The directory to freeze
    # $2: The name that you want the frozen directory to have once unpacked on the board
    # $3: The output file name for the frozen archive .py file
function create_frozen_fs {
    local DIR_TO_FREEZE=$1
    local DIR_NAME_ON_BOARD=$2
    local OUTPUT_FILE=$3

    echo "Creating frozen filesystem for directory: $DIR_TO_FREEZE"
    echo "The frozen directory will be named: $DIR_NAME_ON_BOARD"
    echo "The output file will be: $OUTPUT_FILE"

    cp -r $DIR_TO_FREEZE $DIR_NAME_ON_BOARD

    python -m freezefs $DIR_NAME_ON_BOARD $OUTPUT_FILE
}

# Options:
# $1: The directory to add to the manifest
# $2: The port (e.g. rp2)
# $3: The board (e.g. SPARKFUN_XRP_CONTROLLER)
# $4: The mpconfigboard file name (e.g. mpconfigboard.cmake or mpconfigboard.m) Default: mpconfigboard.cmake
function add_to_manifest {
    local DIR=$1
    local PORT=$2
    local BOARD=$3
    local MPCONFIG_FILE="${4:-mpconfigboard.cmake}"

    # Add the directory to the manifest file
    echo "Adding $DIR to the manifest for $PORT on $BOARD using $MPCONFIG_FILE"
    local BOARD_DIR="micropython/ports/${PORT}/boards/${BOARD}"

    # Create manifest.py if it doesn't exist
    if [ ! -f ${BOARD_DIR}/manifest.py ]; then
        echo "include(\"\$(PORT_DIR)/boards/manifest.py\")" > ${BOARD_DIR}/manifest.py

        # also add the necessary frozen manifest line to mpconfigboard.cmake: set(MICROPY_FROZEN_MANIFEST ${MICROPY_BOARD_DIR}/manifest.py)
        # We will use the optional MPCONFIG_FILE argument to determine if we should add this line

        if [ -n "$MPCONFIG_FILE" ]; then
            if [[ $MPCONFIG_FILE == *.mk ]]; then
                # e.g. for TEENSY which uses mpconfigboard.mk instead of mpconfigboard.cmake
                echo "Adding frozen manifest line to mpconfigboard.mk for $BOARD"
                printf "\nFROZEN_MANIFEST ?= \$(BOARD_DIR)/manifest.py" >> ${BOARD_DIR}/$MPCONFIG_FILE
            elif [[ $MPCONFIG_FILE == *.cmake ]]; then
                echo "Adding frozen manifest line to mpconfigboard.cmake for $BOARD"
                printf "\nset(MICROPY_FROZEN_MANIFEST \"\${MICROPY_BOARD_DIR}/manifest.py\")" >> ${BOARD_DIR}/$MPCONFIG_FILE
            fi
        fi
    fi  

    # Add the freeze line to the manifest.py for the board
    echo "Adding freeze line to manifest.py for $BOARD"
    printf "\nfreeze(\"${DIR}\")" >> ${BOARD_DIR}/manifest.py

    # Helpful for debugging during the build process, but can be removed if we'd rather not see this output...
    echo "Manifest.py for $BOARD:"
    cat ${BOARD_DIR}/manifest.py
}

# Adds the frozen data filesystem to the boot.py file for the given port
# Options:
    # $1: Port name
    # $2: Frozen data file path
    # $3: Copy Source: If copying imported frozen data to a mutable location, this is the directory name of the source (optional)
    # $4: Copy Destination: If copying imported frozen data to a mutable location, this is the directory name of the destination (optional)
    # $5: Add destination to sys.path? If true, the destination directory will be added to sys.path in _boot.py (optional)
    # NOTE: By providing the source and destination, the frozen data filesystem will be copied to a mutable location on the board
    #       If they are not provided, the frozen data filesystem will still be accessible, but will be read-only.
function add_frozen_data_to_boot_for_port {
    local TARGET_PORT_NAME=$1
    local FROZEN_DATA_FILE=$2
    local SOURCE_DIR=$3
    local DESTINATION_DIR=$4
    local ADD_TO_SYSPATH=${5:-false}

    # Remove the ".py" extension from the frozen data file
    local FROZEN_DATA_BASENAME=$(basename $FROZEN_DATA_FILE .py)

    # Check if the _boot.py file exists in the port's modules directory and error out if it does not
    if [ ! -f micropython/ports/${TARGET_PORT_NAME}/modules/_boot.py ]; then
        echo "Error: _boot.py file not found in ports/${TARGET_PORT_NAME}/modules/"
        exit 1
    fi

    # Add the frozen data filesystem to the _boot.py file
    local BOOT_FILE="micropython/ports/${TARGET_PORT_NAME}/modules/_boot.py"

    echo "Adding frozen data filesystem to ${BOOT_FILE}"
    echo "import ${FROZEN_DATA_BASENAME}" >> ${BOOT_FILE}

    # Now, copy the unpacked frozen data filesystem to a mutable location if the source and destination are provided
    if [ -n  "$SOURCE_DIR" ] && [ -n "$DESTINATION_DIR" ]; then
        echo "Copying frozen data from ${SOURCE_DIR} to ${DESTINATION_DIR} in _boot.py"
        local BOOT_FILE="micropython/ports/${TARGET_PORT_NAME}/modules/_boot.py"
        echo "import os" >> ${BOOT_FILE}
        # Simple recursive function to copy the directory tree (since i.e. shutil.copytree is not available on MicroPython)
        echo "def copytree(src, dst):" >> ${BOOT_FILE}
        echo "    try:" >> ${BOOT_FILE}
        echo "        os.mkdir(dst)" >> ${BOOT_FILE}
        echo "    except OSError:" >> ${BOOT_FILE}
        echo "        pass" >> ${BOOT_FILE}
        echo "    for entry in os.ilistdir(src):" >> ${BOOT_FILE}
        echo "        fname, typecode, _, _ = entry" >> ${BOOT_FILE}
        echo "        src_path = src + '/' + fname" >> ${BOOT_FILE}
        echo "        dst_path = dst + '/' + fname" >> ${BOOT_FILE}
        echo "        if typecode == 0x4000:" >> ${BOOT_FILE} # typecode == 0x4000 means directory
        echo "            copytree(src_path, dst_path)" >> ${BOOT_FILE}
        echo "        else:" >> ${BOOT_FILE}
        echo "            with open(src_path, 'rb') as fsrc:" >> ${BOOT_FILE}
        echo "                with open(dst_path, 'wb') as fdst:" >> ${BOOT_FILE}
        echo "                    fdst.write(fsrc.read())" >> ${BOOT_FILE}
        echo "copytree('${SOURCE_DIR}', '${DESTINATION_DIR}')" >> ${BOOT_FILE}
    fi

    # If the ADD_TO_SYSPATH flag is true, add the destination directory to sys.path
    if [ "$ADD_TO_SYSPATH" = true ]; then
        echo "Adding ${DESTINATION_DIR} to sys.path in _boot.py"
        echo "import sys" >> ${BOOT_FILE}
        echo "sys.path.append('/${DESTINATION_DIR}')" >> ${BOOT_FILE}
    fi

    # Helpful for debugging during the build process, but can be removed if we'd rather not see this output...
    echo "Content of _boot.py after adding frozen data filesystem:"
    cat micropython/ports/${TARGET_PORT_NAME}/modules/_boot.py
}

# Installs necessary dependencies and builds OpenCV and the firmware
# Also freezes the examples directory in a filesystem archive on the board
function build_micropython_opencv {
    # Install necessary packages (Could move into an install_dependencies.sh if we want this to be more explicit/modular)
    sudo apt-get update
    sudo apt install cmake python3 build-essential gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
    # Install necessary python packages (could also move this to a requirements.txt file)
    pip install freezefs

    # Create a directory for frozen modules, we can add arbitrary .py files to this directory in the future.
    # For now it will just contain the archived examples script.
    mkdir "$FROZEN_MODULES_DIR"

    # Create our frozen filesystem archive for the examples directory
    # Note the "." to make the read-only version of the examples directory hidden in IDEs like Thonny
    create_frozen_fs "examples" ".$FROZEN_EXAMPLES_UNPACKED_DIR" "$FROZEN_MODULES_DIR/$FROZEN_EXAMPLES_ARCHIVE_SCRIPT"

    # Add necessary content to the manifest file to freeze the modules in the provided directory
    add_to_manifest "$FROZEN_MODULES_DIR" "rp2" "SPARKFUN_XRP_CONTROLLER" "mpconfigvariant_LARGE_BINARY.cmake"

    # Add necessary content to the boot.py file to unpack the frozen data filesystem on boot
    # Provide the source and destination directories to copy the frozen data filesystem to a mutable (and non-hidden) location
    # Provide "true" as the last argument to add the destination directory to sys.path (since our examples directory contains modules that we want to be importable...)
    add_frozen_data_to_boot_for_port "rp2" "$FROZEN_EXAMPLES_ARCHIVE_SCRIPT" ".$FROZEN_EXAMPLES_UNPACKED_DIR" "$FROZEN_EXAMPLES_UNPACKED_DIR" true

    # Set Pico SDK path to $GITHUB_WORKSPACE/micropython/lib/pico-sdk if $GITHUB_WORKSPACE is set, otherwise use the current directory
    if [ -n "$GITHUB_WORKSPACE" ]; then
        export PICO_SDK_PATH="$GITHUB_WORKSPACE/micropython/lib/pico-sdk"
    else
        export PICO_SDK_PATH=$(dirname "$0")/micropython/lib/pico-sdk
    fi

    # Build MPY Cross compiler
    make -C micropython/mpy-cross

    # Update necessary MicroPython submodules
    make -C micropython/ports/rp2 BOARD=SPARKFUN_XRP_CONTROLLER submodules

    # Build OpenCV
    make -C src/opencv PLATFORM=rp2350 --no-print-directory ${MAKEOPTS}

    # Build firmware
    make BOARD=SPARKFUN_XRP_CONTROLLER ${MAKEOPTS}
}
