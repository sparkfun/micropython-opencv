# Import OpenCV
import cv2

# Open a camera, similar to any other Python environment! In standard OpenCV,
# you would use `cv2.VideoCapture(0)` or similar, and OpenCV would leverage the
# host operating system to open a camera object and return it as a
# `cv2.VideoCapture` object. However, we don't have that luxury in MicroPython,
# so a camera driver is required instead. Any camera driver can be used, as long
# as it implements the same methods as the standard OpenCV `cv2.VideoCapture`
# class, such as `open()`, `read()`, and `release()`
# 
# This example assumes a camera driver called `camera` has been initialized by a
# `boot.py` script. See the example `boot.py` script for more details
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
    if success == False:
        print("Error reading frame from camera")
        break

    # Display the frame
    cv2.imshow(display, frame)

    # Check for key presses
    key = cv2.waitKey(1)

    # If any key is pressed, exit the loop
    if key != -1:
        break

# Release the camera, just like in any other Python environment!
camera.release()
