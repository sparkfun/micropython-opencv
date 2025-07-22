#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# ex02_grab_orange_ring.py
# 
# The XRP can act as a bridge to FIRST programs, which includes summer camps
# with FIRST-style games. Learn more here:
# https://experientialrobotics.org/bridge-to-first/
# 
# FIRST-style games often include game elements with randomized locations that
# can be detected with a camera. The exact game elements and tasks change every
# year, but this example assumes there is an orange ring in front of the robot
# that needs to be grabbed. This example demonstrates how to detect the ring,
# calculate its distance and position relative to the robot in real-world units,
# then drive the robot to grab it. This requires the servo arm to be mounted to
# the front of the chassis right next to the camera, so it can reach through the
# ring to grab it.
# 
# The ring used in this example is from the 2020-2021 FIRST Tech Challenge game
# Ultimate Goal, and can be purchased here:
# https://andymark.com/products/5-in-foam-ring
#-------------------------------------------------------------------------------

# Import XRPLib defaults
from XRPLib.defaults import *

# Import OpenCV and hardware initialization module
import cv2 as cv
from cv2_hardware_init import *

# Import time for delays
import time

# Import math for calculations
import math

# This is the pipeline implementation that attempts to find an orange ring in
# an image, and returns the real-world distance to the ring and its left/right
# position relative to the center of the image in centimeters
def find_orange_ring_pipeline(frame):
    # Convert the frame to HSV color space, which is often more effective for
    # color-based segmentation tasks than RGB or BGR color spaces
    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    # Here we use the `cv.inRange()` function to find all the orange pixels.
    # This outputs a binary image where pixels that fall within the specified
    # lower and upper bounds are set to 255 (white), and all other pixels are
    # set to 0 (black). This is applied to the HSV image, so the lower and upper
    # bounds are in HSV color space. The bounds were determined experimentally:
    # 
    # Hue: Orange hue is around 20, so we use a range of 15 to 25
    # Saturation: Anything above 50 is saturated enough
    # Value: Anything above 30 is bright enough
    lower_bound = (15, 50, 30)
    upper_bound = (25, 255, 255)
    inRange = cv.inRange(hsv, lower_bound, upper_bound)

    # Noise in the image often causes `cv.inRange()` to return false positives
    # and false negatives, meaning there are some incorrect pixels in the binary
    # image. These can be cleaned up with morphological operations, which
    # effectively grow and shrink regions in the binary image to remove tiny
    # blobs of noise
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (3, 3))
    morphOpen = cv.morphologyEx(inRange, cv.MORPH_OPEN, kernel)
    morphClose = cv.morphologyEx(morphOpen, cv.MORPH_CLOSE, kernel)

    # Now we use `cv.findContours()` to find the contours in the binary image,
    # which are the boundaries of the regions in the binary image
    contours, hierarchy = cv.findContours(morphClose, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

    # It's possible that no contours were found, so first check if any were
    # found before proceeding
    best_contour = None
    if contours:
        # It's possible that some tiny blobs of noise are still present in the
        # binary image, or other objects entirely, leading to extra contours. A
        # proper pipeline would make an effort to filter out unwanted contours
        # based on size, shape, or other criteria. This example keeps it simple;
        # the contour of a ring is a circle, meaning many points are needed to
        # represent it. A contour with only a few points is obviously not a
        # circle, so we can ignore it. This example assumes the ring is the only
        # large orange object in the image, so the first contour that's complex
        # enough is probably the one we're looking for
        for i in range(len(contours)):
            if len(contours[i]) < 50:
                continue
            best_contour = contours[i]
            break
    
    # If no contour was found, return invalid values to indicate that
    if best_contour is None:
        return (-1, -1)

    # Calculate the bounding rectangle of the contour, and use that to calculate
    # the center coordinates of the ring
    left, top, width, height = cv.boundingRect(best_contour)
    center_x = left + width // 2
    center_y = top + height // 2

    # Now we can calculate the real-world distance to the ring based on its
    # size. We'll first estimate the diameter of the ring in pixels by taking
    # the maximum of the width and height of the bounding rectangle. This
    # compensates for the fact that the ring may be tilted
    diameter_px = max(width, height)
    
    # If the camera has a perfect lens, the distance can be calculated with:
    # 
    # distance_cm = diameter_cm * focal_length_px / diameter_px
    # 
    # Almost every camera lens has some distortion, so this may not be perfect,
    # but testing with the HM01B0 has shown it to be good enough. Note that this
    # distance is measured from the camera lens
    # 
    # The focal length depends on the exact camera being used. This example
    # assumes the HM01B0 camera board sold by SparkFun, which has an effective
    # focal length (EFL) of 0.66mm, and a pixel size of 3.6um. We can calculate
    # the focal length in pixels from these, which were found in the datasheet:
    # https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/5458/HM01B0-ANA-00FT870.pdf
    focal_length_px = 660 / 3.6
    diameter_cm = 12.7
    distance_cm = diameter_cm * focal_length_px / diameter_px

    # Now with our distance estimate, we can calculate how far left or right the
    # ring is from the center in the same real-world units. Assuming a perfect
    # lens, the position can be calculated as:
    #
    # position_x_cm = distance_cm * position_x_px / focal_length_px
    position_x_px = center_x - (frame.shape[1] // 2)
    position_x_cm = distance_cm * position_x_px / focal_length_px

    # Draw the contour, bounding box, center, and text for visualization
    frame = cv.drawContours(frame, [best_contour], -1, (0, 0, 255), 2)
    frame = cv.rectangle(frame, (left, top), (left + width, top + height), (255, 0, 0), 2)
    frame = cv.drawMarker(frame, (center_x, center_y), (0, 255, 0), cv.MARKER_CROSS, 10, 2)
    frame = cv.putText(frame, f"({center_x}, {center_y})", (center_x - 45, center_y - 10), cv.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
    frame = cv.putText(frame, f"{width}x{height}", (left, top - 10), cv.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 2)
    frame = cv.putText(frame, f"D={distance_cm:.1f}cm", (left, top - 25), cv.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
    frame = cv.putText(frame, f"X={position_x_cm:.1f}cm", (left, top - 40), cv.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)

    # Now we can return the distance and position of the ring in cm, since
    # that's the only data we need from this pipeline
    return (distance_cm, position_x_cm)

# Move the servo out of the way of the camera
servo_one.set_angle(90)

# Open the camera and wait a moment for at least one frame to be captured
camera.open()
time.sleep(0.1)

# Prompt the user to press a key to continue
print("Detecting ring...")

# Loop until the ring is found or the user presses a key
while True:
    # Read a frame from the camera
    success, frame = camera.read()
    if success == False:
        print("Error reading frame from camera")
        break

    # Call the pipeline function to find the ring
    distance_cm, position_x_cm = find_orange_ring_pipeline(frame)

    # Display the frame
    cv.imshow(display, frame)

    # If the distance is valid, break the loop
    if distance_cm >= 0:
        break

    # Check for key presses
    key = cv.waitKey(1)

    # If any key is pressed, exit the loop
    if key != -1:
        break

# Print the distance and position of the ring
print(f"Found ring at distance {distance_cm:.1f} cm, X position {position_x_cm:.1f} cm from center")

# Release the camera, we're done with it
camera.release()

# Move the servo to go through the center of the ring
servo_one.set_angle(45)

# Turn to face the ring. We first calculate the angle to turn based on the
# position of the ring
angle = -math.atan2(position_x_cm, distance_cm) * 180 / math.pi
drivetrain.turn(angle)

# Drive forwards to put the arm through the ring
drivetrain.straight(distance_cm)

# Rotate the servo to pick up the ring
servo_one.set_angle(90)

# Drive backwards to grab the ring
drivetrain.straight(-10)
