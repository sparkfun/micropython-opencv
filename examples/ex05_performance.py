#-------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 SparkFun Electronics
#-------------------------------------------------------------------------------
# ex05_performance.py
# 
# This example demonstrates some performance optimization techniques, and ways
# to measure performance in the MicroPython port of OpenCV. Read through the
# comments in this example to learn more!
# 
# Note that most examples do not include these optimizations for simplicity, but
# if maximum performance is needed for your application, use the techniques
# shown here.
#-------------------------------------------------------------------------------

# Import OpenCV and hardware initialization module
import cv2 as cv
from cv2_hardware_init import *

# Import NumPy to create arrays
from ulab import numpy as np

# Import time for frame rate calculation
import time

# Import garbage collector to measure memory usage
import gc

# Many OpenCV functions can take an optional output argument to store the result
# of the operation. If it's not provided, OpenCV allocates a new array to store
# the result, which can be slow and waste memory. When it is provided, OpenCV
# instead writes the result to the provided array, reducing memory usage and
# improving performance. The array must have the same shape and data type as the
# expected output of the operation, otherwise a new array will be allocated.
# 
# Here we preallocate arrays for the destination arguments of this example. If
# the shapes or data types are incorrect, OpenCV will simply allocate new arrays
# for each on the first loop iteration. The variables will then be re-assigned,
# so this only negatively affects the first loop iteration.
frame = np.zeros((240, 320, 3), dtype=np.uint8)
result_image = np.zeros((240, 320, 3), dtype=np.uint8)

# Open the camera
camera.open()

# Initialize a loop timer to calculate processing speed in FPS
loop_time = time.ticks_us()

# Initialize a variable to track memory usage
last_mem_free = gc.mem_free()

# Prompt the user to press a key to continue
print("Press any key to continue")

# Loop to continuously read frames from the camera and display them
while True:
    # Read a frame from the camera and measure how long it takes. Try running
    # this both with and without the preallocated `frame` array to see the
    # difference in performance
    t0 = time.ticks_us()
    success, frame = camera.read(frame)
    t1 = time.ticks_us()
    print("Read frame: %.2f ms" % ((t1 - t0) / 1_000), end='\t')

    # Check if the frame was read successfully
    if not success:
        print("Failed to read frame from camera")
        break

    # Now we'll do some processing on the frame. Try running this with and
    # without the preallocated `result_image` array, and try different OpenCV
    # functions to compare performance
    t0 = time.ticks_us()
    result_image = cv.cvtColor(frame, cv.COLOR_BGR2HSV, result_image)
    t1 = time.ticks_us()
    print("Processing: %.2f ms" % ((t1 - t0) / 1_000), end='\t')

    # It's a good idea to measure the frame rate of the main loop to see how
    # fast the entire pipeline is running. This will include not only the
    # processing steps, but also any overhead from the hardware drivers and
    # other code. We can calculate the FPS with the loop timer and draw it on
    # the frame for visualization
    current_time = time.ticks_us()
    fps = 1_000_000 / (current_time - loop_time)
    loop_time = current_time
    print("FPS: %.2f" % fps, end='\t')
    result_image = cv.putText(result_image, f"FPS: {fps:.2f}", (10, 30), cv.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

    # Display the frame
    cv.imshow(display, result_image)

    # We can also measure memory usage to see how much RAM is being consumed by
    # this code. If you remove the output arguments from the functions above,
    # you'll see that the memory consumption increases significantly as new
    # arrays must be allocated each loop iteration
    # 
    # Note that calling `gc.mem_free()` actually takes a relatively long time to
    # execute, so it should only be used for debugging, not in production code
    mem_free = gc.mem_free()
    memory_used = last_mem_free - mem_free
    last_mem_free = mem_free
    print("Memory free: %d KiB" % (mem_free // 1024), end='\t')
    print("Memory consumed: %d KiB" % (memory_used // 1024), end='\n')

    # If the memory usage is negative, it means the garbage collector triggered
    # and freed some memory. Garbage collection can take some time, so you'll
    # notice a drop in FPS when it happens, and you may see a stutter in the
    # video stream on the display. This is another reason to preallocate arrays,
    # since it mitigates how frequently garbage collection is triggered
    if memory_used < 0:
        print("Garbage collection triggered!")
    
    # Something to try is triggering the garbage collector manually each loop
    # iteration to immediately free up memory. Garbage collection can be faster
    # if less memory has been allocated, so this can help avoid long stutters
    # from occasional garbage collection. However garbage collection will always
    # take *some* time, so this will lower the average FPS. You can choose to do
    # this if you prefer a consistent frame rate, or don't if you prefer maximum
    # frame rate and are okay with occasional stutters
    # gc.collect()

    # For advanced users, you can use the internal buffers of the camera and
    # display drivers: `camera._buffer` and `display._buffer`. Using these
    # buffers directly can avoid the colorspace conversions implemented in
    # `camera.read()` and `display.imshow()`, which can improve performance if
    # your application can make use of the native colorspaces and improve
    # overall performance

    # Check for key presses
    key = cv.waitKey(1)

    # If any key is pressed, exit the loop
    if key != -1:
        break

# Release the camera
camera.release()
