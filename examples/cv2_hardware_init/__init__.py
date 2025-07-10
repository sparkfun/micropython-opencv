# Initializes various hardware components for OpenCV in MicroPython. The
# examples import this module, but you could instead create/edit a `boot.py`
# script to automatically initialize the hardware when the board boots up. See:
# https://micropython.org/resources/docs/en/latest/reference/reset_boot.html#id4

try:
    from .display import display
except:
    print("Display initialization failed, skipping...")

try:
    from .camera import camera
except:
    print("Camera initialization failed, skipping...")

try:
    from .touch_screen import touch_screen
except:
    print("Touch screen initialization failed, skipping...")

try:
    # We don't actually need to import anything here, just want to run the
    # sd_card module so the SD card gets mounted to the filesystem. So just
    # import something then delete it to avoid clutter
    from .sd_card import sdcard
    del sdcard
except:
    print("SD card initialization failed, skipping...")

# Optional - show a splash image on the display if one is available, or clear
# the display of any previous content
try:
    # Load and display a splash image, if one is available
    import cv2
    splash_image = cv2.imread("splash.png")
    cv2.imshow(display, splash_image)
except Exception:    
    # No splash image, instead clear the display if the driver supports it
    if hasattr(display, 'clear'):
        display.clear()
