# Import the machine module to access hardware features
import machine

# Initialize SPI bus, assuming default pins on bus 0. You may need to adjust
# this based on your specific board and configuration
spi = machine.SPI(0)

# Initialize I2C bus, assuming default pins on bus 0. You may need to adjust
# this based on your specific board and configuration
i2c = machine.I2C(0)

# Initialize display, if available
try:
    # Import a display driver module. This example assumes the ST7789, which is
    # a very popular display driver for embedded systems. Moreover, this example
    # uses an SPI-based driver, so it should work on any platform, but it's not
    # always the fastest option
    from cv2_drivers.displays import st7789_spi

    # Create a display object. This will depend on the display driver you are
    # using, and you may need to adjust the parameters based on your specific
    # display and board configuration
    display = st7789_spi.ST7789_SPI(width=240,
                                    height=320,
                                    spi=spi,
                                    pin_dc=16,
                                    pin_cs=17,
                                    rotation=1)
except ImportError:
    print("boot.py - Display driver module not found, skipping display initialization.")

# Initialize SD card, if available
try:
    # Import the SD card module. This is often not installed by default in
    # MicroPython, so you may need to install it manually. For example, you can
    # use `mpremote mip install sdcard`
    import sdcard

    # This example assumes the SD card is on the same SPI bus as the display
    # with a different chip select pin. You may need to adjust this based on
    # your specific board and configuration
    sd_cs = machine.Pin(7, machine.Pin.OUT)
    sd = sdcard.SDCard(spi, sd_cs)
    
    # Mount the SD card to the filesystem under the "/sd" directory, which makes
    # it accessible just like the normal MicroPython filesystem
    import uos
    vfs = uos.VfsFat(sd)
    uos.mount(vfs, "/sd")
except ImportError:
    print("boot.py - sdcard module not found, skipping SD card initialization.")
except OSError:
    print("boot.py - Failed to mount SD card, skipping SD card initialization.")

# Set the SPI bus baudrate (note - the sdcard module overrides the baudrate upon
# initialization, so the baudrate should be set after that). It is recommended
# to use the fastest baudrate supported by your board, display, and SD card to
# minimize latency
spi.init(baudrate=24_000_000)

# Attempt to put something on the display to clear the previous content
try:
    # Load and display a splash image, if it's available
    import cv2
    splash_image = cv2.imread("splash.png")
    cv2.imshow(display, splash_image)
except Exception:    
    # Clear the display, if the driver supports it
    if hasattr(display, 'clear'):
        display.clear()

# Initialize camera, if available
try:
    # Import a camera driver module. This example assumes the HM01B0, which is
    # a popular camera module for embedded systems. This example uses a PIO
    # driver, which is a peripheral interface only available on Raspberry Pi RP2
    # processors
    from cv2_drivers.cameras import hm01b0_pio

    # Create a camera object. This will depend on the camera driver you are
    # using, and you may need to adjust the parameters based on your specific
    # camera and board configuration
    camera = hm01b0_pio.HM01B0_PIO(i2c,
                                   pin_d0=12,
                                   pin_vsync=13,
                                   pin_hsync=14,
                                   pin_pclk=15)
except ImportError:
    print("boot.py - Camera driver module not found, skipping camera initialization.")
