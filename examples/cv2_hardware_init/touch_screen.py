# Initializes a touch screen object. Multiple options are provided below, so you
# can choose one that best fits your needs. You may need to adjust the arguments
# based on your specific touch screen and board configuration

# Import the OpenCV touch screen drivers
from cv2_drivers.touch_screens import *

# Import the I2C bus
from .bus_i2c import i2c

################################################################################
# CST816
################################################################################

# I2C interface
touch_screen = cst816.CST816(i2c)
