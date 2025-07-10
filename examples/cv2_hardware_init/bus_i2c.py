# Import the machine.I2C class
from machine import I2C

# Initialize default I2C bus. You may need to adjust the arguments based on your
# specific board and configuration
i2c = I2C(
    # id = 0,
    # sda = machine.Pin(0),
    # scl = machine.Pin(1),
    # freq = 400_000
)
