#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# cv2_drivers/cameras/__init__.py
# 
# Imports all available camera drivers for MicroPython OpenCV.
#-------------------------------------------------------------------------------

# Import sys module to check platform
import sys

# Import RP2 drivers
if 'rp2' in sys.platform:
    from . import hm01b0_pio
    from . import ov5640_pio
