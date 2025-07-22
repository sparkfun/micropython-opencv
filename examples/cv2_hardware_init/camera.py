# Initializes a camera object. Multiple options are provided below, so you can
# choose one that best fits your needs. You may need to adjust the arguments
# based on your specific camera and board configuration

# Import the OpenCV camera drivers
from cv2_drivers.cameras import *

# Import the I2C bus
from .bus_i2c import i2c

################################################################################
# HM01B0
################################################################################

# PIO interface, only available on Raspberry Pi RP2 processors
camera = hm01b0_pio.HM01B0_PIO(
    i2c,
    pin_d0 = 12,
    pin_vsync = 13,
    pin_hsync = 14,
    pin_pclk = 15,
    sm_id = 5,
    pin_xclk = None, # Optional xclock pin, specify if needed
    num_data_pins = 1 # Number of data pins used by the camera (1, 4, or 8)
)

################################################################################
# OV5640
################################################################################

# PIO interface, only available on Raspberry Pi RP2 processors
# camera = ov5640_pio.OV5640_PIO(
#     i2c,
#     sm_id = 5,
#     pin_d0 = 8,
#     pin_vsync = 22,
#     pin_hsync = 21,
#     pin_pclk = 20,
#     pin_xclk = 3 # Optional xclock pin, specify if needed
# )
