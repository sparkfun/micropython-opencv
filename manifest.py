# The manifest gets overwritten by the Makefile, so re-include the board's
# original manifest file
include("$(BOARD_DIR)/manifest.py")

# Include this directory as one package
package("cv2_drivers")

# Include the SD card module
require("sdcard")
