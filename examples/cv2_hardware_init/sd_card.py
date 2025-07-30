# Initializes SD card and mounts it to the filesystem. This assumes the SD card
# is on the same SPI bus as the display with a different chip select pin. You
# may need to adjust this based on your specific board and configuration

# Import the Pin class for the chip select pin
from machine import Pin

# Import the SPI bus
from .bus_spi import spi

# When the SD card is initialized, it changes the SPI bus baudrate. We'll
# want to revert it, so we need to know the original baudrate. There's no
# way to get it directly, so we convert the bus to a string and parse it.
# Example format:
# "SPI(0, baudrate=24000000, sck=Pin(2), mosi=Pin(3), miso=Pin(4))"
spi_str = str(spi)
baudrate = int(spi_str[spi_str.index("baudrate=") + 9:].partition(",")[0])

# Set the chip select pin for the SD card
sd_cs = Pin(7, Pin.OUT)

try:
    # Import the SD card module. This is often not installed by default in
    # MicroPython, so you may need to install it manually. For example, you can
    # use `mpremote mip install sdcard`
    import sdcard

    # Initialize the SD card, then restore the original SPI bus baudrate. This
    # is wrapped in a try/finally block to ensure the baudrate is restored even if
    # the SD card initialization fails
    try:
        sd_card = sdcard.SDCard(spi, sd_cs)
    finally:
        spi.init(baudrate = baudrate)

    # Mount the SD card to the filesystem under the "/sd" directory, which makes
    # it accessible just like the normal MicroPython filesystem
    import uos
    vfs = uos.VfsFat(sd_card)
    uos.mount(vfs, "/sd")
except ImportError:
    print("`sdcard` module not found, skipping SD card initialization...")
except OSError as e:
    eStr = str(e)
    if "no SD card" in eStr:
        print("No SD card found, skipping SD card initialization...")
    elif "Errno 1" in eStr:
        print("SD card already mounted, skipping SD card initialization...")
    else:
        print("Failed to mount SD card, skipping SD card initialization...")
