#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# cv2_drivers/displays/__init__.py
# 
# Imports all available display drivers for MicroPython OpenCV.
#-------------------------------------------------------------------------------

# Import platform agnostic drivers
from . import st7789_spi

# Import sys module to check platform
import sys

# Import RP2 drivers
if 'rp2' in sys.platform:
    from . import st7789_pio
