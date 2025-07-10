# Initializes a display object. Multiple options are provided below, so you can
# choose one that best fits your needs. You may need to adjust the arguments
# based on your specific display and board configuration

# Import the OpenCV display drivers
from cv2_drivers.displays import *

# Import the SPI bus
from .bus_spi import spi

################################################################################
# ST7789
################################################################################

# SPI interface. This should work on any platform, but it's not always the
# fastest option (24Mbps on RP2350)
display = st7789_spi.ST7789_SPI(
    width = 240,
    height = 320,
    spi = spi,
    pin_dc = 16,
    pin_cs = 17,
    rotation = 1
)

# PIO interface. This is only available on Raspberry Pi RP2 processors,
# but is much faster than the SPI interface (75Mbps on RP2350)
# display = st7789_pio.ST7789_PIO(
#     width = 240,
#     height = 320,
#     sm_id = 1,
#     pin_clk = 18,
#     pin_tx = 19,
#     pin_dc = 16,
#     pin_cs = 17,
#     rotation = 1
# )
