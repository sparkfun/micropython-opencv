# Import OpenCV
import cv2
from cv2_hardware_init import *
from ulab import numpy as np
import time

# Here we define a reference contour for the SparkFun flame logo. This was
# created manually by picking points on the boundary of a small image of the
# logo in an image editor. This gets drawn in the top left corner of the
# display for reference
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

    # Here we binarize the image. There are many ways to do this, but here we
    # simply convert the image to grayscale and then apply Otsu's thresholding
    # method to create a binary image. This means it will only detect a dark
    # logo on a light background (or vice versa), but you can modify this to
    # find specific colors or use other methods if desired
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    thresh = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

    # Find contours in the binary image, which represent the boundaries of
    # shapes. Contours are a powerful tool in OpenCV for shape analysis and
    # object detection
    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # It's possible that no contours were found, so first check if any were
    # found before proceeding
    if contours:
        # We'll compare the contours found in the image to the reference logo
        # contour defined earlier. We will use the `cv2.matchShapes()` function
        # to compare the shapes to pick the best match, so we need to initialize
        # variables to keep track of the best match found so far
        best_contour = None
        best_similarity = float('inf') # Start with a very high similarity score

        # Loop through each contour found in the image to find the best match
        for i in range(len(contours)):
            # If the image is noisy, the binarized image may contain many tiny
            # contours that are obviously not the logo. `cv2.matchShapes()` can
            # take some time, so we can be more efficient by skipping obviously
            # wrong contours. In this example, the logo we're looking for is
            # fairly complex, so we can skip contours that have too few points
            # since they will definitely be too simple to match the logo
            if len(contours[i]) < 20:
                continue

            # Now we call `cv2.matchShapes()` which returns a "similarity" score
            # between the two shapes. The lower the score, the more similar the
            # shapes are
            similarity = cv2.matchShapes(logo_contour, contours[i], cv2.CONTOURS_MATCH_I2, 0)

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
            # Now we'll draw the best contour found on the original image
            frame = cv2.drawContours(frame, [best_contour], -1, (0, 0, 255), 2)

    # All processing is done! Calculate the frame rate and display it
    current_time = time.ticks_us()
    fps = 1000000 / (current_time - loop_time)
    loop_time = current_time
    frame = cv2.putText(frame, f"FPS: {fps:.2f}", (40, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

    # Draw the reference logo contour in the top left corner of the frame
    frame[0:50, 0:40] = (0,0,0)
    frame = cv2.drawContours(frame, [logo_contour], -1, (255, 255, 255), 1, offset=(2, 2))

    # Display the frame
    cv2.imshow(display, frame)

    # Check for key presses
    key = cv2.waitKey(1)

    # If any key is pressed, exit the loop
    if key != -1:
        break

# Release the camera
camera.release()
