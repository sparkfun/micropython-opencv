#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# ex06_detect_sfe_logo.py
# 
# This example demonstrates a basic vision processing pipeline. A pipeline is
# just a sequence of steps used to extract meaningful data from an image. The
# pipeline in this example attempts to detect the SparkFun flame logo using
# contour matching. If it's detected, it will be outlined on the display for
# visualization. The bounding box and center of the logo will also be drawn,
# demonstrating how to acquire useful numerical data from an image (eg. the
# position and size of an object).
#
# Note that this pipeline is very simple and does not include many of the steps
# that would typically be included in more robust pipelines. This was done for
# simplicity and performance, so it may produce false positives or miss the logo
# entirely sometimes.
#-------------------------------------------------------------------------------

# Import OpenCV and hardware initialization module
import cv2 as cv
from cv2_hardware_init import *

# Import NumPy
from ulab import numpy as np

# Import time for frame rate calculation
import time

# Here we define a reference contour for the SparkFun flame logo. This was
# created manually by picking points on the boundary of a small image of the
# logo in an image editor. Below is also ASCII art of the logo for reference,
# but the actual contour is drawn in the top left corner of the display.
#     ___
#    /  _\
#    \  \
#  /|_|  \/\
# |         |
# |         |
# |        /
# |  _____/
# | /
# |/
logo_contour = np.array(
    [[[0,48]],
     [[0,22]],
     [[4,16]],
     [[9,16]],
     [[7,19]],
     [[10,22]],
     [[13,22]],
     [[16,19]],
     [[16,17]],
     [[10,10]],
     [[10,5]],
     [[15,1]],
     [[20,0]],
     [[24,2]],
     [[19,5]],
     [[19,8]],
     [[23,12]],
     [[26,11]],
     [[26,8]],
     [[32,14]],
     [[32,25]],
     [[28,32]],
     [[20,36]],
     [[12,36]]], dtype=np.float)

# This is the pipeline implementation. This gets called for each frame captured
# by the camera in the main loop
def my_pipeline(frame):
    # Here we binarize the image. There are many ways to do this, but here we
    # simply convert the image to grayscale and then apply Otsu's thresholding
    # method to create a binary image. This means it will only detect a dark
    # logo on a light background (or vice versa), but you can modify this to
    # find specific colors or use other methods if desired
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    ret, thresh = cv.threshold(gray, 0, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)

    # Find contours in the binary image, which are simply lists of points around
    # the boundaries of shapes. Contours are a powerful tool in OpenCV for shape
    # analysis and object detection
    contours, hierarchy = cv.findContours(thresh, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)

    # It's possible that no contours were found, so first check if any were
    # found before proceeding
    if contours:
        # We'll compare the contours found in the image to the reference logo
        # contour defined earlier. We will use the `cv.matchShapes()` function
        # to compare the shapes to pick the best match, so we need to initialize
        # variables to keep track of the best match found so far
        best_contour = None
        best_similarity = float('inf') # Start with a very high similarity score

        # Loop through each contour found in the image to find the best match
        for i in range(len(contours)):
            # If the image is noisy, the binarized image may contain many tiny
            # contours that are obviously not the logo. `cv.matchShapes()` can
            # take some time, so we can be more efficient by skipping obviously
            # wrong contours. In this example, the logo we're looking for is
            # fairly complex, so we can skip contours that have too few points
            # since they will definitely be too simple to match the logo
            if len(contours[i]) < 20:
                continue

            # Now we call `cv.matchShapes()` which returns a "similarity" score
            # between the two shapes. The lower the score, the more similar the
            # shapes are
            similarity = cv.matchShapes(logo_contour, contours[i], cv.CONTOURS_MATCH_I2, 0)

            # Check if this contour is a better match than the best so far
            if similarity < best_similarity:
                # This contour is a better match, so update the best match
                best_similarity = similarity
                best_contour = contours[i]
        
        # We're done checking all contours. It's possible that the best contour
        # found is not a good match, so we can check if the score is below a
        # threshold to determine whether it's close enough. Testing has shown
        # that good matches are usually around 0.5, so we'll use a slightly
        # higher threshold of 1.0
        if best_similarity < 1.0:
            # The best contour found is a good match, so we'll draw it on the
            # frame to outline the detected logo for visualization
            frame = cv.drawContours(frame, [best_contour], -1, (0, 0, 255), 2)

            # Visualization is great, but the purpose of most real pipelines is
            # to extract useful data from the image. For example, suppose we
            # want to know where the logo is located in the image and how large
            # it is. We can use the bounding rectangle of the contour to get the
            # position and size of the logo
            left, top, width, height = cv.boundingRect(best_contour)
            center_x = left + width // 2
            center_y = top + height // 2

            # Now we could use this data for some task! For example, if we were
            # detecting an object that a robot needs to drive in front of, we
            # could turn to face it with the center point, then drive forwards
            # until the size is big enough (meaning we're close enough to it).
            #
            # This example doesn't actually make use of the data, so we'll just
            # draw the bounding box and center of the logo for visualization,
            # and add text of the position and size of the logo
            frame = cv.rectangle(frame, (left, top), (left + width, top + height), (255, 0, 0), 2)
            frame = cv.circle(frame, (center_x, center_y), 3, (0, 255, 0), -1)
            frame = cv.putText(frame, f"({center_x}, {center_y})", (center_x - 45, center_y - 10), cv.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
            frame = cv.putText(frame, f"{width}x{height}", (left, top - 10), cv.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 2)

# Initialize a loop timer to calculate processing speed in FPS
loop_time = time.ticks_us()

# Open the camera
camera.open()

# Prompt the user to press a key to continue
print("Press any key to continue")

# Loop to continuously read frames from the camera and display them
while True:
    # Read a frame from the camera
    success, frame = camera.read()
    if not success:
        print("Failed to read frame from camera")
        break
    
    # Call the pipeline function to process the frame
    my_pipeline(frame)

    # All processing is done! Calculate the frame rate and display it
    current_time = time.ticks_us()
    fps = 1_000_000 / (current_time - loop_time)
    loop_time = current_time
    frame = cv.putText(frame, f"FPS: {fps:.2f}", (40, 30), cv.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

    # Draw the reference logo contour in the top left corner of the frame
    frame[0:50, 0:40] = (0,0,0)
    frame = cv.drawContours(frame, [logo_contour], -1, (255, 255, 255), 1, offset=(2, 2))

    # Display the frame
    cv.imshow(display, frame)

    # Check for key presses
    key = cv.waitKey(1)

    # If any key is pressed, exit the loop
    if key != -1:
        break

# Release the camera
camera.release()
