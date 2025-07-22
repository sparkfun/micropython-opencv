#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# ex04_imread_imwrite.py
# 
# This example demonstrates how to read and write images to and from the
# MicroPython filesystem using `cv.imread()` and `cv.imwrite()`. Any paths
# accessible to the MicroPython filesystem can be used, including an SD card if
# your board has one connected.
#-------------------------------------------------------------------------------

# Import OpenCV and hardware initialization module
import cv2 as cv
from cv2_hardware_init import *

# Call `cv.imread()` to read an image from the MicroPython filesystem, just
# like in any other Python environment! Make sure to copy the image to the
# MicroPython filesystem first, and set the path to the image file as needed
# 
# If your board can mount an SD card, you can instead load the image to the SD
# card and change the path to point to the SD card
# 
# Note - only BMP and PNG formats are currently supported in MicroPython OpenCV
print("Loading image...")
img = cv.imread("test_images/sparkfun_logo.png")

# Show the image
# 
# Note - If the image is larger or smaller than the display, the behavior will
# depend on the display driver. For example, the default ST7789 display driver
# will crop large images, and show small images in the top-left corner
cv.imshow(display, img)

# Prompt the user to press a key to continue
print("Press any key to continue")
key = cv.waitKey(0)

# Let's modify the image! Here we use `cv.Canny()` to perform edge detection
# on the image, which is a common operation in computer vision
print("Performing edge detection...")
edges = cv.Canny(img, 100, 200)

# Display the modified image
cv.imshow(display, edges)

# Now we'll save the modified image to the MicroPython filesystem using
# `cv.imwrite()`, just like in any other Python environment!
# 
# Again, SD cards are supported, just change the path to point to the SD card
# 
# Note - only BMP and PNG formats are currently supported in MicroPython OpenCV
print("Saving modified image...")
success = cv.imwrite("test_images/sparkfun_logo_edges.png", edges)

# Check if the image was saved successfully
if success:
    print("Image saved successfully!")
else:
    print("Failed to save the image!")
