#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# ex05_performance.py
# 
# This example
#-------------------------------------------------------------------------------

# Import OpenCV and hardware initialization module
import cv2 as cv
from cv2_hardware_init import *

# Import NumPy
from ulab import numpy as np

# Import time for frame rate calculation
import time

# Initialize a loop timer to calculate processing speed in FPS
loop_time = time.ticks_us()

# Open the camera
camera.open()

# The `read()` method of OpenCV camera drivers can optionally take an output
# image as an argument. When it's not provided, the camera driver must allocate
# a whole new image for the frame, which can be slow and waste memory. If the
# image argument is provided, then the camera driver will write the data to the
# provided image. The image must be a NumPy array with the same shape and data
# type as the camera's
success, frame = camera.read()

# Prompt the user to press a key to continue
print("Press any key to continue")

# Loop to continuously read frames from the camera and display them
while True:
    # Read a frame from the camera
    success, frame = camera.read()
    if not success:
        print("Failed to read frame from camera")
        break

    # Now we'll 

    # It's a good idea to measure the frame rate of the main loop to see how
    # fast the entire pipeline is running. This will include not only the
    # processing steps, but also any overhead from the hardware drivers and
    # other code. We can calculate the FPS with the loop timer and draw it on
    # the frame for visualization
    current_time = time.ticks_us()
    fps = 1_000_000 / (current_time - loop_time)
    loop_time = current_time
    frame = cv.putText(frame, f"FPS: {fps:.2f}", (10, 30), cv.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

    # Display the frame
    cv.imshow(display, frame)

    # Check for key presses
    key = cv.waitKey(1)

    # If any key is pressed, exit the loop
    if key != -1:
        break

# Release the camera
camera.release()
