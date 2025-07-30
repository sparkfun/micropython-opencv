#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# ex01_touch_drive.py
# 
# This example creates a simple touch screen interface to drive the XRP robot.
# It creates arrow buttons to drive around, and a stop button to exit the
# example. The XRP is available from SparkFun:
# https://www.sparkfun.com/experiential-robotics-platform-xrp-kit.html
#-------------------------------------------------------------------------------

# Import XRPLib defaults
from XRPLib.defaults import *

# Import OpenCV and hardware initialization module
import cv2 as cv
from cv2_hardware_init import *

# Import NumPy
from ulab import numpy as np

# Dimensions and properties for the UI elements
ui_shape = (240, 320, 3)
ui_cx = ui_shape[1] // 2
ui_cy = ui_shape[0] // 2
button_size = 50
button_cx = button_size // 2
button_cy = button_size // 2
button_spacing = 75
button_shape = (button_size, button_size, 3)
button_color = (255, 255, 255)
arrow_length = 30
arrow_thickness = 5
arrow_tip_length = 0.5
arrow_background_color = (255, 0, 0)
stop_size = 25
stop_background_color = (0, 0, 255)

def create_ui_image():
    # Initialize arrow button images. This could be done with a single image
    # that gets transposed and flipped, but ulab's transpose() doesn't support
    # the axes argument:
    # https://github.com/v923z/micropython-ulab/issues/731
    # So we instead create separate images for vertical and horizontal arrows
    img_arrow_vertical = np.zeros(button_shape, dtype=np.uint8)
    img_arrow_vertical[:, :] = arrow_background_color
    img_arrow_horizontal = img_arrow_vertical.copy()
    img_arrow_vertical = cv.arrowedLine(
        img_arrow_vertical,
        (button_cx, button_cy + arrow_length // 2),
        (button_cx, button_cy - arrow_length // 2),
        button_color,
        arrow_thickness,
        cv.FILLED,
        0,
        arrow_tip_length
    )
    img_arrow_horizontal = cv.arrowedLine(
        img_arrow_horizontal,
        (button_cx - arrow_length // 2, button_cy),
        (button_cx + arrow_length // 2, button_cy),
        button_color,
        arrow_thickness,
        cv.FILLED,
        0,
        arrow_tip_length
    )

    # Initialize stop button image
    img_button_stop = np.zeros(button_shape, dtype=np.uint8)
    img_button_stop[:, :] = stop_background_color
    img_button_stop = cv.rectangle(
        img_button_stop,
        (button_cx - stop_size // 2, button_cy - stop_size // 2),
        (button_cx + stop_size // 2, button_cy + stop_size // 2),
        button_color,
        cv.FILLED
    )

    # Initialize UI image
    img_ui = np.zeros(ui_shape, dtype=np.uint8)
    
    # Draw the stop button in the center
    img_ui[
        ui_cy-button_cy:ui_cy+button_cy,
        ui_cx-button_cx:ui_cx+button_cx
    ] = img_button_stop
    
    # Draw the forward arrow above the stop button
    img_ui[
        ui_cy-button_spacing-button_cy:ui_cy-button_spacing+button_cy,
        ui_cx-button_cx:ui_cx+button_cx
    ] = img_arrow_vertical
    
    # Draw the backward arrow below the stop button
    img_ui[
        ui_cy+button_spacing-button_cy:ui_cy+button_spacing+button_cy,
        ui_cx-button_cx:ui_cx+button_cx
    ] = img_arrow_vertical[::-1, :]  # Flip the arrow image vertically
    
    # Draw the right arrow to the right of the stop button
    img_ui[
        ui_cy-button_cy:ui_cy+button_cy,
        ui_cx+button_spacing-button_cx:ui_cx+button_spacing+button_cx
    ] = img_arrow_horizontal
    
    # Draw the left arrow to the left of the stop button
    img_ui[
        ui_cy-button_cy:ui_cy+button_cy,
        ui_cx-button_spacing-button_cx:ui_cx-button_spacing+button_cx
    ] = img_arrow_horizontal[:, ::-1]  # Flip the arrow image horizontally

    # Return the UI image
    return img_ui

# Create the UI image and show it on the display
cv.imshow(display, create_ui_image())

# Prompt the user to touch the screen to drive around
print("Touch the screen to drive around. Press any key to exit.")

# Loop to continuously read touch input and drive around
while True:
    # Check if there is touch input
    if touch_screen.is_touched():
        # Read touch coordinates
        x, y = touch_screen.get_touch_xy()
        
        # Check if the stop button was pressed
        if (ui_cx - button_cx <= x <= ui_cx + button_cx and
            ui_cy - button_cy <= y <= ui_cy + button_cy):
            print("Stop")
            break
        
        # Check if the forward arrow was pressed
        elif (ui_cx - button_cx <= x <= ui_cx + button_cx and
              ui_cy - button_spacing - button_cy <= y <= ui_cy - button_spacing + button_cy):
            print("Forward")
            # drivetrain.straight(20, 0.5)
        
        # Check if the backward arrow was pressed
        elif (ui_cx - button_cx <= x <= ui_cx + button_cx and
              ui_cy + button_spacing - button_cy <= y <= ui_cy + button_spacing + button_cy):
            print("Backward")
            # drivetrain.straight(-20, 0.5)
        
        # Check if the right arrow was pressed
        elif (ui_cy - button_cy <= y <= ui_cy + button_cy and
              ui_cx + button_spacing - button_cx <= x <= ui_cx + button_spacing + button_cx):
            print("Right")
            # drivetrain.turn(-90, 0.5)
        
        # Check if the left arrow was pressed
        elif (ui_cy - button_cy <= y <= ui_cy + button_cy and
              ui_cx - button_spacing - button_cx <= x <= ui_cx - button_spacing + button_cx):
            print("Left")
            # drivetrain.turn(90, 0.5)

    # Check for key presses
    key = cv.waitKey(1)

    # If any key is pressed, exit the loop
    if key != -1:
        break

# Clear the display to remove the UI
display.splash()
