# Import the machine.SPI class
from machine import SPI

# Initialize default SPI bus. You may need to adjust the arguments based on your
# specific board and configuration
spi = SPI(
    # id = 0,
    baudrate = 24_000_000, # Use the fastest baudrate you can for best performance!
    # sck = machine.Pin(2),
    # mosi = machine.Pin(3),
    # miso = machine.Pin(4),
    # freq = 100_000
)
