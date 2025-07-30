#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# ex07_animation.py
# 
# This example demonstrates how to play an animation using a series of frames
# stored in a single image file. It assumes full 320x240 frames are stacked
# vertically in the image, and the animation plays by displaying each frame in
# sequence. This can be the basis for things like sprite sheets, where smaller
# icons or characters are stored in a single image and displayed as needed.
#-------------------------------------------------------------------------------

# Import OpenCV and hardware initialization module
import cv2 as cv
from cv2_hardware_init import *

# Load an animation sheet image that contains multiple frames of an animation
animation_sheet = cv.imread("opencv-examples/images/animation_sheet.png")

# This example assumes the image has full 320x240 frames stacked vertically
frame_height = 240

# Calculate the number of frames in the sheet by dividing the sheet height by
# the frame height
frame_num = animation_sheet.shape[0] // frame_height

# Initialize variables to keep track of the current row in the sheet and the
# direction of animation playback (up or down)
row_index = 0
direction = 1

# Prompt the user to press a key to continue
print("Press any key to continue")

# Loop to continuously play the animation
while True:
    # Calculate the starting and ending pixel row for the current frame
    row_start_px = row_index * frame_height
    row_end_px = row_start_px + frame_height
    cv.imshow(display, animation_sheet[row_start_px:row_end_px, :])

    # Update the row index based on the direction of playback
    row_index += direction

    # If we reach the end of the sheet, reverse the direction
    if row_index == frame_num-1:
        direction = -1
    elif row_index == 0:
        direction = 1

    # Check for key presses. If you want the animation to play at a specific
    # frame rate, you can change the wait time to slow it down. This example
    # plays the animation as fast as possible, which is often needed to look
    # smooth in MicroPython
    key = cv.waitKey(1)

    # If any key is pressed, exit the loop
    if key != -1:
        break
