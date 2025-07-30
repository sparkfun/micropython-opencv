#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# ex02_camera.py
# 
# This example demonstrates how to read frames from a camera and show them on a
# display using OpenCV in MicroPython. It can be used to verify that the camera
# driver is functioning.
#-------------------------------------------------------------------------------

# Import OpenCV and hardware initialization module
import cv2 as cv
from cv2_hardware_init import *

# Open a camera, similar to any other Python environment! In standard OpenCV,
# you would use `cv.VideoCapture(0)` or similar, and OpenCV would leverage the
# host operating system to open a camera object and return it as a
# `cv.VideoCapture` object. However, we don't have that luxury in MicroPython,
# so a camera driver is required instead. Any camera driver can be used, as long
# as it implements the same methods as the standard OpenCV `cv.VideoCapture`
# class, such as `open()`, `read()`, and `release()`
camera.open()

# Prompt the user to press a key to continue
print("Press any key to continue")

# Loop to continuously read frames from the camera and display them
while True:
    # Read a frame from the camera, just like any other Python environment! It
    # returns a tuple, where the first element is a boolean indicating success,
    # and the second element is the frame (NumPy array) read from the camera
    success, frame = camera.read()

    # Check if the frame was read successfully
    if not success:
        print("Error reading frame from camera")
        break

    # Display the frame
    cv.imshow(display, frame)

    # Check for key presses
    key = cv.waitKey(1)

    # If any key is pressed, exit the loop
    if key != -1:
        break

# Release the camera, just like in any other Python environment!
camera.release()
