# Initializes various hardware components for OpenCV in MicroPython. The
# examples import this module, but you could instead create/edit a `boot.py`
# script to automatically initialize the hardware when the board boots up. See:
# https://micropython.org/resources/docs/en/latest/reference/reset_boot.html#id4

# Import the display driver
try:
    from .display import display
except:
    print("Display initialization failed, skipping...")

# Optional - Show a splash screen on the display with an optional filename (if
# not provided, it defaults to `splash.png` in the root directory of the
# MicroPython filesystem). If the file is not present, the driver will simply
# clear the display of any previous content
display.splash("opencv-examples/images/splash.png")

# Import the camera driver
try:
    from .camera import camera
except:
    print("Camera initialization failed, skipping...")

# Import the touch screen driver
try:
    from .touch_screen import touch_screen
except:
    print("Touch screen initialization failed, skipping...")

# Mount the SD card
try:
    # We don't actually need to import anything here, just want to run the
    # sd_card module so the SD card gets mounted to the filesystem. So just
    # import something then delete it to avoid clutter
    from .sd_card import sdcard
    del sdcard
except:
    print("SD card initialization failed, skipping...")
