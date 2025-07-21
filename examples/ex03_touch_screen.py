#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# ex03_touch_screen.py
# 
# This example demonstrates how to read input from a touch screen, which can be
# used to verify that the touch screen driver is functioning. It simply draws
# lines on a blank image based on touch input, similar to a drawing application.
#-------------------------------------------------------------------------------

# Import OpenCV and hardware initialization module
import cv2 as cv
from cv2_hardware_init import *

# Import NumPy
from ulab import numpy as np

# Initialize an image to draw on
img = np.zeros((240, 320, 3), dtype=np.uint8)

# Prompt the user to draw on the screen
img = cv.putText(img, "Touch to draw!", (10, 30), cv.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

# Prompt the user to press a key to continue
print("Press any key to continue")

# Create variables to store touch coordinates and state
x0, y0, x1, y1 = 0, 0, 0, 0
touch_input = False

# Loop to continuously read touch input and draw on the image
while True:
    # Check if there is touch input
    if touch_screen.is_touched():
        # Check if this is the first touch or a continuation
        if not touch_input:
            # This is the first touch, set both (x0, y0) and (x1, y1) to the
            # initial touch coordinates. This will draw a point at the touch
            # location if no further touch inputs are made
            x0, y0 = touch_screen.get_touch_xy()
            x1, y1 = x0, y0
            # Set the state to indicate there is touch input
            touch_input = True
        else:
            # This is a continuation of the touch, set (x0, y0) to the previous
            # coordinates and set (x1, y1) to the current touch coordinates so
            # we can draw a line between them
            x0, y0 = x1, y1
            x1, y1 = touch_screen.get_touch_xy()
    else:
        # Check if there was touch input before
        if touch_input:
            # There was touch input before, but not any more
            touch_input = False

    # Draw a line if there was touch input
    if touch_input:
        img = cv.line(img, (x0, y0), (x1, y1), (255, 255, 255), 2)

    # Display the frame
    display.imshow(img)

    # Check for key presses
    key = cv.waitKey(1)

    # If any key is pressed, exit the loop
    if key != -1:
        break
