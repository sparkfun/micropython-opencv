# Import platform agnostic drivers
from . import st7789_spi

# Import sys module to check platform
import sys

# Import RP2 drivers
if 'rp2' in sys.platform:
    from . import st7789_pio
